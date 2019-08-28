#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"

void* mytask(void *arg)
{
	printf("thread 0x%x is working on task %d\n", (int)pthread_self(), *(int*)arg);
	sleep(1);
	return NULL;
}

int main()
{
	threadpool pool;
	threadpool_init(&pool, 3);
	int i;
	for(i=0; i<10; ++i)
	{
		int *p = (int*)malloc(sizeof(int));
		*p = i;
		threadpool_add_task(&pool, mytask, p);
	}
//	sleep(10);
	threadpool_destroy(&pool);
	return 0;
}
