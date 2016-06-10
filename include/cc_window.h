/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_WINDOW_H__
#define __CC_WINDOW_H__

#include <stdint.h>

enum cc_window_flag {
	CC_WINDOW_ALWAYS_ON_TOP = 1 << 1,
	CC_WINDOW_NO_BUTTONS =    1 << 2,
	CC_WINDOW_NO_RESIZE =     1 << 3
};

enum cc_cursor {
	CC_CURSOR_ARROW = 0,
	CC_CURSOR_CROSS,
	CC_CURSOR_BEAM,
	CC_CURSOR_MOVE,
	CC_CURSOR_HAND,
	CC_CURSOR_SIZEH,
	CC_CURSOR_SIZEV,
	CC_CURSOR_NO,
	CC_CURSOR_QUESTION,
	CC_CURSOR_NONE
};

int cc_new_window(enum cc_window_flag flags);
int cc_destroy_window(void);

int cc_blink_window(void);

/* Setters */
int cc_set_window_windowed(void);
int cc_set_window_maximized(void);
int cc_set_window_centered(void);
int cc_set_window_fullscreen_on_current_screen(void);
int cc_set_window_fullscreen_on_multiple_screens(void);

int cc_set_window_title(const char *title);
int cc_set_window_icon(int width, int height, const uint32_t *data);
int cc_set_window_position(int x, int y);
int cc_set_window_size(int width, int height);

int cc_set_mouse_position(int x, int y);
int cc_set_mouse_cursor(enum cc_cursor cursor);

/* Getters */
int cc_get_window_coordinates(int *x, int *y);
int cc_get_window_size(int *width, int *height);

int cc_get_mouse_coordinates(int *x, int *y);

#endif /* __CC_WINDOW_H__ */
