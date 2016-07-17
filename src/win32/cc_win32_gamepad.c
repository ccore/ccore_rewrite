#include "cc_win32_gamepad_c.h"
#include <cc_gamepad.h>

#include <cc_error.h>
#include <cc_event.h>

/* Local functions */

int cc_poll_gamepad(void)
{
	return 1;
}

/* Public functions */

int cc_initialize_gamepad(void)
{
	return 1;
}

int cc_destroy_gamepad(void)
{
	return 1;
}

/* Getters */

int cc_get_gamepad_count(int *count)
{
	return 1;
}

int cc_get_gamepad_info(int id, struct cc_gamepad_info *info)
{
	return 1;
}
