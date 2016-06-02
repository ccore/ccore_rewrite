#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

int current_cursor = 0;

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", cc_get_error());
	exit(EXIT_FAILURE);
}

void cycle_cursors(void)
{
	current_cursor++;
	if(current_cursor > CC_CURSOR_NONE){
		current_cursor = CC_CURSOR_ARROW;
	}

	cc_set_mouse_cursor(current_cursor);
}

int main(int argc, char** argv)
{
	int event_time_divisor;
	struct cc_event event;

	cc_set_error_handler(error_handler);

	cc_new_window(0);
	cc_set_window_size(200, 200);
	cc_set_window_title("ccore example: mouse");

	/* Don't fill the whole output stream with mouse move events */
	event_time_divisor = 0;
	while(cc_poll_window()){
		event = cc_pop_event();
		switch(event.type){
			case CC_EVENT_PRESS_MOUSE:
				printf("Mouse button \"%d\" pressed\n", event.data.mouse_button);
				break;
			case CC_EVENT_RELEASE_MOUSE:
				cycle_cursors();
				printf("Mouse button \"%d\" released\n", event.data.mouse_button);
				break;
			case CC_EVENT_MOVE_MOUSE:
				if(event_time_divisor++ % 50 == 0){
					printf("Mouse move to \"%d, %d\"\n", cc_get_mouse_x(), cc_get_mouse_y());
				}
				break;
		}
	}

	cc_destroy_window();

	return EXIT_SUCCESS;
}
