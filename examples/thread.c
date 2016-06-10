#include <ccore.h>

#include <stdio.h>
#include <stdlib.h>

void error_handler(const char *message)
{
	fprintf(stderr, "Error: \"%s\"\n", cc_get_error());
	exit(EXIT_FAILURE);
}

void *thread_function(void *arg)
{
	printf("Thread id: %d\n", *(int*)arg);

	cc_exit_thread(NULL);

	return NULL;
}

int main(int argc, char** argv)
{
	int i, integers[100];
	cc_thread threads[100];

	cc_set_error_handler(error_handler);

	for(i = 0; i < 100; i++){
		integers[i] = i;
		cc_new_thread(thread_function, integers + i, threads + i);
	}

	for(i = 0; i < 100; i++){
		cc_join_thread(threads + i);
	}

	return EXIT_SUCCESS;
}
