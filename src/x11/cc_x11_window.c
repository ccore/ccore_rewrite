#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>

#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

static Window _win;
static Display *_dpy = NULL;
static int _screen, _width, _height;

/* Private functions */

static int cc_set_window_state(const char *type, int value)
{
	Atom wm_state, new_wm_state;
	XEvent event;

	if(_dpy == NULL){
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

	if(_dpy == NULL){
		cc_no_window_error();
		return 0;
	}

	size_hints = NULL;
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
		cc_set_window_state("_NET_WM_STATE_ABOVE", 0);
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
	return cc_set_window_state("_NET_WM_STATE_DEMANDS_ATTENTION", 1);
}

/* Setters */
int cc_set_window_windowed(void);
int cc_set_window_maximized(void);
int cc_set_window_centered(void);

int cc_set_window_title(const char *title);
int cc_set_window_icon(unsigned width, unsigned height, const unsigned char *data);

int cc_set_window_position(int x, int y);
int cc_set_window_size(unsigned width, unsigned height);

int cc_set_mouse_position(void);
int cc_set_mouse_cursor(enum cc_cursor cursor);

int cc_set_clipboard(const char *data);

/* Getters */
int cc_get_window_x(void);
int cc_get_window_y(void);
int cc_get_window_width(void);
int cc_get_window_height(void);

int cc_get_mouse_x(void);
int cc_get_mouse_y(void);

const char *cc_get_clipboard(void);
