/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_EVENT_H__
#define __CC_EVENT_H__

enum cc_event_type {
	CC_EVENT_SKIP = 0,

	CC_EVENT_DRAW,

	CC_EVENT_QUIT,
	CC_EVENT_RESIZE,
	CC_EVENT_MOVE,

	CC_EVENT_GAIN_FOCUS,
	CC_EVENT_LOSE_FOCUS,

	CC_EVENT_PRESS_MOUSE,
	CC_EVENT_RELEASE_MOUSE,
	CC_EVENT_MOVE_MOUSE,
	CC_EVENT_SCROLL_MOUSE,

	CC_EVENT_PRESS_KEY,
	CC_EVENT_RELEASE_KEY,

	CC_EVENT_DISPLAY_CHANGE
};

struct cc_event {
	int type;

	union {
		int mouse_button;
		int mouse_scroll_delta;
		int key_code;
	} data;
};

int cc_push_event(struct cc_event event);
struct cc_event cc_pop_event(void);
void cc_clear_event_queue(void);

#endif /* __CC_EVENT_H__ */
