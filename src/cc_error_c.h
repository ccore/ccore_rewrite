#ifndef __CC_ERROR_C_H__
#define __CC_ERROR_C_H__

#define CC_ERROR_MAX_STRLEN  128

struct cc_error {
	int error;

	char message[CC_ERROR_MAX_STRLEN];
};

#endif
