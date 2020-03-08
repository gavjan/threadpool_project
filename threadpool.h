#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stddef.h>
#include <stdbool.h>
#include "queue.h"
#include <pthread.h>

typedef struct thread_pool {
	size_t pool_size;
	pthread_mutex_t mutex;
	pthread_cond_t task_cond;
	pthread_t* threads;
	Queue* queue;
	bool destroy;
} thread_pool_t;

int thread_pool_init(thread_pool_t* pool, size_t pool_size);

void thread_pool_destroy(thread_pool_t* p);

int defer(thread_pool_t* p, runnable_t runnable);

#endif
