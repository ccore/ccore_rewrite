#include "cc_win32_window_c.h"
#include <cc_window.h>

#include <cc_error.h>
#include <cc_event.h>
#include <cc_display.h>

#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Local functions */

int cc_poll_window(void)
{	
	return 1;
}

/* Public functions */

int cc_new_window(enum cc_window_flag flags)
{
	return 1;
}

int cc_destroy_window(void)
{	
	return 1;
}

int cc_blink_window(void)
{
	return 1;
}

int cc_capture_mouse(void)
{	return 1;
}

int cc_release_mouse(void)
{
	return 1;
}

/* Setters */
int cc_set_window_windowed(void)
{
	return 1;
}

int cc_set_window_maximized(void)
{
	return 1;
}

int cc_set_window_centered(void)
{	
	return 1;
}

int cc_set_window_fullscreen_on_screen(int display_id)
{	
	return 1;
}

int cc_set_window_fullscreen_on_multiple_screens(int left_display_id, int right_display_id, int top_display_id, int bottom_display_id)
{	
	return 1;
}

int cc_set_window_title(const char *title)
{
	return 1;
}

int cc_set_window_icon(int width, int height, const uint32_t *data)
{	
	return 1;
}

int cc_set_window_position(int x, int y)
{	
	return 1;
}

int cc_set_window_size(int width, int height)
{	
	return 1;
}

int cc_set_mouse_position(int x, int y)
{	
	return 1;
}

int cc_set_mouse_cursor(enum cc_cursor cursor)
{	
	return 1;
}

/* Getters */

int cc_get_window_coordinates(int *x, int *y)
{	
	return 1;
}

int cc_get_window_size(int *width, int *height)
{	
	return 1;
}

int cc_get_mouse_coordinates(int *x, int *y)
{	
	return 1;
}
