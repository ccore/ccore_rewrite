#include "cc_event_c.h"
#include <cc_event.h>

#include <cc_error.h>
#include <cc_system.h>

#ifdef CC_OS_LINUX
#include "linux/cc_linux_gamepad_c.h"
#include "x11/cc_x11_window_c.h"
#endif

#include <stdlib.h>

static struct cc_event _event_stack[CC_EVENT_MAX_STACK_SIZE];
static int _event_stack_pointer;

/* Public functions */

int cc_poll_events(void)
{
	if(!cc_poll_window()){
		return 0;
	}

	if(!cc_poll_gamepad()){
		return 0;
	}

	return 1;
}

int cc_push_event(struct cc_event event)
{
	if(_event_stack_pointer == CC_EVENT_MAX_STACK_SIZE - 1){
		cc_set_error("The event stack is full, call \"cc_pop_event\" (maximum \"%d\" items)", CC_EVENT_MAX_STACK_SIZE);
		return 0;
	}
	_event_stack_pointer++;
	_event_stack[_event_stack_pointer] = event;

	return 1;
}

int cc_pop_event(struct cc_event *event)
{
	struct cc_event *stack_event;

	if(_event_stack_pointer == -1){
		event->type = CC_EVENT_SKIP;
		return 0;
	}

	stack_event = _event_stack + _event_stack_pointer;
	event->data = stack_event->data;
	event->type = stack_event->type;

	_event_stack_pointer--;

	return 1;
}

int cc_clear_event_queue(void)
{
	_event_stack_pointer = -1;

	return 1;
}
