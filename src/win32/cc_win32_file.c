#include "cc_win32_file_c.h"
#include <cc_file.h>

#include <cc_error.h>

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>

#ifndef CC_DATA_LOCATION
static char *_data_dir = NULL;
#endif

#ifndef CC_TEMP_LOCATION
static char *_temp_dir = NULL;
#endif

void cc_win32_path_to_unix(char *path)
{
	/* TODO harden */
	int i, path_length;

	path_length = strlen(path);
	for(i = 0; i < path_length; i++){
		if(path[i] == '\\'){
			path[i] = '/';
		}
	}
}

const char *cc_get_dir_user(void)
{	
	return CC_USER_LOCATION;
}

const char *cc_get_dir_temp(void)
{
#ifndef CC_TEMP_LOCATION
	int path_length;

	if(!_temp_dir){
		_temp_dir = (char*)malloc(MAX_PATH);
		path_length = GetTempPath(MAX_PATH, _temp_dir);
		_temp_dir = (char*)realloc(_temp_dir, path_length + 1);

		cc_win32_path_to_unix(_data_dir);
	}

	return _temp_dir;
#else
	return CC_TEMP_LOCATION;
#endif
}

const char *cc_get_dir_data(void)
{
#ifndef CC_DATA_LOCATION
	HMODULE module_handle;
	int path_length;

	if(!_data_dir){
		_data_dir = (char*)malloc(MAX_PATH);
		path_length = GetModuleFileName(module_handle, _data_dir, MAX_PATH);
		_data_dir = (char*)realloc(_data_dir, path_length + 1);

		cc_win32_path_to_unix(_data_dir);
	}

	return _data_dir;
#else
	return CC_DATA_LOCATION;
#endif
}

int cc_get_file_size(const char *file, unsigned long *file_size)
{
	struct _stat file_info;

	if(_stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*file_size = file_info.st_size;

	return 1;
}

int cc_get_file_last_modified(const char *file, time_t *time_stamp)
{
	struct _stat file_info;

	if(_stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*time_stamp = file_info.st_mtime;
	
	return 1;
}

int cc_get_file_last_accessed(const char *file, time_t *time_stamp)
{
	struct _stat file_info;

	if(_stat(file, &file_info) != 0){
		cc_set_error("Could not get file information for file %s", file);
		return 0;
	}

	*time_stamp = file_info.st_atime;

	return 1;
}
