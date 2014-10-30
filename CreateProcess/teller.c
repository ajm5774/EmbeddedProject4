/*
 * teller.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "teller.h"

static int TellerWaitTimes[NUMBER_OF_TELLERS][100] = {0};
static int TellerWaitCounter[NUMBER_OF_TELLERS] = {0};

static pthread_t threads[NUMBER_OF_TELLERS] ;	// where we store the results of the thread creation
static pthread_t *threadIDs[NUMBER_OF_TELLERS] = {&threads[0], &threads[1], &threads[2]};

extern conQueue cq;

static void TellerThread( int *threadNumber )
{
	int myThreadNumber = *threadNumber + 1 ;
	int maxSleepTimeMS = SIMULATION_MINUTE_MSEC * 6;
	int halfSecondMS = SIMULATION_MINUTE_MSEC / 2;
	int sleepTimeUS;
	Customer *cust;

	do
	{
		cust = dequeue(&cq);

		if(cust)
		{
			// Customer out of line, record the time.
			cust->timeWaiting = getSimTimeMilliMins() - cust->timeStart;
			
			printf("Teller %d handling customer %d.\n", myThreadNumber, cust->id);

			// Each customer requires between 30 seconds and 6 minutes for help
			sleepTimeUS = ((rand() % (maxSleepTimeMS - halfSecondMS)) + halfSecondMS) * 1000;
			usleep(sleepTimeUS);

			// Customer finished. Record times
			cust->timeWithTeller = sleepTimeUS * SIMULATION_MINUTE_MSEC;
			cust->timeEnd = getSimTimeMilliMins();

			printf("Teller %d finished processing customer %d at %d.\n",
								myThreadNumber,
								cust->id,
								cust->timeEnd);

			/*printf("Teller %d finished processing customer %d at %dh %dm.\n",
					myThreadNumber, 
					cust->id, 
					getHour(cust->timeEnd), 
					getMinute(cust->timeEnd));*/
		}
	} while (cust || getSimTimeMins() < MINUTES_PER_DAY);
	printf("Teller %d thread stopped \n", myThreadNumber);
}

void StartTellerThreads()
{
	int loopCounter ;
	pthread_attr_t threadAttributes ;
	int policy ;
	struct sched_param parameters ;

	pthread_attr_init(&threadAttributes) ;		// initialize thread attributes structure -- must do before any other activity on this struct
	pthread_getschedparam(pthread_self(), &policy, &parameters) ;	// get this main thread's scheduler parameters
	printf( "Main Teller Server thread running at priority %d\n", parameters.sched_priority) ;
	parameters.sched_priority-- ;									// lower the priority
	pthread_attr_setschedparam(&threadAttributes, &parameters) ;	// set up the pthread_attr struct with the updated priority

	// now create the threads and pass along its thread number from the loop counter.
	for ( loopCounter = 0 ; loopCounter < NUMBER_OF_TELLERS ; loopCounter++ )
	{
		pthread_create( threadIDs[loopCounter], &threadAttributes, (void *)TellerThread, &loopCounter ) ;
	}
}
