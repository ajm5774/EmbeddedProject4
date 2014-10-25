
#include "customer.h"

typedef struct {
        Customer * first;                      /* position of first element */
        Customer * last;                       /* position of last element */
        int count;
} queue;

int dequeue(queue *q);
void enqueue(queue *q, Customer * cust);
void print_queue(queue *q);


