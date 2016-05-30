#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	struct cc_event event;

	if(!cc_new_window(0)){
		fprintf(stderr, "Couldn't create window:\n\t%s\n", cc_get_error());
		return EXIT_FAILURE;
	}

	while(cc_poll_window()){
		event = cc_pop_event();
		switch(event.type){
			case CC_EVENT_LOSE_FOCUS:
				cc_blink_window();
				break;
		}
	}

	cc_destroy_window();

	return EXIT_SUCCESS;
}
