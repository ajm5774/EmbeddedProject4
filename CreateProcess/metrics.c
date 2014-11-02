/*
 * metrics.c
 *
 *  Created on: Oct 31, 2014
 *      Author: ajm5774
 */
#include "metrics.h"

extern conQueue cq;
extern Customer * custArray[1000];
extern int maxDepth;

int getNumServiced() {
	int serviced = 0;
	while (custArray[serviced++]);

	return serviced-1;
}

long getAverageCustomerWaitTime() {
	int numCustomers = getNumServiced();
	long totalWaitMinutes = 0;
	int i = 0;

	while (i < numCustomers) {
		totalWaitMinutes += custArray[i++]->timeWaiting;
	}

	return totalWaitMinutes / numCustomers;
}

long getAverageServiceTime(){
	int numCustomers = getNumServiced();
	long totalServiceMinutes = 0;
	int i = 0;

	while (i < numCustomers) {
		totalServiceMinutes += custArray[i++]->timeWithTeller;
	}

	return totalServiceMinutes / numCustomers;
}

// TODO
long getAverageTellerWaitTime(){
	int numCustomers = getNumServiced();
	long totalWaitMinutes = 0;
	int i = 0;

	while (i < numCustomers) {
		totalWaitMinutes += custArray[i++]->timeWaiting;
	}

	return totalWaitMinutes / numCustomers;
}

long getMaximumCustomerWaitTime(){
	int numCustomers = getNumServiced();
	long maximumWaitTime = 0;
	int i = 0;

	while (i < numCustomers) {
		if (custArray[i++]->timeWaiting > maximumWaitTime)
			maximumWaitTime = custArray[i]->timeWaiting;
	}

	return maximumWaitTime;
}

// TODO
long getMaximumTellerWaitTime(){
	int numCustomers = getNumServiced();
	long maximumWaitTime = 0;
	int i = 0;

	while (i < numCustomers) {
		if (custArray[i++]->timeWaiting > maximumWaitTime)
			maximumWaitTime = custArray[i++]->timeWaiting;
	}

	return maximumWaitTime;
}

long getMaximumServiceTime(){
	int numCustomers = getNumServiced();
	long maximumServiceTime = 0;
	int i = 0;

	while (i < numCustomers) {
		if (custArray[i++]->timeWaiting > maximumServiceTime)
			maximumServiceTime = custArray[i++]->timeWaiting;
	}

	return maximumServiceTime;
}

int getMaximumLineLength(){
	return maxDepth;
}
