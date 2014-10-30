/*
 * TellerSemaphore.h
 *
 *  Created on: Oct 14, 2014
 *      Author: llk
 */

#ifndef TELLERINTERFACE_H_
#define TELLERINTERFACE_H_

#define NUMBER_OF_TELLERS 3
#include "queue.h"

#define SIMULATION_MINUTE_USEC (100000)	//100000us = 1 minute
#define SIMULATION_MINUTE_MSEC (100)	//100ms = 1 minute
#define MINUTES_PER_DAY (420)		// 60 minutes * 7 hours
#define START_HOUR (9)

long getSimTimeMins();
long getSimTimeMilliMins();
long setStartTime();

int getHour(int time);
int getMinute(int time);

#endif /* TELLERINTERFACE_H_ */
