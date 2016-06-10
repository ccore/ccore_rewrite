/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_FILE_H__
#define __CC_FILE_H__

#include <time.h>

const char *cc_get_dir_user(void);
const char *cc_get_dir_data(void);
const char *cc_get_dir_temp(void);

int cc_get_file_size(const char *file, unsigned long *file_size);
int cc_get_file_last_modified(const char *file, time_t *time_stamp);
int cc_get_file_last_accessed(const char *file, time_t *time_stamp);

#endif /* __CC_FILE_H__ */
