/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_DISPLAY_H__
#define __CC_DISPLAY_H__

struct cc_resolution_info {
	int width, height, refresh_rate;
};

struct cc_display_info {
	int x, y, possible_resolutions;
	unsigned short dpi;
	char monitor_name[64];
};

int cc_initialize_display(void);
int cc_destroy_display(void);

int cc_get_display_count(void);
int cc_get_display_info(int display_id, struct cc_display_info *info);

int cc_get_default_resolution_id(int display_id);
int cc_get_resolution_info(int display_id, int resolution_id, struct cc_resolution_info *info);
int cc_set_resolution(int display_id, int resolution_id);

#endif /* __CC_DISPLAY_H__ */
