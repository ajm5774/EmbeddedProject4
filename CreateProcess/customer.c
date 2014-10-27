/*
 * customer.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */
#include "queue.h"


Customer * custArray[1000];
static pthread_t *custThreadID;
void StartCustomerThread()
{
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	printf( "Customer Creation thread running at priority %d\n", parameters.sched_priority) ;
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority

	//start customer creation thread
	pthread_create( custThreadID, &threadAttributes, (void *)CustomersThread, 0) ;
}

//Method to create customers
static void CustomersThread(int * arg)
{
	Customer *cust;
	int sleepMins;//in minutes
	int custNum = 0;
	int maxSleepTime = 4;
	int remMinsInDay = 7*60;//7 hours 60 minutes * .1 minutes

	//loop until theres no more time in the day
	while(remMinsInDay > 0)
	{
		//create customer
		cust = malloc( sizeof(Customer) );
		cust->custNum = custNum;
		cust->timeWaiting = 0;
		cust->timeWithTeller = 0;
		cust->behind = NULL;

		//Add customer to the list of all customers (for metrics)
		custArray[custNum++] = cust;

		//Add customer to the queue
		enqueue(&q, cust);

		//wait 1-4 sim minutes before creating anothe customer
		sleepMins = (rand() % maxSleepTime) + 1;
		usleep(sleepMins * simMinute);

		//decrement the number of minutes remaining in the day
		remMinsInDay = remMinsInDay - sleepMins;
	}
}
