#ifndef CONDITION_H_INCLUDE
#define CONDITION_H_INCLUDE

//#include <sys/time.h>
#include <pthread.h>

//条件变量，封装posix
struct condition{
	pthread_cond_t cond_t;
	pthread_mutex_t mutex_t;
};

int condition_lock(condition *cond);
int condition_unlock(condition *cond);
int condition_init(condition *cond);
int condition_destroy(condition *cond);
int condition_signal(condition *cond);
int condition_wait(condition *cond);
int condition_timedwait(condition *cond, const struct timespec *restrict_abstime);
int condition_broadcast(condition *cond);

#endif    //CONDITION_H_INCLUDE
