#ifndef __CC_X11_WINDOW_C_H__
#define __CC_X11_WINDOW_C_H__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

enum cc_window_attributes {
	CC_WINDOW_ATTR_RESIZABLE =  1 << 1,
	CC_WINDOW_ATTR_FULLSCREEN = 1 << 2,
	CC_WINDOW_ATTR_BUTTONS =    1 << 3
};

Window cc_get_x_window(void);
Display *cc_get_x_display(void);
int cc_get_x_screen(void);

void cc_set_need_redraw(void);
int cc_poll_window(void);

#endif /* __CC_X11_WINDOW_C_H__ */
