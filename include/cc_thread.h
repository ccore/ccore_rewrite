/*
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the 3-clause BSD license.
 *
 * You should have received a copy of the 3-clause BSD License along with
 * this program. If not, see <http:opensource.org/licenses/>.
 */

#ifndef __CC_THREAD_H__
#define __CC_THREAD_H__

#include "cc_system.h"

#if defined CC_OS_WIN32

#include <windows.h>

typedef HANDLE cc_thread;
typedef CRITICAL_SECTION cc_mutex;

#elif defined CC_OS_LINUX

#include <pthread.h>

typedef pthread_t cc_thread;
typedef pthread_mutex_t cc_mutex;

#endif

int cc_new_thread(void *(function)(void*), void *data, cc_thread *thread);
int cc_destroy_thread(cc_thread *thread);
int cc_join_thread(cc_thread *thread);
int cc_exit_thread(void *data);

int cc_new_mutex(int spin_count, cc_mutex *mutex);
int cc_destroy_mutex(cc_mutex *mutex);
int cc_lock_mutex(cc_mutex *mutex);
int cc_unlock_mutex(cc_mutex *mutex);

#endif /* __CC_THREAD_H__ */
