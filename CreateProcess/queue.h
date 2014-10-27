
#include "customer.h"

//concurrent queue
typedef struct {
        Customer * first;                      /* position of first element */
        Customer * last;                       /* position of last element */
        int count;
} conQueue;

int dequeue(conQueue *q);
void enqueue(conQueue *q, Customer * cust);
void print_queue(conQueue *q);


