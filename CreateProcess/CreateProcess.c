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

static int simMinute = 100;//100ms = 1 minute

int main(int argc, char *argv[])
{
	//make teller semaphore
	mySemaphore = CreateTellerSemaphore();
	//start teller threads
	StartTellerThreads();
	//start customer creation thread
	StartCustomerThread();
}
