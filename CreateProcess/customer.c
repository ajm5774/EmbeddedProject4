/*
 * customer.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "customer.h"
#include "TellerInterface.h"

Customer * custArray[1000];
static pthread_t custThreadID;

extern conQueue cq;

//Method to create customers
static void CustomersThread(int * arg)
{
	Customer *cust;
	int id = 0;
	int sleepMinsMS;//in msec
	int maxSleepTime = 4 * SIMULATION_MINUTE_MSEC;
	int minSleepTime = SIMULATION_MINUTE_MSEC;

	//loop until theres no more time in the day
	while(getSimTime() < MINUTES_PER_DAY)
	{
		//create customer
		cust = malloc( sizeof(Customer) );
		cust->id = id;
		cust->timeStart = getSimTimeMs();
		cust->timeWaiting = 0;
		cust->timeWithTeller = 0;
		cust->timeEnd = 0;
		cust->behind = 0;

		printf("Customer %d arrived at %d.\n", cust->id, cust->timeStart);
		//printf("Customer %d arrived at %dh %dm.\n", cust->id, getHour(cust->timeStart), getMinute(cust->timeStart));

		//Add customer to the list of all customers (for metrics)
		custArray[id++] = cust;

		//Add customer to the queue
		enqueue(&cq, cust);

		//wait 1-4 sim minutes before creating another customer
		sleepMinsMS = (rand() % (maxSleepTime - minSleepTime)) + minSleepTime;
		usleep(sleepMinsMS);
	}
	printf("Customer creation thread stopped \n");
}

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
	pthread_create( &custThreadID, &threadAttributes, (void *)CustomersThread, 0) ;
}


