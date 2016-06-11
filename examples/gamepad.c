#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", message);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	int gamepad_count;
	struct cc_event event;

	cc_set_error_handler(error_handler);

	cc_initialize_gamepad();

	cc_get_gamepad_count(&gamepad_count);
	printf("%d gamepads found\n", gamepad_count);

	cc_set_window_size(400, 1);
	cc_new_window(0);
	cc_set_window_title("ccore example: gamepad");

	while(cc_poll_events()){
		while(cc_pop_event(&event)){
			switch(event.type){
				case CC_EVENT_GAMEPAD_CONNECT:
					printf("Gamepad %d connected\n", event.data.gamepad.id);
					break;
				case CC_EVENT_GAMEPAD_DISCONNECT:
					printf("Gamepad %d disconnected\n", event.data.gamepad.id);
					break;
				case CC_EVENT_GAMEPAD_MOVE_AXIS:
					printf("Gamepad %d axis %d moved\n", event.data.gamepad.id, event.data.gamepad.axis);
					break;
				case CC_EVENT_GAMEPAD_PRESS_BUTTON:
					printf("Gamepad %d button %d pressed\n", event.data.gamepad.id, event.data.gamepad.button);
					break;
				case CC_EVENT_GAMEPAD_RELEASE_BUTTON:
					printf("Gamepad %d button %d released\n", event.data.gamepad.id, event.data.gamepad.button);
					break;
			}
		}
	}

	cc_destroy_gamepad();
	cc_destroy_window();

	return EXIT_SUCCESS;
}
