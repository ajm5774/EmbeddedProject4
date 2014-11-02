// Create process demo program with semaphore for TellerServer
//
// L. Kiser Oct. 14, 2014

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "queue.h"
#include "teller.h"
#include "metrics.h"

conQueue cq = {0, 0, 0};
Customer * custArray[1000];

pthread_t tellerThreads[NUMBER_OF_TELLERS];
pthread_t customerThread;


void printStats() {
	printf("----------Statistics---------\n");
	printf("Total customers serviced today: %d\n", getNumServiced());
	printf("Average Customer Wait Time: %d\n", getAverageCustomerWaitTime());
	printf("Average Service Time: %d\n", getAverageServiceTime());
	printf("Average Teller Wait Time: %d\n", getAverageTellerWaitTime());
	printf("Maximum Customer Wait Time: %d\n", getMaximumCustomerWaitTime());
	printf("Maximum Teller Wait Time: %d\n", getMaximumTellerWaitTime());
	printf("Maximum Service Time: %d\n", getMaximumServiceTime());
	printf("Maximum Line Length: %d\n", getMaximumLineLength());
	printf("----------Statistics---------\n");
}

int main(int argc, char *argv[])
{
	int i;
	i = setStartTime();
	i = getSimTimeMins();
	i = getSimTimeMilliMins();

	printf("%d", i);


	//start teller threads
	StartTellerThreads();

	//start customer creation thread
	StartCustomerThread();

	// Wait for threads to finish
	pthread_join(customerThread, 0);
	pthread_join(tellerThreads[0], 0);
	pthread_join(tellerThreads[1], 0);
	pthread_join(tellerThreads[2], 0);

	// Print statistics
	printStats();

	return 0;
}
