/*
 * TellerSemaphore.h
 *
 *  Created on: Oct 14, 2014
 *      Author: llk
 */

#ifndef TELLERINTERFACE_H_
#define TELLERINTERFACE_H_

#define NUMBER_OF_TELLERS 3

const char tellerSemaphore[] = "TellerSemaphore" ;	// this will show up in /dev/sem/tmp
conQueue cq = {0,0,0};					// The queue of customers

#endif /* TELLERINTERFACE_H_ */
