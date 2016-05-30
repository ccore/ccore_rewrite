#include "cc_linux_file_c.h"
#include <cc_file.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <linux/limits.h>

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
