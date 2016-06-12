#include <cc_info.h>

#include <cc_error.h>

#include <unistd.h>
#include <sys/sysinfo.h>

int cc_get_processor_count(int *count)
{
	*count = sysconf(_SC_NPROCESSORS_CONF);

	return 1;
}

int cc_get_available_ram(unsigned long *ram)
{
	struct sysinfo info;

	if(sysinfo(&info)){
		cc_set_error("Could not access Linux system information");
	}
	*ram = info.freeram * info.mem_unit;

	return 1;
}

int cc_get_total_ram(unsigned long *ram)
{
	struct sysinfo info;

	if(sysinfo(&info)){
		cc_set_error("Could not access Linux system information");
	}
	*ram = info.totalram * info.mem_unit;

	return 1;
}

int cc_get_max_file_descriptors(int *max)
{
	*max = sysconf(_SC_OPEN_MAX);

	return 1;
}
