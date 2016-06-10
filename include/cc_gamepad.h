/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_GAMEPAD_H__
#define __CC_GAMEPAD_H__

int cc_initialize_gamepad(void);
int cc_destroy_gamepad(void);

int cc_get_gamepad_count(int *count);


#endif /* __CC_GAMEPAD_H__ */