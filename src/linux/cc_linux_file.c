#include "cc_linux_file_c.h"
#include <cc_file.h>

#include <cc_error.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>

struct cc_dir {
	DIR *dir;
};
struct cc_dir_entry {
	struct dirent *entry;
};

#ifndef CC_DATA_LOCATION
static char *_data_dir = NULL;
#endif

const char *cc_get_dir_user(void)
{
	return CC_USER_LOCATION;
}

const char *cc_get_dir_temp(void)
{
	return CC_TEMP_LOCATION;
}

const char *cc_get_dir_data(void)
{
#ifndef CC_DATA_LOCATION
	size_t len;

	if(_data_dir == NULL){
		_data_dir = (char*)malloc(PATH_MAX);
		len = readlink("/proc/self/exe", _data_dir, PATH_MAX);
		if(len > 0){
			_data_dir[len] = '\0';
			_data_dir = dirname(_data_dir);
			strcat(_data_dir, "/");
		}

	}
	
	return _data_dir;
#else
	return CC_DATA_LOCATION;
#endif
}

int cc_get_file_size(const char *file, unsigned long *file_size)
{
	struct stat file_info;

	if(stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*file_size = file_info.st_size;

	return 1;
}

int cc_get_file_last_modified(const char *file, time_t *time_stamp)
{
	struct stat file_info;

	if(stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*time_stamp = file_info.st_mtime;
	
	return 1;
}

int cc_get_file_last_accessed(const char *file, time_t *time_stamp)
{
	struct stat file_info;

	if(stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*time_stamp = file_info.st_atime;

	return 1;
}

int cc_open_dir(const char *path, struct cc_dir *dir)
{
	dir->dir = opendir(path);

	return 1;
}

int cc_close_dir(struct cc_dir *dir)
{
	return closedir(dir->dir);
}

int cc_read_dir(struct cc_dir *dir, struct cc_dir_entry *dir_entry)
{
	dir_entry->entry = readdir(dir->dir);
	if(dir_entry->entry == NULL){
		cc_set_error("Could not read directory");
		return 0;
	}

	dir_entry->name = dir_entry->entry->d_name;

	return 1;
}

int cc_rewind_dir(struct cc_dir *dir)
{
	rewinddir(dir->dir);

	return 1;
}

int cc_get_dir_name(const struct cc_dir_entry *entry, char *name)
{
	name = entry->d_name;

	return 1;
}
