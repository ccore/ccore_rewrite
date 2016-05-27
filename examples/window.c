#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

#define EXIT_ON_ERROR(x) \
	if(x != 1){ \
		fprintf(stderr, "Error on line %d:\n\t%s: %s\n", __LINE__, #x, cc_get_error());\
		return EXIT_FAILURE; \
	}

int main(int argc, char** argv)
{
	struct cc_event event;

	EXIT_ON_ERROR(cc_new_window(0));

	while(cc_poll_window()){
		event = cc_pop_event();
		switch(event.type){
			case CC_EVENT_LOSE_FOCUS:
				EXIT_ON_ERROR(cc_blink_window());
				break;
		}
	}

	EXIT_ON_ERROR(cc_free_window());

	return EXIT_SUCCESS;
}
