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

// Starts the Teller Server process
static pid_t StartTellerTask()
{
	pid_t tellerTask ;					// process ID of the teller server task that we start
	char *teller_argv[] = {tellerProcessFileName, NULL} ;	// the first argument must be the name of the program we are running
	struct inheritance childInheritance ;			// defines the child's privileges
	childInheritance.flags = 0 ;				// tell the child process to use the same privileges as the parent

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

// returns connection ID
static int OpenConnection( const char *channelName )
{
	int connectionID = name_open( channelName, 0 ) ;

	assert( connectionID != -1 ) ;
	return connectionID ;
}

static void CloseConnection( int connectionID )
{
	name_close( connectionID ) ;
}


static void tellerClientDriver( int connectionID )
{
	int counter ;
	int status ;
	tellerRequest oneRequest ;
	tellerRequest requestResponse ;
	uint64_t startClock ;
	uint64_t stopClock ;

	// this will tell you how long it takes for this particular system clock to wrap around.
	// If you exceed that time then your timing measurement will be wildly inaccurate.
	//printf( "seconds per clock wrap %llu\n", (~(uint64_t)0) /SYSPAGE_ENTRY(qtime) -> cycles_per_sec ) ;

	oneRequest.hdr.type = 0 ;
	oneRequest.hdr.subtype = 0 ;

	startClock = ClockCycles() ;
	// simulate bank simulator
	for ( counter = 0 ; counter < 10000 ; counter++ )
	{
		status = MsgSend( connectionID, &oneRequest, sizeof(oneRequest), &requestResponse, sizeof(requestResponse) ) ;
		assert( status != -1 ) ;
	}
	stopClock = ClockCycles() ;
	printf( "%llu clock cycles at %llu cycles per second\n", stopClock - startClock,SYSPAGE_ENTRY(qtime) -> cycles_per_sec  ) ;
}

int main(int argc, char *argv[])
{
	int connectionID = OpenConnection( serverNamedChannel ) ;
	sem_t *tellerSemaphore = CreateTellerSemaphore() ;	// Create the teller semaphore by name
	pid_t tellerTask = StartTellerTask() ;		// process ID of the teller server task that we start

	printf( "semaphore id: %lx   pid: %d\n", (unsigned long)tellerSemaphore, tellerTask) ;
	
	printf( "Connected to Server\n" ) ;
	tellerClientDriver( connectionID ) ;
	
	printf( "Client exiting\n" ) ;
	CloseConnection( connectionID ) ;
	CloseTellerSemaphore(tellerSemaphore) ;

	return EXIT_SUCCESS;
}
