#include "cc_event_c.h"
#include <cc_event.h>

#include <cc_error.h>

#include <stdlib.h>

#ifdef CC_DISABLE_THREADS
static struct cc_event _event_stack[CC_EVENT_MAX_STACK_SIZE];
static int _event_stack_pointer;
#else
#error "TODO, implement thread-safe event pool"
#endif

/* Private functions */

/* Public functions */

int cc_push_event(struct cc_event event)
{
#ifdef CC_DISABLE_THREADS
	if(_event_stack_pointer == CC_EVENT_MAX_STACK_SIZE - 1){
		cc_set_error("The event stack is full, call \"cc_pop_event\" (maximum \"%d\" items)", CC_EVENT_MAX_STACK_SIZE);
		return 0;
	}
	_event_stack_pointer++;
	_event_stack[_event_stack_pointer] = event;

	return 1;
#else
	return 0;
#endif
}

struct cc_event cc_pop_event(void)
{
	struct cc_event event;

#ifdef CC_DISABLE_THREADS
	if(_event_stack_pointer == -1){
		event.type = CC_EVENT_SKIP;
		return event;
	}

	event = _event_stack[_event_stack_pointer];
	_event_stack_pointer--;

	return event;
#else
	return NULL;
#endif
}

void cc_clear_event_queue(void)
{
#ifdef CC_DISABLE_THREADS
	_event_stack_pointer = -1;
#else
	return 0;
#endif
}
