#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", message);
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	int processor_count, max_file_descriptors;
	unsigned long ram;

	cc_set_error_handler(error_handler);

	printf("===INFO===\n");
	cc_get_processor_count(&processor_count);
	printf("Processors:           %d\n", processor_count);
	
	cc_get_available_ram(&ram);
	printf("RAM available:        %lu\n", ram);

	cc_get_total_ram(&ram);
	printf("RAM total:            %lu\n", ram);

	cc_get_max_file_descriptors(&max_file_descriptors);
	printf("Max file descriptors: %d\n", max_file_descriptors);

	putchar('\n');

	return EXIT_SUCCESS;
}
