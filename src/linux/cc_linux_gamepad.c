#include "cc_linux_gamepad_c.h"
#include <cc_gamepad.h>

#include <cc_error.h>
#include <cc_event.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <linux/joystick.h>

static struct cc_gamepad *_gamepads;
static int _count = 0;
static int _notify_fd, _watch;

/* Private functions */

static int cc_open_gamepad_descriptor(const char *path, int *fd)
{
	char dir_name[30];

	sprintf(dir_name, "/dev/input/%s", path);
	*fd = open(dir_name, O_RDONLY | O_NONBLOCK, 0);

	return fd >= 0;
}

static int cc_create_gamepad(const char *path, int index)
{
	int fd;
	char empty_buffer[64];
	struct cc_gamepad *gamepad;

	if(!cc_open_gamepad_descriptor(path, &fd)){
		cc_set_error("Could not open gamepad input /dev/input/%s", path);
		return 0;
	}

	/* Clear gamepad buffer */
	while(read(fd, empty_buffer, 64) > 0);

	_gamepads = (struct cc_gamepad*)realloc(_gamepads, (index + 1) * sizeof(struct cc_gamepad));
	if(!_gamepads){
		cc_out_of_memory_error();
		return 0;
	}

	gamepad = _gamepads + index;
	gamepad->connected = 1;
	gamepad->fd = fd;
	gamepad->id = atoi(path);

	ioctl(fd, JSIOCGAXES, &gamepad->axis_count);
	ioctl(fd, JSIOCGBUTTONS, &gamepad->button_count);
	ioctl(fd, JSIOCGNAME(80), gamepad->name);

	gamepad->button = (char*)calloc(gamepad->button_count, sizeof(char));
	gamepad->axis = (int*)calloc(gamepad->axis_count, sizeof(int));

	return 1;
}

static int cc_is_inotify_readable(void)
{
	fd_set set;
	struct timeval time_out;

	FD_ZERO(&set);
	FD_SET(_notify_fd, &set);

	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

	return select(_notify_fd + 1, &set, NULL, NULL, &time_out) > 0 && FD_ISSET(_notify_fd, &set);
}

/* Local functions */

int cc_poll_gamepad(void)
{
	int i, id, gamepad_id;
	struct inotify_event notify_event;
	struct js_event joystick_event;
	struct cc_gamepad *gamepad;
	struct cc_event event;

	while(cc_is_inotify_readable()){
		if(read(_notify_fd, &notify_event, sizeof(struct inotify_event) + 16) < 0){
			continue;
		}

		if(notify_event.name[0] != 'j' && notify_event.name[1] != 's'){
			continue;
		}

		/* Find matching gamepad */
		gamepad_id = -1;
		id = atoi(notify_event.name + 2);
		for(i = 0; i < _count; i++){
			if(_gamepads[i].id == id){
				gamepad_id = i;
				break;
			}
		}

		if(gamepad_id >= 0){
			gamepad = _gamepads + gamepad_id;
			if(notify_event.mask & IN_DELETE){
				gamepad->connected = 0;
				close(gamepad->fd);

				event.type = CC_EVENT_GAMEPAD_DISCONNECT;
				event.data.gamepad.id = gamepad_id;
				cc_push_event(event);
			}
			if(notify_event.mask & IN_ATTRIB && !gamepad->connected){
				if(cc_open_gamepad_descriptor(notify_event.name, &gamepad->fd)){
					gamepad->connected = 1;

					event.type = CC_EVENT_GAMEPAD_CONNECT;
					event.data.gamepad.id = gamepad_id;
					cc_push_event(event);
				}
			}
		}else if(notify_event.mask & IN_ATTRIB){
			if(!cc_create_gamepad(notify_event.name, _count)){
				return 0;
			}
			event.data.gamepad.id = _count;
			_count++;

			event.type = CC_EVENT_GAMEPAD_CONNECT;
			cc_push_event(event);
		}
	}

	for(i = 0; i < _count; i++){
		gamepad = _gamepads + i;
		if(!gamepad->connected){
			continue;
		}

		if(read(gamepad->fd, &joystick_event, sizeof(struct js_event)) < 0){
			continue;
		}

		event.data.gamepad.id = i;
		switch(joystick_event.type & ~JS_EVENT_INIT){
			case JS_EVENT_AXIS:
				if(gamepad->axis[joystick_event.number] == joystick_event.value){
					break;
				}

				gamepad->axis[joystick_event.number] = joystick_event.value;
				event.data.gamepad.axis = joystick_event.number;
				event.type = CC_EVENT_GAMEPAD_MOVE_AXIS;
				cc_push_event(event);
				break;
			case JS_EVENT_BUTTON:
				if(gamepad->button[joystick_event.number] == (joystick_event.value != 0)){
					break;
				}

				gamepad->button[joystick_event.number] = joystick_event.value != 0;
				event.data.gamepad.button = joystick_event.number;
				if(gamepad->button[joystick_event.number] == 0){
					event.type = CC_EVENT_GAMEPAD_PRESS_BUTTON;
				}else{
					event.type = CC_EVENT_GAMEPAD_RELEASE_BUTTON;
				}
				cc_push_event(event);
				break;
		}
	}

	return 1;
}

/* Public functions */

int cc_initialize_gamepad(void)
{
	DIR *dir;
	struct dirent *dir_entry;

	if(_count > 0){
		return 1;
	}

	_notify_fd = inotify_init();
	if(_notify_fd < 0){
		cc_set_error("Could not initialize inotify");
		return 0;
	}

	_watch = inotify_add_watch(_notify_fd, "/dev/input", IN_DELETE | IN_ATTRIB);
	if(_watch < 0){
		cc_set_error("Could not access /dev/input");
		close(_notify_fd);
		return 0;
	}

	_gamepads = (struct cc_gamepad*)malloc(sizeof(struct cc_gamepad));
	if(!_gamepads){
		cc_out_of_memory_error();
		close(_notify_fd);
		close(_watch);
		return 0;
	}

	dir = opendir("/dev/input");
	while((dir_entry = readdir(dir))){
		if(dir_entry->d_name[0] == 'j' && dir_entry->d_name[1] == 's'){
			if(!cc_create_gamepad(dir_entry->d_name, _count)){
				closedir(dir);
				close(_notify_fd);
				close(_watch);
				free(_gamepads);
				return 0;
			}
			_count++;
		}
	}
	closedir(dir);

	return 1;
}

int cc_destroy_gamepad(void)
{
	int i;

	if(_count > 0){
		for(i = 0; i < _count; i++){
			close(_gamepads[i].fd);
			free(_gamepads[i].axis);
			free(_gamepads[i].button);
		}
		free(_gamepads);
		_count = 0;
	}

	close(_notify_fd);
	close(_watch);
}

/* Getters */

int cc_get_gamepad_count(int *count)
{
	*count = _count;

	return 1;
}

int cc_get_gamepad_info(int id, struct cc_gamepad_info *info)
{
	
	return 1;
}
