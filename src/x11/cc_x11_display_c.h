#ifndef __CC_X11_DISPLAY_C_H__
#define __CC_X11_DISPLAY_C_H__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

struct cc_resolution {
	int width, height, refresh_rate;
	RRMode mode;
};

struct cc_display {
	int x, y, screen, resolution_amount, current_resolution;
	unsigned short dpi;
	char *monitor_name, *display_name;
	struct cc_resolution *resolutions;
	RROutput output;
	RRMode old_mode;
};

#endif /* __CC_X11_DISPLAY_C_H__ */
