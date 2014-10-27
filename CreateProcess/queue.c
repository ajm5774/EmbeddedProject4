#include <pthread.h>
#include "queue.h"

int maxDepth = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void enqueue(conQueue *q, Customer * cust)
{
	pthread_mutex_lock(&mutex);
	if (q->count == 0)
	{
		q->first = cust;
		q->last = cust;
	}
	else {
		q->last->behind = cust;
		q->last = cust;
	}
	q->count++;

	if(q->count > maxDepth)
		maxDepth = q->count;
	pthread_mutex_unlock(&mutex);
}

Customer * dequeue(conQueue *q)
{
	Customer * cust = 0;
	pthread_mutex_lock(&mutex);
	if(q->count == 0)
	{
		pthread_mutex_unlock(&mutex);
		return cust;
	}
	else if (q->count == 1)
	{
		cust = q->first;
		q->first = 0;
		q->last = 0;
	}
	else {
		cust = q->first;
		q->first = q->first->behind;
	}
	q->count--;
	pthread_mutex_unlock(&mutex);
	return cust;
}

void print_queue(conQueue *q)
{
	pthread_mutex_lock(&mutex);
	Customer * cust = q->first;

	while (cust->behind) {
		printf("%c \n", cust->custNum);
		cust = cust->behind;
	}
	pthread_mutex_unlock(&mutex);
}

