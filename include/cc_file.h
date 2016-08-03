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

struct cc_dir;
struct cc_dir_entry;

const char *cc_get_dir_user(void);
const char *cc_get_dir_data(void);
const char *cc_get_dir_temp(void);

int cc_get_file_size(const char *file, unsigned long *file_size);
int cc_get_file_last_modified(const char *file, time_t *time_stamp);
int cc_get_file_last_accessed(const char *file, time_t *time_stamp);

int cc_open_dir(const char *path, struct cc_dir *dir);
int cc_close_dir(struct cc_dir *dir);
int cc_rewind_dir(struct cc_dir *dir);
int cc_seek_dir(struct cc_dir *dir, long distance);
int cc_tell_dir(const struct cc_dir *dir, long *position);
int cc_read_dir(struct cc_dir *dir, struct cc_dir_entry *dir_entry);
int cc_get_dir_name(const struct cc_dir *dir, char *name); /* Maximum length of the name is 256 characters */

#endif /* __CC_FILE_H__ */
