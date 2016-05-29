#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ICON_WIDTH 32
#define ICON_HEIGHT 32
#define ICON_PATH "icon.data"

uint32_t *icon;

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", cc_get_error());
	exit(EXIT_FAILURE);
}

void load_icon()
{
	char file_path[256];
	FILE *icon_file;
	size_t icon_size;

	icon_size = ICON_WIDTH * ICON_HEIGHT * sizeof(uint32_t);
	icon = (uint32_t*)malloc(icon_size);

	printf("Reading a %dx%d icon file of %lu bytes\n", ICON_WIDTH, ICON_HEIGHT, icon_size);

	strcpy(file_path, cc_get_dir_data());
	strcat(file_path, ICON_PATH);

	icon_file = fopen(file_path, "rb");
	if(!icon_file){
		fprintf(stderr, "Can't open file \"%s\"\n", file_path);
		exit(EXIT_FAILURE);
	}
	fread(icon, sizeof(uint32_t), icon_size, icon_file);
	fclose(icon_file);
}

int main(int argc, char** argv)
{
	struct cc_event event;

	load_icon();

	cc_set_error_handler(error_handler);

	cc_new_window(CC_WINDOW_NO_RESIZE);

	cc_set_window_title("ccore example: window icon");

	cc_set_window_icon(ICON_WIDTH, ICON_HEIGHT, icon);
	free(icon);

	while(cc_poll_window()){
		event = cc_pop_event();
		switch(event.type){
			case CC_EVENT_LOSE_FOCUS:
				cc_blink_window();
				break;
		}
	}

	cc_free_window();

	return EXIT_SUCCESS;
}
