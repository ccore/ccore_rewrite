#include <cc_thread.h>

#include <cc_error.h>

int cc_new_thread(void *(function)(void*), void *data, cc_thread *thread)
{
	if(pthread_create(thread, NULL, function, data)){
		cc_set_error("Could not create thread");
		return 0;
	}

	return 1;
}

int cc_destroy_thread(cc_thread *thread)
{
	if(pthread_cancel(*thread)){
		cc_set_error("Could not destroy thread");
		return 0;
	}

	return 1;
}

int cc_join_thread(cc_thread *thread)
{
	if(pthread_join(*thread, NULL)){
		cc_set_error("Could not join thread");
		return 0;
	}

	return 1;
}

int cc_exit_thread(void *data)
{
	pthread_exit(data);
}

int cc_new_mutex(int spin_count, cc_mutex *mutex)
{
	if(pthread_mutex_init(mutex, NULL)){
		cc_set_error("Could not create mutex");
		return 0;
	}

	return 1;
}

int cc_destroy_mutex(cc_mutex *mutex)
{
	if(pthread_mutex_destroy(mutex)){
		cc_set_error("Could not destroy mutex");
		return 0;
	}

	return 1;
}

int cc_lock_mutex(cc_mutex *mutex)
{
	if(pthread_mutex_lock(mutex)){
		cc_set_error("Could not lock mutex");
		return 0;
	}

	return 1;
}

int cc_unlock_mutex(cc_mutex *mutex)
{
	if(pthread_mutex_unlock(mutex)){
		cc_set_error("Could not unlock mutex");
		return 0;
	}

	return 1;
}
