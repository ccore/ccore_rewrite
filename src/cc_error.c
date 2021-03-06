#include "cc_error_c.h"
#include <cc_error.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static struct cc_error _global_err;
static void (*_err_handler)(const char*) = NULL;

/* Private functions */

static struct cc_error *cc_get_error_local(void)
{
	return &_global_err;
}

static void cc_set_error_values_v(int type, const char *format, va_list args)
{
	struct cc_error *err;
	
	err = cc_get_error_local();
	err->error = type;

	vsprintf(err->message, format, args);
#ifdef CC_DEBUG
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
#endif

	if(_err_handler != NULL){
		_err_handler(err->message);
	}
}

static int cc_set_error_values(int type, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	cc_set_error_values_v(type, format, args);
	va_end(args);

	return 1;
}

/* Public functions */

int cc_set_error(const char *format, ...)
{
	va_list args;
	
	va_start(args, format);
	cc_set_error_values_v(1, format, args);
	va_end(args);

	return 1;
}

const char *cc_get_error(void)
{
	struct cc_error *err;

	err = cc_get_error_local();

	if(err->error == 0){
		return "No error is currently set";
	}

	return err->message;
}

int cc_set_error_handler(void (*handler)(const char*))
{
	_err_handler = handler;

	return 1;
}

int cc_clear_error(void)
{
	cc_get_error_local()->error = 0;

	return 1;
}

int cc_out_of_memory_error(void)
{
	return cc_set_error_values(2, "Could not allocate more memory");
}

int cc_no_window_error(void)
{
	return cc_set_error_values(2, "No valid window is found, make sure \"cc_new_window\" is called before the current function");
}

int cc_invalid_parameter_error(const char *param)
{
	return cc_set_error_values(2, "Parameter(s) \"%s\" is not valid", param);
}
