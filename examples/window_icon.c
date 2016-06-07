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
	size_t icon_size, ints_read;

	icon_size = ICON_WIDTH * ICON_HEIGHT;
	icon = (uint32_t*)malloc(icon_size * sizeof(uint32_t));
	if(!icon){
		cc_out_of_memory_error();
	}

	strcpy(file_path, cc_get_dir_data());
	strcat(file_path, ICON_PATH);

	icon_file = fopen(file_path, "rb");
	if(!icon_file){
		cc_set_error("Can't open file %s", file_path);
	}
	ints_read = fread(icon, sizeof(uint32_t), icon_size, icon_file);
	if(ints_read != icon_size){
		if(feof(icon_file)){
			cc_set_error("Failed reading %s: unexpected end of file at %d bytes", file_path, ints_read);
		}else if(ferror(icon_file)){
			perror("Error reading file");
			exit(EXIT_FAILURE);
		}
	}
	fclose(icon_file);

	printf("Read a %dx%d icon file of %lu bytes\n", ICON_WIDTH, ICON_HEIGHT, ints_read * sizeof(uint32_t));
}

int main(int argc, char** argv)
{
	struct cc_event event;

	cc_set_error_handler(error_handler);

	cc_set_window_size(300, 200);
	cc_new_window(CC_WINDOW_NO_RESIZE);

	load_icon();
	cc_set_window_icon(ICON_WIDTH, ICON_HEIGHT, icon);
	free(icon);

	cc_set_window_title("ccore example: window icon");

	while(cc_poll_window()){
		while(cc_pop_event(&event)){
			if(event.type == CC_EVENT_LOSE_FOCUS){
				cc_blink_window();
			}
		}
	}

	cc_destroy_window();

	return EXIT_SUCCESS;
}
