#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int max_mouse_move_events;
	struct cc_event event;

	if(!cc_new_window(0)){
		fprintf(stderr, "Couldn't create window:\n\t%s\n", cc_get_error());
		return EXIT_FAILURE;
	}

	cc_set_window_size(200, 200);
	cc_set_window_title("ccore example: events");

	/* Don't fill the whole output stream with mouse move events */
	max_mouse_move_events = 10;
	while(cc_poll_window()){
		event = cc_pop_event();
		switch(event.type){
			case CC_EVENT_QUIT:
				printf("Window exit button pressed\n");
				break;
			case CC_EVENT_RESIZE:
				printf("Window resized to \"%dx%d\"\n", cc_get_window_width(), cc_get_window_height());
				break;
			case CC_EVENT_GAIN_FOCUS:
				printf("Window gained focus\n");
				break;
			case CC_EVENT_LOSE_FOCUS:
				printf("Window lost focus\n");
				break;
			case CC_EVENT_PRESS_MOUSE:
				printf("Mouse button \"%d\" pressed\n", event.data.mouse_button);
				break;
			case CC_EVENT_RELEASE_MOUSE:
				printf("Mouse button \"%d\" released\n", event.data.mouse_button);
				break;
			case CC_EVENT_SCROLL_MOUSE:
				printf("Mouse scroll with delta \"%d\"\n", event.data.mouse_scroll_delta);
				break;
			case CC_EVENT_MOVE_MOUSE:
				if(max_mouse_move_events-- > 0){
					printf("Mouse move to \"%d, %d\"\n", cc_get_mouse_x(), cc_get_mouse_y());
				}
				break;
			case CC_EVENT_PRESS_KEY:
				printf("Key %d pressed\n", event.data.key_code);
				break;
			case CC_EVENT_RELEASE_KEY:
				printf("Key %d released\n", event.data.key_code);
				break;
		}
	}

	cc_free_window();

	return EXIT_SUCCESS;
}
