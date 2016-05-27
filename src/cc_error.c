#include "cc_error_c.h"
#include <cc_error.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef CC_DISABLE_THREADS
static struct cc_error _global_err;
#else
#error "TODO, implement thread-safe error message pool"
#endif

/* Private functions */

static struct cc_error *cc_get_error_local(void)
{
#ifdef CC_DISABLE_THREADS
	return &_global_err;
#else
#error "TODO, implement thread-safe error message pool"
#endif
}

/* Public functions */

void cc_set_error(const char *format, ...)
{
	va_list args;
	struct cc_error *err;
	
	err = cc_get_error_local();
	err->error = 1;

	va_start(args, format);
	vsprintf(err->message, format, args);
#ifdef CC_DEBUG
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
#endif
	va_end(args);
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

void cc_clear_error(void)
{
	cc_get_error_local()->error = 0;
}

void cc_out_of_memory_error(void)
{
	struct cc_error *err;

	err = cc_get_error_local();

	err->error = 2;
	strcpy(err->message, "Could not allocate more memory");
#ifdef CC_DEBUG
	fprintf(stderr, "%s\n", err->message);
#endif
}

void cc_no_window_error(void)
{
	struct cc_error *err;

	err = cc_get_error_local();

	err->error = 2;
	strcpy(err->message, "No valid window found");
#ifdef CC_DEBUG
	fprintf(stderr, "%s\n", err->message);
#endif
}

void cc_invalid_parameter_error(const char *param)
{
	struct cc_error *err;

	err = cc_get_error_local();

	err->error = 2;
	sprintf(err->message, "Parameter %s is not valid", param);
#ifdef CC_DEBUG
	fprintf(stderr, "%s\n", err->message);
#endif
}
