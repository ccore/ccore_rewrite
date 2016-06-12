/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_INFO_H__
#define __CC_INFO_H__

int cc_get_processor_count(int *count);
int cc_get_available_ram(unsigned long *ram);
int cc_get_total_ram(unsigned long *ram);
int cc_get_max_file_descriptors(int *max);

#endif /* __CC_INFO_H__ */
