/*
 * teller.c
 *
 *  Created on: Oct 27, 2014
 *      Author: ajm5774
 */



static int TellerWaitTimes[NUMBER_OF_TELLERS][100] = {0};
static int TellerWaitCounter[NUMBER_OF_TELLERS] = {0};

static pthread_t threads[NUMBER_OF_TELLERS] ;	// where we store the results of the thread creation
static pthread_t *threadIDs[NUMBER_OF_TELLERS] = {&threads[0], &threads[1], &threads[2]};

static void TellerThread( int *threadNumber )
{
	int myThreadNumber = *threadNumber + 1 ;

	while(1)
	{
		Customer * cust = dequeue(&cq);

		if(cust)
		{
			//do some processing
		}
	}
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
