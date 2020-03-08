#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

#include "threadpool.h"
#include "queue.h"
#include "err.h"

void* starter(void* data) {
	thread_pool_t* p=data;
	int err;
	while(true) {
		if((err=pthread_mutex_lock(&(p->mutex)))) syserr(err, "mutex lock failed");
		while(isEmptyQueue(p->queue) && !p->destroy)
			if((err=pthread_cond_wait(&(p->task_cond), &(p->mutex)))) syserr(err, "wait failed");
		if(p->destroy) break;
		runnable_t runnable=popQueue(p->queue);
		if((err=pthread_mutex_unlock(&(p->mutex)))) syserr(err, "mutex unlock failed");
		runnable.function(runnable.arg, runnable.argsz);
	}
	if((err=pthread_mutex_unlock(&(p->mutex)))) syserr(err, "mutex unlcok failed");
	return 0;
}

int thread_pool_init(thread_pool_t* pool, size_t pool_size) {
	thread_pool_t* p=pool;
	int err;
	if((err=pthread_cond_init(&(p->task_cond), 0))) syserr(err, "cond unlock failed");
	if((err=pthread_mutex_init(&(p->mutex), 0))) syserr(err, "mutex init failed");
	p->destroy=false;
	p->pool_size=pool_size;
	p->queue=createQueue();
	if(!(p->threads=malloc((p->pool_size)*sizeof(pthread_t))))syserr(err, "malloc failed");
	for(size_t i=0; i<(p->pool_size); ++i)
		if((err=pthread_create(&(p->threads)[i], NULL, starter, p))) syserr(err, "create failed");
	return 0;
}

void thread_pool_destroy(thread_pool_t* pool) {
	thread_pool_t* p=pool;
	int err;
	if((err=pthread_mutex_lock(&(p->mutex)))) syserr(err, "mutex lock failed");
	p->destroy=true;
	if((err=pthread_mutex_unlock(&(p->mutex)))) syserr(err, "mutex unlock failed");
	for(size_t i=0; i<p->pool_size; ++i) {
		if((err=pthread_mutex_lock(&(p->mutex)))) syserr(err, "mutex lock failed");
		if((err=pthread_cond_signal(&(p->task_cond)))) syserr(err, "cond signal failed");
		if((err=pthread_mutex_unlock(&(p->mutex)))) syserr(err, "mutex unlock failed");
	}
	for(size_t i=0; i<p->pool_size; ++i)
		if((err=pthread_join(p->threads[i], NULL))) syserr(err, "join failed");
	free(p->threads);
	p->threads=NULL;
	if((err=pthread_mutex_destroy(&(p->mutex)))) syserr(err, "mutex destroy failed");
	if((err=pthread_cond_destroy(&(p->task_cond)))) syserr(err, "cond destroy failed");
	destroyQueue(p->queue);
}

int defer(thread_pool_t* pool, runnable_t runnable) {
	thread_pool_t* p=pool;
	int err;
	if((err=pthread_mutex_lock(&(p->mutex)))) syserr(err, "mutex lock failed");
	if(p->destroy) syserr(err, "pool is already stopped");
	if(!addQueue(p->queue, runnable)) syserr(err, "add to queue failed");
	if((err=pthread_cond_signal(&(p->task_cond)))) syserr(err, "cond signal failed");
	if((err=pthread_mutex_unlock(&(p->mutex)))) syserr(err, "mutex unlock failed");
	return 0;
}