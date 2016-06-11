#ifndef __CC_LINUX_GAMEPAD_C_H__
#define __CC_LINUX_GAMEPAD_C_H__

struct cc_gamepad {
	int id, fd, connected, axis_count, button_count;
	int *axis;
	char *button;
	char name[80];
};

int cc_poll_gamepad(void);

#endif /* __CC_LINUX_GAMEPAD_C_H__ */
