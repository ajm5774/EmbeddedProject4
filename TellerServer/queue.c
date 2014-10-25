
#include "queue.h"

int maxDepth = 0;

void enqueue(queue *q, Customer * cust)
{
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
}

int dequeue(queue *q)
{
	if (q->count == 1)
	{
		q->first = 0;
		q->last = 0;
	}
	else {
		q->first = q->first->behind;
	}
	q->count--;
}

void print_queue(queue *q)
{
	Customer * cust = q->first;

	while (cust->behind) {
		printf("%c \n", cust->custNum);
		cust = cust->behind;
	}
}

