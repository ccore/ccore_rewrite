#include "cc_x11_window_c.h"
#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>
#include <cc_display.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xrandr.h>

static Window _window;
static Display *_display = NULL;
static int _attr, _screen, _x, _y, _width, _height, _mouse_x, _mouse_y, _need_redraw;

static XID _cursor;
static Pixmap _cursor_image;
static const int _cursor_list[] = {XC_arrow, XC_crosshair, XC_xterm, XC_fleur, XC_hand1, XC_sb_h_double_arrow, XC_sb_v_double_arrow, XC_X_cursor, XC_question_arrow};
static const char _empty_cursor_data[] = {0, 0, 0, 0, 0, 0, 0, 0};

static int (*_default_error_handler)(Display*, XErrorEvent*);

/* Private functions */

static int cc_handle_x_error(Display *display, XErrorEvent *event)
{
	char error[256];

	XGetErrorText(_display, event->error_code, error, sizeof(error));
	cc_set_error("Internal X11 error: %s", error);

	return 0;
}

static int cc_set_window_state(const char *type, int value)
{
	Atom wm_state, new_wm_state;
	XEvent event;

	if(!_display){
		cc_no_window_error();
		return 0;
	}

	wm_state = XInternAtom(_display, "_NET_WM_STATE", 1);
	new_wm_state = XInternAtom(_display, type, 1);

	event.type = ClientMessage;
	event.xclient.window = _window;
	event.xclient.message_type = wm_state;
	event.xclient.format = 32;
	event.xclient.data.l[0] = value;
	event.xclient.data.l[1] = new_wm_state;

	XSendEvent(_display, DefaultRootWindow(_display), 1, SubstructureNotifyMask, &event);

	return 1;
}

static int cc_set_resizable(int resizable)
{
	XSizeHints *size_hints;
	long flags;

	if(!_display){
		cc_no_window_error();
		return 0;
	}

	size_hints = XAllocSizeHints();
	flags = 0;
	XGetWMNormalHints(_display, _window, size_hints, &flags);

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

	XSetWMNormalHints(_display, _window, size_hints);

	XFree(size_hints);

	return 1;
}

/* Local functions */
Window cc_get_x_window(void)
{
	return _window;
}

Display *cc_get_x_display(void)
{
	return _display;
}

int cc_get_x_screen(void)
{
	return _screen;
}

void cc_set_need_redraw(void)
{
	_need_redraw = 1;
}

/* Public functions */

