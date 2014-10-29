// Create process demo program with semaphore for TellerServer
//
// L. Kiser Oct. 14, 2014

#include <stdlib.h>
#include <stdio.h>
#include <spawn.h>
#include <fcntl.h>				// defines O_CREAT for creating semaphore
#include <assert.h>
#include <sys/stat.h>			// provides constants used for setting mode of semaphore
#include "queue.h"

conQueue cq = {0, 0, 0};

int main(int argc, char *argv[])
{
	//start teller threads
	StartTellerThreads();
	//start customer creation thread
	StartCustomerThread();
	pthread_exit();
}
