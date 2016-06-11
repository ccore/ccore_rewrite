/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_GAMEPAD_H__
#define __CC_GAMEPAD_H__

enum cc_gamepad_state {
	CC_GAMEPAD_STATE_CONNECTED,
	CC_GAMEPAD_STATE_DISCONNECTED
};

struct cc_gamepad_info {
	int axis_count, *axis_values, button_count, *button_states;
	enum cc_gamepad_state state;
};

int cc_initialize_gamepad(void);
int cc_destroy_gamepad(void);

int cc_get_gamepad_count(int *count);
int cc_get_gamepad_info(int id, struct cc_gamepad_info *info);


#endif /* __CC_GAMEPAD_H__ */