int cc_new_window(enum cc_window_flag flags)
{
	Atom wm_window_type_atom, wm_window_type_dialog_atom, delete_atom;

	if(_display != NULL){
		cc_set_error("ccore can only create a single window");
		return 0;
	}

	if(_width == 0){
		_width = 100;
	}
	if(_height == 0){
		_height = 100;
	}

	_x = 0;
	_y = 0;
	_attr = 0;
	_need_redraw = 1;

	_default_error_handler = XSetErrorHandler(cc_handle_x_error);

	_display = XOpenDisplay(NULL);
	_screen = DefaultScreen(_display);
	_window = XCreateWindow(_display, RootWindow(_display, _screen), _x, _y, _width, _height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);
	XSelectInput(_display, _window, PropertyChangeMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | FocusChangeMask);

	if(flags & CC_WINDOW_NO_RESIZE){
		cc_set_resizable(0);
	}else{
		_attr |= CC_WINDOW_ATTR_RESIZABLE;
	}

	if(flags & CC_WINDOW_NO_BUTTONS){
		wm_window_type_atom = XInternAtom(_display, "_NET_WM_WINDOW_TYPE", False);
		wm_window_type_dialog_atom = XInternAtom(_display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
		XChangeProperty(_display, _window, wm_window_type_atom, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wm_window_type_dialog_atom, 1);
	}else{
		_attr |= CC_WINDOW_ATTR_BUTTONS;
	}

	XMapWindow(_display, _window);
	XStoreName(_display, _window, "ccore");

	delete_atom = XInternAtom(_display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(_display, _window, &delete_atom, 1);

	if(flags & CC_WINDOW_ALWAYS_ON_TOP){
		cc_set_window_state("_NET_WM_STATE_ABOVE", True);
	}
	
	cc_clear_event_queue();

	_cursor_image = XCreateBitmapFromData(_display, _window, _empty_cursor_data, 8, 8);

	return 1;
}

int cc_destroy_window(void)
{
	if(_cursor != 0){
		XFreeCursor(_display, _cursor);
	}
	XFreePixmap(_display, _cursor_image);

	XDestroyWindow(_display, _window);
	XCloseDisplay(_display);

	XSetErrorHandler(_default_error_handler);

	return 1;
}

int cc_poll_window(void)
{
	struct cc_event event;
	XEvent ev, next_ev;

	while(XPending(_display)){
		XNextEvent(_display, &ev);
		switch(ev.type){
			case Expose:
				_need_redraw = 0;

				event.type = CC_EVENT_DRAW;
				cc_push_event(event);
				break;
			case KeyPress:
				event.type = CC_EVENT_PRESS_KEY;
				event.data.key_code = XLookupKeysym(&ev.xkey, 0);
				cc_push_event(event);
				break;
			case KeyRelease:
				/* Check for a fake key release which is triggered by auto repeat */
				if(XEventsQueued(_display, QueuedAfterReading)){
					XPeekEvent(_display, &next_ev);
					if(next_ev.type == KeyPress && next_ev.xkey.time == ev.xkey.time && next_ev.xkey.keycode == ev.xkey.keycode){
						break;
					}
				}
				event.type = CC_EVENT_RELEASE_KEY;
				event.data.key_code = XLookupKeysym(&ev.xkey, 0);
				cc_push_event(event);
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
				cc_push_event(event);
				break;
			case ButtonRelease:
				if(ev.xbutton.button != 4 && ev.xbutton.button != 5){
					event.type = CC_EVENT_RELEASE_MOUSE;
					event.data.mouse_button = ev.xbutton.button;
					cc_push_event(event);
				}
				break;
			case MotionNotify:
				if(_mouse_x != ev.xmotion.x || _mouse_y != ev.xmotion.y){
					event.type = CC_EVENT_MOVE_MOUSE;
					cc_push_event(event);
					_mouse_x = ev.xmotion.x;
					_mouse_y = ev.xmotion.y;
				}
				break;
			case ConfigureNotify:
				if(_width != ev.xconfigure.width || _height != ev.xconfigure.height){
					event.type = CC_EVENT_RESIZE;
					cc_push_event(event);
					_width = ev.xconfigure.width;
					_height = ev.xconfigure.height;
				}
				if(_x != ev.xconfigure.x || _y != ev.xconfigure.y){
					event.type = CC_EVENT_MOVE;
					cc_push_event(event);
					_x = ev.xconfigure.x;
					_y = ev.xconfigure.y;
				}
				break;
			case FocusIn:
				event.type = CC_EVENT_GAIN_FOCUS;
				cc_push_event(event);
				break;
			case FocusOut:
				event.type = CC_EVENT_LOSE_FOCUS;
				cc_push_event(event);
				break;
			case ClientMessage:
				cc_clear_event_queue();

				event.type = CC_EVENT_QUIT;
				cc_push_event(event);
				return 0;
		}
	}

	if(_need_redraw){
		_need_redraw = 0;

		event.type = CC_EVENT_DRAW;
		cc_push_event(event);
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
	if(!_display){
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
	if(!_display){
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
	int i, display_count, default_resolution, new_x, new_y;
	struct cc_display_info display;
	struct cc_resolution_info resolution;

	/* Find the screen the window is in, this might be improved */
	display_count = cc_get_display_count();
	for(i = 0; i < display_count; i++){
		cc_get_display_info(i, &display);
		if(_x < display.x || _y < display.y){
			continue;
		}

		default_resolution = cc_get_default_resolution_id(i);
		cc_get_resolution_info(i, default_resolution, &resolution);
		if(_x > display.x + resolution.width || _y > display.y + resolution.height){
			continue;
		}

		new_x = display.x + ((resolution.width - _width) >> 1);
		new_y = display.y + ((resolution.height - _height) >> 1);
		XMoveWindow(_display, _window, new_x, new_y);
		return 1;
	}
	
	return 0;
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

	if(!_display){
		cc_no_window_error();
		return 0;
	}

	wm_name_atom = XInternAtom(_display, "WM_NAME", True);
	wm_icon_name_atom = XInternAtom(_display, "WM_ICON_NAME", True);
	utf8_string_atom = XInternAtom(_display, "UTF8_STRING", True);

#ifdef X_HAVE_UTF8_STRING
	len = strlen(title);
	XChangeProperty(_display, _window, wm_name_atom, utf8_string_atom, 8, PropModeReplace, (unsigned char*)title, len);
	XChangeProperty(_display, _window, wm_icon_name_atom, utf8_string_atom, 8, PropModeReplace, (unsigned char*)title, len);
#else
	titleCopy = strdup(title);
	if(!XStringListToTextProperty(&titleCopy, 1, &titleProperty)) {
		cc_set_error("Internal X11 string conversion error");
		return 0;
	}
	free(titleCopy);

	XSetTextProperty(_display, _window, &titleProperty, _WM_NAME);
	XSetTextProperty(_display, _window, &titleProperty, _WM_ICON_NAME);
	XSetWMName(_display, _window, &titleProperty);
	XSetWMIconName(_display, _window, &titleProperty);
	XFree(titleProperty.value);
#endif

	return 1;
}

int cc_set_window_icon(int width, int height, const uint32_t *data)
{
	size_t data_len, total_len, i;
	unsigned long *data_copy;
	Atom wm_icon_atom;

	wm_icon_atom = XInternAtom(_display, "_NET_WM_ICON", False);
	if(!data){
		XDeleteProperty(_display, _window, wm_icon_atom);
		return 0;
	}

	if(width == 0 || height == 0){
		cc_invalid_parameter_error("width or height");
		return 0;
	}

	if(!_display){
		cc_no_window_error();
		return 0;
	}

	data_len = width * height;
	total_len = data_len + 2;
	data_copy = (unsigned long*)malloc(total_len * sizeof(unsigned long));
	if(!data_copy){
		cc_out_of_memory_error();
		return 0;
	}

	data_copy[0] = width;
	data_copy[1] = height;
	if(sizeof(unsigned long) == sizeof(uint32_t)){
		memcpy(data_copy + 2, data, data_len * sizeof(unsigned long));
	}else{
		for(i = 0; i < data_len; i++){
			data_copy[i + 2] = data[i];
		}
	}

	XChangeProperty(_display, _window, wm_icon_atom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data_copy, total_len);

	free(data_copy);

	return 1;
}

int cc_set_window_position(int x, int y)
{
	if(!_display){
		cc_no_window_error();
		return 0;
	}

	XMoveWindow(_display, _window, x, y);

	_x = x;
	_y = y;

	return 1;
}

int cc_set_window_size(int width, int height)
{
	if(_display){
		if(!(_attr & CC_WINDOW_ATTR_RESIZABLE)){
			cc_set_error("Can not resize a window where the NO_RESIZE flag has been set, call \"cc_set_window_size\" before \"cc_new_window\" if you want to set the initial size");
			return 0;
		}
		XResizeWindow(_display, _window, width, height);
	}	

	_width = width;
	_height = height;

	return 1;
}

int cc_set_mouse_position(int x, int y)
{
	if(!_display){
		cc_no_window_error();
		return 0;
	}

	XWarpPointer(_display, None, _window, 0, 0, 0, 0, x, y);

	return 1;
}

int cc_set_mouse_cursor(enum cc_cursor cursor)
{
	XColor black;

	if(!_display){
		cc_no_window_error();
		return 0;
	}
	
	if(_cursor != 0){
		XFreeCursor(_display, _cursor);
	}

	if(cursor == CC_CURSOR_NONE){
		memset(&black, 0, sizeof(XColor));
		_cursor = XCreatePixmapCursor(_display, _cursor_image, _cursor_image, &black, &black, 0, 0);
	}else{
		_cursor = XCreateFontCursor(_display, _cursor_list[cursor]);
	}

	if(!_cursor){
		cc_set_error("Could not create X11 cursor");
		return 0;
	}

	XDefineCursor(_display, _window, _cursor);

	return 1;
}

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
