#include <cc_time.h>

#include <unistd.h>
#include <time.h>

unsigned long cc_get_time_nano_seconds(void)
{
	struct timespec time;

	clock_gettime(CLOCK_MONOTONIC, &time);

	return (unsigned long)time.tv_nsec + (unsigned long)(time.tv_sec * (unsigned long)1.0e9);
}

void cc_sleep_micro_seconds(unsigned long micro_seconds)
{
	usleep(micro_seconds);
}
