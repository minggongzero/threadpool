#ifndef THREADPOOL_H_INCLUDE
#define THREADPOOL_H_INCLUDE

#include "condition.h"

struct task
{
	void* (*run)(void*);
	void* arg;
	struct task* next;
};

struct threadpool
{
	condition cond;
	task *first;  //task queue
	task *last;
	int threadsize;  //max size of threadpool
	int count;  //how many threads in this time
	int idle;  //not busy thread num
	int quit;  //destroy smybol
};

int threadpool_init(threadpool *pool, int threadsize);
int threadpool_add_task(threadpool *pool, void*(*run)(void*), void *arg);
int threadpool_destroy(threadpool *pool);

#endif //THREADPOOL_H_INCLUDE
