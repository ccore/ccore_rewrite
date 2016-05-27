/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_ERROR_H__
#define __CC_ERROR_H__

void cc_set_error(const char *format, ...);
const char *cc_get_error(void);
void cc_clear_error(void);

/* Common errors */
void cc_out_of_memory_error(void);
void cc_invalid_parameter_error(const char *param);

#endif /* __CC_ERROR_H__ */
