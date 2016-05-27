#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_PATH "icon.data"

#define EXIT_ON_ERROR(x) \
	if(!(x)){ \
		fprintf(stderr, "Error on line %d:\n\t%s: %s\n", __LINE__, #x, cc_get_error());\
		return EXIT_FAILURE; \
	}

int main(int argc, char** argv)
{
	uint32_t *icon;
	char file_path[256];
	FILE *icon_file;
	size_t icon_size;
	struct cc_event event;

	EXIT_ON_ERROR(cc_new_window(CC_WINDOW_NO_RESIZE));

	icon_size = ICON_WIDTH * ICON_HEIGHT * sizeof(uint32_t);
	icon = (uint32_t*)malloc(icon_size);

	strcpy(file_path, cc_get_data_dir());
	strcat(file_path, ICON_PATH);
	icon_file = fopen(file_path, "rb");
	if(!icon_file){
		fprintf(stderr, "Can't open file \"%s\"\n", file_path);
		return EXIT_FAILURE;
	}
	fread(icon, icon_size, 1, icon_file);
	fclose(icon_file);

	EXIT_ON_ERROR(cc_set_window_icon(ICON_WIDTH, ICON_HEIGHT, icon));
	free(icon);

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
