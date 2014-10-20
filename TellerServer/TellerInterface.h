/*
 * TellerSemaphore.h
 *
 *  Created on: Oct 14, 2014
 *      Author: llk
 */

#ifndef TELLERINTERFACE_H_
#define TELLERINTERFACE_H_

#define NUMBER_OF_TELLERS 3

const char tellerSemaphore[] = "TellerSemaphore" ;			// this will show up in /dev/sem/tmp
const char *tellerProcessFileName = "/tmp/TellerServer" ;	// provides an absolute path to the teller server application

#endif /* TELLERINTERFACE_H_ */
