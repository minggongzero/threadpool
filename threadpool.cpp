#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include "threadpool.h"

void* thread_routine(void *arg)
{
	printf("thread 0x%x is starting...\n", (int)pthread_self());
	threadpool *pool = (threadpool*)arg;
	while(1)
	{
		int timedout = 0;
		condition_lock(&pool->cond);
		pool->idle++;
		//if task queue is empty, wait
		while(pool->first == NULL && !pool->quit)
		{
			printf("0x%x is waitting...\n", (int)pthread_self());
			struct timespec abstime;
			clock_gettime(CLOCK_REALTIME, &abstime);
			abstime.tv_sec += 2;
			int status = condition_timedwait(&pool->cond, &abstime);
			if(status == ETIMEDOUT)
			{
				printf("thread 0x%x wait time out...\n", (int)pthread_self());
				timedout = 1;
				break;
			}
		}
		pool->idle--;
		//if there is a task
		if(pool->first != NULL)
		{
			struct task *t = pool->first;
			pool->first = pool->first->next;
			condition_unlock(&pool->cond);
			t->run(t->arg);
			free(t);
			//condition_lock(&pool->cond);
			continue;
		}
		//if there is a destroy broadcast
		else if(pool->quit && (pool->first == NULL))
		{
			printf("thread 0x%x is exiting...\n", (int)pthread_self());
			pool->count--;
			//if there is no thread in threadpool
			//send a signal to wake up destory-wait
			if(!pool->count)
				condition_signal(&pool->cond);
			condition_unlock(&pool->cond);
			break;
		}

		if(timedout && (pool->first == NULL))
		{
			printf("thread 0x%x is exiting\n", (int)pthread_self());
                        pool->count--;
                        if(!pool->count)
                                condition_signal(&pool->cond);
                        condition_unlock(&pool->cond);
                        break;
		}
		condition_unlock(&pool->cond);
	}
	return NULL;
}

int threadpool_init(threadpool *pool, int threadsize)
{
	condition_init(&pool->cond);
	pool->first = pool->last = NULL;
	pool->threadsize = threadsize;
	pool->count = 0;
	pool->idle = 0;
	pool->quit = 0;	
}

int threadpool_add_task(threadpool *pool, void* (*run)(void*), void *arg)
{
	struct task *newtask = (struct task*)malloc(sizeof(struct task));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;
	condition_lock(&pool->cond);
	if(pool->first != NULL) 
	{
		pool->last->next = newtask;
		pool->last = newtask;
	}
	else pool->first = pool->last = newtask;
	//if threadpool has waitting thread
	//waking up a waitting threads
	if(pool->idle > 0)
	{
		condition_signal(&pool->cond);
	}
	//else if has not waitting threads, and threadpool is not full
	//than create a new thread for newtask
	else if(pool->count < pool->threadsize)
	{
		pthread_t t;
		pthread_create(&t, NULL, thread_routine, pool);
		pool->count++;
	}
	condition_unlock(&pool->cond);
}

int threadpool_destroy(threadpool *pool)
{
	if(pool->quit)
		return 0;
	condition_lock(&pool->cond);
	pool->quit = 1;
	if(pool->count > 0)
	{
		if(pool->idle > 0)
			condition_broadcast(&pool->cond);
		while(pool->count > 0)
			condition_wait(&pool->cond);
	}
	condition_unlock(&pool->cond);
	condition_destroy(&pool->cond);
	return 0;
}

