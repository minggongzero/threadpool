#include <pthread.h>
//#include <time.h>
#include "condition.h"

int condition_lock(condition *cond)
{
	return pthread_mutex_lock(&cond->mutex_t);
}

int condition_unlock(condition *cond)
{
	return pthread_mutex_unlock(&cond->mutex_t);
}

int condition_init(condition *cond)
{
	int status;
	if((status = pthread_mutex_init(&cond->mutex_t, NULL)) != 0)
		return status;
	if((status = pthread_cond_init(&cond->cond_t, NULL)) != 0)
		return status;
	return 0;
}

int condition_destroy(condition *cond)
{
	int status;
	if((status = pthread_mutex_destroy(&cond->mutex_t)) != 0)
		return status;
	if((status = pthread_cond_destroy(&cond->cond_t)) != 0)
		return status;
	return 0;
}

int condition_signal(condition *cond)
{
	return pthread_cond_signal(&cond->cond_t);
}

int condition_wait(condition *cond)
{
	return pthread_cond_wait(&cond->cond_t, &cond->mutex_t);
}

int condition_timedwait(condition *cond, const struct timespec *restrict_abstime)
{
	return pthread_cond_timedwait(&cond->cond_t, &cond->mutex_t, restrict_abstime);
}


int condition_broadcast(condition *cond)
{
	return pthread_cond_broadcast(&cond->cond_t);
}
