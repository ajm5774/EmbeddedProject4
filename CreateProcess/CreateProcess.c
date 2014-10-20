// Create process demo program with semaphore for TellerServer
//
// L. Kiser Oct. 14, 2014

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <spawn.h>
#include <fcntl.h>				// defines O_CREAT for creating semaphore
#include <assert.h>
#include <sys/stat.h>			// provides constants used for setting mode of semaphore
#include "TellerInterface.h"	// application specific header file
#include "queue.h"

queue q = {0,0,0};
Customer * custArray[1000];

// Starts the Teller Server process
static pid_t StartTellerTask()
{
	pid_t tellerTask ;		// process ID of the teller server task that we start
	char *teller_argv[] = {tellerProcessFileName, NULL} ;	// the first argument must be the name of the program we are running
	struct inheritance childInheritance ;					// defines the child's privileges
	childInheritance.flags = 0 ;							// tell the child process to use the same privileges as the parent

	// Start the teller server. Since the second parameter is 0 the third parameter for the file descriptor
	// mapping can be NULL. The fifth parameter is NULL meaning that the child inherits the parents environment variables.
	tellerTask = spawnp(tellerProcessFileName, 0, NULL, &childInheritance, teller_argv, NULL) ;

	return tellerTask ;
}

static sem_t *CreateTellerSemaphore()
{
	mode_t myMode = S_IRWXU | S_IRWXG | S_IRWXO;	// gives owner, group, and world Read, Write, and eXecute permissions

	return sem_open(tellerSemaphore, O_CREAT, myMode, 0 ) ;
}

static void CloseTellerSemaphore( sem_t *ptrSemaphore)
{
	int status = sem_close( ptrSemaphore ) ;
	if (status == 0)
	{
		status = sem_unlink( tellerSemaphore ) ;
	}
	// terminate with error if status is not 0 indicating failure closing or unlinking the semaphore
	assert( status == 0 ) ;
}

static void StartCustCreateThread()
{
	//pthread_create( threadIDs[loopCounter], &threadAttributes, (void *)CreateCustomers) ;
}

static void CreateCustomers()
{
	int custNum = 0;
	while(1)
	{
		Customer cust = {custNum, 0};
		custArray[custNum++] = cust;
		enqueue(q, );
	}
}

int main(int argc, char *argv[])
{
	sem_t *tellerSemaphore = CreateTellerSemaphore() ;	// Create the teller semaphore by name
	pid_t tellerTask = StartTellerTask() ;		// process ID of the teller server task that we start
	printf( "semaphore id: %lx   pid: %d\n", (unsigned long)tellerSemaphore, tellerTask) ;
	CloseTellerSemaphore(tellerSemaphore) ;
	return EXIT_SUCCESS;
}
