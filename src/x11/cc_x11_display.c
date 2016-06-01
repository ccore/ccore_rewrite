#include "cc_x11_display_c.h"
#include <cc_display.h>

#include <cc_error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "cc_x11_window_c.h"

static struct cc_display *_displays;
static int _amount = 0;

/* Private functions */

static int cc_populate_resolution(struct cc_display *display, RRMode mode, XRRModeInfo mode_info)
{
	unsigned int v_total;
	struct cc_resolution *resolution;

	v_total = mode_info.vTotal;
	if(mode_info.modeFlags & RR_DoubleScan){
		v_total <<= 1;
	}
	if(mode_info.modeFlags & RR_Interlace){
		v_total >>= 1;
	}

	display->resolution_amount++;
	display->resolutions = (struct cc_resolution*)realloc(display->resolutions, display->resolution_amount * sizeof(struct cc_resolution));
	if(!display->resolutions){
		cc_out_of_memory_error();
		return 0;
	}
	resolution = display->resolutions + display->resolution_amount - 1;

	resolution->mode = mode;
	resolution->refresh_rate = mode_info.dotClock / (mode_info.hTotal * v_total);
	resolution->width = mode_info.width;
	resolution->height = mode_info.height;

	return 1;
}

static int cc_populate_display_data(Display *x_display, XRRScreenResources *resources, int output_index)
{
	int i, j;
	struct cc_display *display;
	XRROutputInfo *output_info;
	XRRCrtcInfo *crtc_info;

	output_info = XRRGetOutputInfo(x_display, resources, resources->outputs[output_index]);
	if(output_info->connection){
		return 0;
	}

	_amount++;
	_displays = (struct cc_display*)realloc(_displays, _amount * sizeof(struct cc_display));
	if(!_displays){
		cc_out_of_memory_error();
		return 0;
	}
	display = _displays + _amount - 1;

	display->resolution_amount = 0;
	display->resolutions = (struct cc_resolution*)malloc(sizeof(struct cc_resolution));

	for(i = 0; i < output_info->nmode; i++){
		for(j = 0; j < resources->nmode; j++){
			if(output_info->modes[i] != resources->modes[j].id){
				continue;
			}

			cc_populate_resolution(display, output_info->modes[i], resources->modes[j]);
			break;
		}
	}

	if(display->resolution_amount == 0){
		cc_set_error("Could not find resolutions for display \"%s\"", output_info->name);
		return 0;
	}

	display->monitor_name = (char*)malloc(output_info->nameLen + 1);
	if(!display->monitor_name){
		cc_out_of_memory_error();
		return 0;
	}
	strcpy(display->monitor_name, output_info->name);

	display->x = -1;
	display->y = -1;
	for(i = 0; i < resources->ncrtc; i++){
		if(resources->crtcs[i] != output_info->crtc){
			continue;
		}
		crtc_info = XRRGetCrtcInfo(x_display, resources, resources->crtcs[i]);
		if(crtc_info->mode == None){
			XRRFreeCrtcInfo(crtc_info);
			continue;
		}

		display->x = crtc_info->x;
		display->y = crtc_info->y;
		display->old_mode = crtc_info->mode;

		XRRFreeCrtcInfo(crtc_info);
		break;
	}

	display->screen = output_index;
	display->output = resources->outputs[output_index];
	display->dpi = ((double)DisplayWidth(x_display, 0) * 25.4) / (double)DisplayWidthMM(x_display, 0);
	
	XRRFreeOutputInfo(output_info);

	return 1;
}

/* Public functions */

int cc_initialize_display(void)
{
	DIR *dir;
	struct dirent *dir_entry;
	char display_name[256];
	int i;
	Display *display;
	Window root;
	XRRScreenResources *resources;

	if(_amount > 0){
		return 1;
	}

	_displays = (struct cc_display*)malloc(sizeof(struct cc_display));
	if(!_displays){
		cc_out_of_memory_error();
		return 0;
	}

	dir = opendir("/tmp/.X11-unix");
	if(!dir){
		cc_set_error("Could not open X11 directory to find displays");
		return 0;
	}

	while((dir_entry = readdir(dir))){
		if(dir_entry->d_name[0] != 'X'){
			continue;
		}

		sprintf(display_name, ":%s", dir_entry->d_name + 1);

		display = XOpenDisplay(display_name);
		if(!display){
			cc_set_error("Can't open display \"%s\"", display_name);
			continue;
		}

		root = RootWindow(display, 0);
		resources = XRRGetScreenResources(display, root);
		if(resources->noutput <= 0){
			XCloseDisplay(display);
			continue;
		}

		for(i = 0; i < resources->noutput; i++){
			cc_populate_display_data(display, resources, i);
		}
		XRRFreeScreenResources(resources);

		XCloseDisplay(display);
	}

	closedir(dir);

	return 1;
}

int cc_destroy_display(void)
{
	int i;
	struct cc_display *display;

	/* TODO reset resolutions */

	for(i = 0; i < _amount; i++){
		display = _displays + i;
		free(display->resolutions);
		free(display->monitor_name);
	}

	free(_displays);
	_amount = 0;

	return 0;
}

/* Getters */

int cc_get_display_count(void)
{
	return _amount;
}

int cc_get_display_info(int display_id, struct cc_display_info *info)
{
	struct cc_display *display;

	if(display_id > _amount || display_id < 0){
		cc_invalid_parameter_error("display_id");
		return 0;
	}
	display = _displays + display_id;
	info->x = display->x;
	info->y = display->y;
	info->possible_resolutions = display->resolution_amount;
	info->dpi = display->dpi;
	strncpy(info->monitor_name, display->monitor_name, 64);

	return 1;
}

int cc_get_default_resolution_id(int display_id)
{
	return 0;
}

int cc_get_resolution_info(int display_id, int resolution_id, struct cc_resolution_info *info)
{
	struct cc_display *display;
	struct cc_resolution *resolution;

	if(display_id > _amount || display_id < 0){
		cc_invalid_parameter_error("display_id");
		return 0;
	}
	display = _displays + display_id;

	if(resolution_id > display->resolution_amount || resolution_id < 0){
		cc_invalid_parameter_error("resolution_id");
		return 0;
	}
	resolution = display->resolutions + resolution_id;

	info->width = resolution->width;
	info->height = resolution->height;
	info->refresh_rate = resolution->refresh_rate;

	return 1;
}
