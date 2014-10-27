// TellerServer demo of creating threads and connecting to semaphore
//
// L. Kiser Oct. 15, 2014

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>

#include "TellerInterface.h"
#include "queue.h"

static sem_t *mySemaphore;

static queue q = {0,0,0};
static Customer * custArray[1000];
static int simMinute = 100;//100ms = 1 minute

static int TellerWaitTimes[NUMBER_OF_TELLERS][100] = {0};
static int TellerWaitCounter[NUMBER_OF_TELLERS] = {0};

static pthread_t threads[NUMBER_OF_TELLERS] ;	// where we store the results of the thread creation
static pthread_t *threadIDs[NUMBER_OF_TELLERS] = {
		&threads[0], &threads[1], &threads[2]
};	// sets up an array of pointers to where we store the thread creation results

static sem_t *OpenTellerSemaphore()
{
	return sem_open(tellerSemaphore) ;
}

static void CloseTellerSemaphore( sem_t *ptrSemaphore)
{
	// Release the semaphore resources
	int status = sem_close( ptrSemaphore ) ;
	
	assert( status == 0 ) ;
}

static void TellerThread( int *threadNumber )
{
	int myThreadNumber = *threadNumber + 1 ;

	printf("Thread %d started\n", myThreadNumber) ;
	// add mutex here to wait for something to do in a loop that continues until there are no more customers.

	//loop
	//wait for customer in the queue
	//process customer request
	//send to semaphore

	// Also could use ThreadDestroy() from the main to kill all threads when out of customers for the day.
	sleep(2 + *threadNumber) ;
	printf("Thread %d exiting\n", myThreadNumber) ;
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

static void StartThreads()
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

	//start customer creation thread
	pthread_create( threadIDs[++loopCounter], &threadAttributes, (void *)CustomersThread, &loopCounter) ;
}

// opens a named channel to be used for sending messages to this server.
// Despite the notes in the QNX documentation you do not actually need the gns service to be running
// to use named channels locally.
static name_attach_t *CreateNamedChannel()
{
	return name_attach( NULL, serverNamedChannel, 0 ) ;
}

// close the named channel just to clean up before exiting the server
static void CloseNamedChannel( name_attach_t *ptrChannel )
{
	assert( name_detach( ptrChannel, 0 ) == 0 ) ;
}

// derived from demo code provided by QNX on the name_attach() page in the programmer's library
static int ProcessPulse( tellerRequest *ptrMessage )
{
	int allDone = 0 ;

    switch (ptrMessage->hdr.code) {
    case _PULSE_CODE_DISCONNECT:
        /*
         * A client disconnected all its connections (called
         * name_close() for each name_open() of our name) or
         * terminated
         */
        ConnectDetach(ptrMessage->hdr.scoid);
        allDone = 1 ;
        break;
    case _PULSE_CODE_UNBLOCK:
        /*
         * REPLY blocked client wants to unblock (was hit by
         * a signal or timed out).  It's up to you if you
         * reply now or later.
         */
        break;
    default:
        /*
         * A pulse sent by one of your processes or a
         * _PULSE_CODE_COIDDEATH or _PULSE_CODE_THREADDEATH
         * from the kernel?
         */
        break;
    }
    return allDone ;		// if true then got the disconnect
}

static void ProcessOneRequest( int receiveID, tellerRequest *ptrMessage )
{
    /* name_open() sends a connect message, must EOK this */
    if ( ptrMessage->hdr.type == _IO_CONNECT )
    	MsgReply( receiveID, EOK, NULL, 0 );
    else
    	MsgReply( receiveID, EOK, ptrMessage, sizeof(tellerRequest) ) ;
}

static void ProcessRequests( name_attach_t *ptrChannel )
{
	// add code here to receive requests including teller service request and
	// the exit request.
	// If you get an exit request then you may want to close all of the threads from here.

	int receivedID ;
	int allDone = 0 ;
	tellerRequest receivedMessage ;

	for ( ; ; )
	{
		receivedID = MsgReceive( ptrChannel->chid, &receivedMessage, sizeof(receivedMessage), NULL ) ;
		if ( receivedID == 0 )
			allDone = ProcessPulse( &receivedMessage ) ;
		else
			ProcessOneRequest( receivedID, &receivedMessage ) ;

		if ( allDone )
			break ;
	}
	
	sleep(10) ;	// allow enough demo time for the teller threads to exit.
	printf("Teller Server Exiting\n") ;
}

int main(int argc, char *argv[]) {
	mySemaphore = AccessTellerSemaphore() ;	// Create the teller semaphore by name
	name_attach_t *ptrNamedChannel = CreateNamedChannel() ;
	
	if ( ptrNamedChannel )
		printf("Teller Server is started\n");
	else
		printf("Named Channel was not created\n") ;

	StartThreads() ;
	ProcessRequests( ptrNamedChannel ) ;
	
	CloseNamedChannel( ptrNamedChannel ) ;
	CloseTellerSemaphore(mySemaphore);
	return EXIT_SUCCESS;
}
