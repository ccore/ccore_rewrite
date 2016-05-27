#ifndef __CC_EVENT_H__
#define __CC_EVENT_H__

enum cc_event_type {
	CC_EVENT_SKIP = 0,

	CC_EVENT_QUIT,
	CC_EVENT_RESIZE,

	CC_EVENT_GAIN_FOCUS,
	CC_EVENT_LOSE_FOCUS,

	CC_EVENT_PRESS_MOUSE,
	CC_EVENT_RELEASE_MOUSE,
	CC_EVENT_MOVE_MOUSE,
	CC_EVENT_SCROLL_MOUSE,

	CC_EVENT_PRESS_KEY,
	CC_EVENT_RELEASE_KEY
};

struct cc_event {
	int type;
};

int cc_push_event(struct cc_event event);
struct cc_event cc_pop_event(void);
void cc_clear_events(void);

#endif /* __CC_EVENT_H__ */
