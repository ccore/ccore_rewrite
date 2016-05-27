#include <cc_window.h>
#include <cc_error.h>

#include <stdlib.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static Window _win;
static Display *_dpy = NULL;
static int _screen;

/* Private functions */



/* Public functions */

int cc_new_window(int flags)
{
	if(_dpy != NULL){
		cc_set_error("ccore can only create a single window");
		return EXIT_FAILURE;
	}

	_dpy = XOpenDisplay(NULL);
	_screen = DefaultScreen(_dpy);
	_win = XCreateWindow(_dpy, RootWindow(_dpy, _screen), 0, 0, 100, 100, 0, CopyFromParent, InputOutput, CopyFromParent, 0, 0);

	XSelectInput(_dpy, _win, PropertyChangeMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | FocusChangeMask);
	
	XMapWindow(_dpy, _win);
	XStoreName(_dpy, _win, "");

	return EXIT_SUCCESS;
}

int cc_free_window()
{

	return EXIT_SUCCESS;
}
