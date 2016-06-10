#include <cc_time.h>

#include <unistd.h>
#include <time.h>

int cc_get_time_nano_seconds(unsigned long *nano_seconds)
{
	struct timespec time;

	clock_gettime(CLOCK_MONOTONIC, &time);

	*nano_seconds = (unsigned long)time.tv_nsec + (unsigned long)(time.tv_sec * (unsigned long)1.0e9);

	return 1;
}

int cc_sleep_micro_seconds(unsigned long micro_seconds)
{
	usleep(micro_seconds);

	return 1;
}
