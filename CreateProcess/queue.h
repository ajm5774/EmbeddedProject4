/*
 * queue.h
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "customer.h"

//concurrent queue
typedef struct conQueue {
        Customer * first;                      /* position of first element */
        Customer * last;                       /* position of last element */
        int count;
} conQueue;

// Insert a customer at the back of the queue.
void enqueue(conQueue *q, Customer * cust);

// Remove the head from the queue.
Customer* dequeue(conQueue *q);

// Print the specified queue.
void print_queue(conQueue *q);

#endif /* QUEUE_H_ */
