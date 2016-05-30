#include <cc_opengl.h>

#include <cc_error.h>

#include <GL/glx.h>

#include "cc_x11_window_c.h"

static GLXContext _context;

static int _attribute_list[] = {GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, 16, None};

/* Public functions */

int cc_bind_opengl_context(void)
{
	Display *dpy;
	Window win;
	int screen;
	XVisualInfo *visual;

	dpy = cc_get_x_display();
	if(!dpy){
		cc_no_window_error();
		return 0;
	}

	screen = cc_get_x_screen();
	visual = glXChooseVisual(dpy, screen, _attribute_list);
	if(!visual){
		cc_set_error("Could not create OpenGL GLX visual");
		return 0;
	}

	_context = glXCreateContext(dpy, visual, NULL, GL_TRUE);
	if(!_context){
		cc_set_error("Could not create OpenGL GLX context");
		return 0;
	}

	win = cc_get_x_window();
	glXMakeCurrent(dpy, win, _context);

	return 1;
}

int cc_free_opengl_context(void)
{
	Display *dpy;

	dpy = cc_get_x_display();
	if(!dpy){
		cc_no_window_error();
		return 0;
	}

	glXDestroyContext(dpy, _context);

	return 1;
}

int cc_swap_opengl_buffers(void)
{
	Display *dpy;
	Window win;

	dpy = cc_get_x_display();
	win = cc_get_x_window();
	glXSwapBuffers(dpy, win);
	
	return 1;
}
