/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_WINDOW_H__
#define __CC_WINDOW_H__

enum cc_window_flag {
	CC_WINDOW_ALWAYS_ON_TOP = 1 << 1,
	CC_WINDOW_NO_BUTTONS =    1 << 2,
	CC_WINDOW_NO_RESIZE =     1 << 3
};

int cc_new_window(int flags);
int cc_free_window();

#endif /* __CC_WINDOW_H__ */
