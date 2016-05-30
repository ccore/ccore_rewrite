#include "cc_x11_window_c.h"
#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

static Window _win;
static Display *_dpy = NULL;
static int _attr, _screen, _x, _y, _width, _height, _mouse_x, _mouse_y;
static int (*_default_error_handler)(Display*, XErrorEvent*);

/* Local getters */
Window cc_get_x_window(void)
{
	return _win;
}

Display *cc_get_x_display(void)
{
	return _dpy;
}

int cc_get_x_screen(void)
{
	return _screen;
}

/* Private functions */

static int cc_handle_x_error(Display *display, XErrorEvent *event)
{
	char error[256];

	XGetErrorText(_dpy, event->error_code, error, sizeof(error));
	cc_set_error("Internal X11 error: %s", error);

	return 0;
}

static int cc_set_window_state(const char *type, int value)
{
	Atom wm_state, new_wm_state;
	XEvent event;

	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	wm_state = XInternAtom(_dpy, "_NET_WM_STATE", 1);
	new_wm_state = XInternAtom(_dpy, type, 1);

	event.type = ClientMessage;
	event.xclient.window = _win;
	event.xclient.message_type = wm_state;
	event.xclient.format = 32;
	event.xclient.data.l[0] = value;
	event.xclient.data.l[1] = new_wm_state;

	XSendEvent(_dpy, DefaultRootWindow(_dpy), 1, SubstructureNotifyMask, &event);

	return 1;
}

static int cc_set_resizable(int resizable)
{
	XSizeHints *size_hints;
	long flags;

	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	size_hints = XAllocSizeHints();
	flags = 0;
	XGetWMNormalHints(_dpy, _win, size_hints, &flags);

	if(resizable){
		size_hints->flags &= ~(PMinSize | PMaxSize);
		size_hints->min_width = 0;
		size_hints->max_width = 0;
		size_hints->min_height = INT_MAX;
		size_hints->max_height = INT_MAX;
	}else{
		size_hints->flags |= PMinSize | PMaxSize;
		size_hints->min_width = _width;
		size_hints->max_width = _width;
		size_hints->min_height = _height;
		size_hints->max_height = _height;
	}

	XSetWMNormalHints(_dpy, _win, size_hints);

	XFree(size_hints);

	return 1;
}

/* Public functions */

