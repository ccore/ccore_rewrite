#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>

#include <stdlib.h>
#include <string.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

static Window _win;
static Display *_dpy = NULL;
static int _screen, _width, _height;

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
	Atom delete_atom;

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

	XSetErrorHandler(cc_handle_x_error);

	_dpy = XOpenDisplay(NULL);
	_screen = DefaultScreen(_dpy);
	_win = XCreateWindow(_dpy, RootWindow(_dpy, _screen), 0, 0, _width, _height, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);
	XSelectInput(_dpy, _win, PropertyChangeMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | FocusChangeMask);

	if(flags & CC_WINDOW_NO_RESIZE){
		cc_set_resizable(0);
	}
	
	XMapWindow(_dpy, _win);
	XStoreName(_dpy, _win, "ccore");

	delete_atom = XInternAtom(_dpy, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(_dpy, _win, &delete_atom, 1);

	if(flags & CC_WINDOW_ALWAYS_ON_TOP){
		cc_set_window_state("_NET_WM_STATE_ABOVE", True);
	}

	cc_clear_events();

	return 1;
}

int cc_free_window(void)
{
	XUnmapWindow(_dpy, _win);
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
int cc_set_window_windowed(void);
int cc_set_window_maximized(void);
int cc_set_window_centered(void);

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

int cc_set_window_icon(unsigned width, unsigned height, const uint32_t *data)
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

int cc_set_window_position(int x, int y);
int cc_set_window_size(unsigned width, unsigned height);

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

int cc_set_clipboard(const char *data);

/* Getters */
int cc_get_window_x(void);
int cc_get_window_y(void);
int cc_get_window_width(void);
int cc_get_window_height(void);

const char *cc_get_clipboard(void);
