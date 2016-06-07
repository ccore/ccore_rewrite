/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_TIME_H__
#define __CC_TIME_H__

#include "cc_system.h"

unsigned long cc_get_time_nano_seconds(void);
void cc_sleep_micro_seconds(unsigned long micro_seconds);

#endif /* __CC_TIME_H__ */