int cc_new_window(enum cc_window_flag flags)
{
	Atom wm_window_type_atom, wm_window_type_dialog_atom, delete_atom;
	XEvent ev_shill;

	if(_dpy != NULL){
		cc_set_error("ccore can only create a single window");
		return 0;
	}

	if(_width == 0){
		_width = 100;
	}
	if(_height == 0){
		_height = 100;
	}
	_attr = 0;

	_default_error_handler = XSetErrorHandler(cc_handle_x_error);

	_dpy = XOpenDisplay(NULL);
	_screen = DefaultScreen(_dpy);
	_win = XCreateWindow(_dpy, RootWindow(_dpy, _screen), 0, 0, _width, _height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);
	XSelectInput(_dpy, _win, PropertyChangeMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | FocusChangeMask);

	if(flags & CC_WINDOW_NO_RESIZE){
		cc_set_resizable(0);
	}else{
		_attr |= CC_WINDOW_ATTR_RESIZABLE;
	}

	if(flags & CC_WINDOW_NO_BUTTONS){
		wm_window_type_atom = XInternAtom(_dpy, "_NET_WM_WINDOW_TYPE", False);
		wm_window_type_dialog_atom = XInternAtom(_dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
		XChangeProperty(_dpy, _win, wm_window_type_atom, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wm_window_type_dialog_atom, 1);
	}else{
		_attr |= CC_WINDOW_ATTR_BUTTONS;
	}

	XMapWindow(_dpy, _win);
	XStoreName(_dpy, _win, "ccore");

	delete_atom = XInternAtom(_dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(_dpy, _win, &delete_atom, 1);

	if(flags & CC_WINDOW_ALWAYS_ON_TOP){
		cc_set_window_state("_NET_WM_STATE_ABOVE", True);
	}
	
	cc_clear_event_queue();

	_x = 0;
	_y = 0;

	return 1;
}

int cc_destroy_window(void)
{
	XSetErrorHandler(_default_error_handler);

	XDestroyWindow(_dpy, _win);
	XCloseDisplay(_dpy);

	return 1;
}

int cc_poll_window(void)
{
	struct cc_event event;
	XEvent ev;

	event.type = CC_EVENT_SKIP;

	if(!XPending(_dpy)){
		return 1;
	}

	XNextEvent(_dpy, &ev);
	switch(ev.type){
		case Expose:
			event.type = CC_EVENT_DRAW;
			break;
		case KeyPress:
			event.type = CC_EVENT_PRESS_KEY;
			event.data.key_code = XLookupKeysym(&ev.xkey, 0);
			break;
		case KeyRelease:
			event.type = CC_EVENT_RELEASE_KEY;
			event.data.key_code = XLookupKeysym(&ev.xkey, 0);
			break;
		case KeymapNotify:
			XRefreshKeyboardMapping(&ev.xmapping);
			break;
		case ButtonPress:
			if(ev.xbutton.button == 4){
				event.type = CC_EVENT_SCROLL_MOUSE;
				event.data.mouse_scroll_delta = 1;
			}else if(ev.xbutton.button == 5){
				event.type = CC_EVENT_SCROLL_MOUSE;
				event.data.mouse_scroll_delta = -1;
			}else{
				event.type = CC_EVENT_PRESS_MOUSE;
				event.data.mouse_button = ev.xbutton.button;
			}
			break;
		case ButtonRelease:
			if(ev.xbutton.button != 4 && ev.xbutton.button != 5){
				event.type = CC_EVENT_RELEASE_MOUSE;
				event.data.mouse_button = ev.xbutton.button;
			}
			break;
		case MotionNotify:
			if(_mouse_x != ev.xmotion.x || _mouse_y != ev.xmotion.y){
				event.type = CC_EVENT_MOVE_MOUSE;
				_mouse_x = ev.xmotion.x;
				_mouse_y = ev.xmotion.y;
			}
			break;
		case ConfigureNotify:
			if(_width != ev.xconfigure.width || _height != ev.xconfigure.height){
				event.type = CC_EVENT_RESIZE;
				_width = ev.xconfigure.width;
				_height = ev.xconfigure.height;
			}
			_x = ev.xconfigure.x;
			_y = ev.xconfigure.y;
			break;
		case FocusIn:
			event.type = CC_EVENT_GAIN_FOCUS;
			break;
		case FocusOut:
			event.type = CC_EVENT_LOSE_FOCUS;
			break;
		case ClientMessage:
			event.type = CC_EVENT_QUIT;
			cc_push_event(event);
			return 0;
	}

	if(event.type != CC_EVENT_SKIP){
		return cc_push_event(event);
	}

	return 1;
}

int cc_blink_window(void)
{
	return cc_set_window_state("_NET_WM_STATE_DEMANDS_ATTENTION", True);
}

/* Setters */
int cc_set_window_windowed(void)
{
	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	if(!cc_set_window_state("_NET_WM_STATE_FULLSCREEN", False)){
		return 0;
	}else if(!cc_set_window_state("_NET_WM_STATE_MAXIMIZED_VERT", False)){
		return 0;
	}
	return cc_set_window_state("_NET_WM_STATE_MAXIMIZED_HORZ", False);
}

int cc_set_window_maximized(void)
{
	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	if(!cc_set_window_state("_NET_WM_STATE_MAXIMIZED_VERT", True)){
		return 0;
	}
	return cc_set_window_state("_NET_WM_STATE_MAXIMIZED_HORZ", True);
}

int cc_set_window_centered(void)
{
	//TODO
	
	return 1;
}

int cc_set_window_fullscreen_on_current_screen(void)
{
	cc_set_window_state("_NET_WM_STATE_FULLCREEN", False);
	
	return 1;
}

int cc_set_window_title(const char *title)
{
#ifdef X_HAVE_UTF8_STRING
	size_t len;
#else
	char *titleCopy;
	XTextProperty titleProperty;
#endif
	Atom wm_name_atom, wm_icon_name_atom, utf8_string_atom;

	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	wm_name_atom = XInternAtom(_dpy, "WM_NAME", True);
	wm_icon_name_atom = XInternAtom(_dpy, "WM_ICON_NAME", True);
	utf8_string_atom = XInternAtom(_dpy, "UTF8_STRING", True);

#ifdef X_HAVE_UTF8_STRING
	len = strlen(title);
	XChangeProperty(_dpy, _win, wm_name_atom, utf8_string_atom, 8, PropModeReplace, (unsigned char*)title, len);
	XChangeProperty(_dpy, _win, wm_icon_name_atom, utf8_string_atom, 8, PropModeReplace, (unsigned char*)title, len);
#else
	titleCopy = strdup(title);
	if(!XStringListToTextProperty(&titleCopy, 1, &titleProperty)) {
		cc_set_error("Internal X11 string conversion error");
		return 0;
	}
	free(titleCopy);

	XSetTextProperty(_dpy, _win, &titleProperty, _WM_NAME);
	XSetTextProperty(_dpy, _win, &titleProperty, _WM_ICON_NAME);
	XSetWMName(_dpy, _win, &titleProperty);
	XSetWMIconName(_dpy, _win, &titleProperty);
	XFree(titleProperty.value);
#endif

	return 1;
}

int cc_set_window_icon(int width, int height, const uint32_t *data)
{
	size_t data_len, total_len;
	uint32_t *data_copy;
	Atom wm_icon_atom;

	if(width == 0 || height == 0){
		cc_invalid_parameter_error("width or height");
		return 0;
	}

	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	data_len = width * height;
	total_len = data_len + 2;
	data_copy = (uint32_t*)malloc(total_len * sizeof(uint32_t));
	if(data_copy == NULL){
		cc_out_of_memory_error();
		return 0;
	}

	data_copy[0] = (uint32_t)width;
	data_copy[1] = (uint32_t)height;
	memcpy(data_copy + 2, data, data_len * sizeof(uint32_t));

	wm_icon_atom = XInternAtom(_dpy, "_NET_WM_ICON", False);
	XChangeProperty(_dpy, _win, wm_icon_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data_copy, total_len);

	free(data_copy);

	return 1;
}

int cc_set_window_position(int x, int y)
{
	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	XMoveResizeWindow(_dpy, _win, x, y, _width, _height);

	_x = x;
	_y = y;

	return 1;
}

int cc_set_window_size(int width, int height)
{
	if(_dpy){
		if(!(_attr & CC_WINDOW_ATTR_RESIZABLE)){
			cc_set_error("Can not resize a window where the NO_RESIZE flag has been set, call \"cc_set_window_size\" before \"cc_new_window\" if you want to set the initial size");
			return 0;
		}
		XMoveResizeWindow(_dpy, _win, _x, _y, width, height);
	}	

	_width = width;
	_height = height;

	return 1;
}

int cc_set_mouse_position(int x, int y)
{
	if(!_dpy){
		cc_no_window_error();
		return 0;
	}

	XWarpPointer(_dpy, None, _win, 0, 0, 0, 0, x, y);

	return 1;
}

int cc_set_mouse_cursor(enum cc_cursor cursor);

/* Getters */

int cc_get_window_x(void)
{
	return _x;
}

int cc_get_window_y(void)
{
	return _y;
}

int cc_get_window_width(void)
{
	return _width;
}

int cc_get_window_height(void)
{
	return _height;
}

int cc_get_mouse_x(void)
{
	return _mouse_x;
}

int cc_get_mouse_y(void)
{
	return _mouse_y;
}
