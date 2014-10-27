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
const char *serverNamedChannel = "TellerServerChannel" ;	// NOTE -- you do not need to run gns on the target system. If it
															// is not running then it will create /dev/name/local/demoServerChannel
															// on the target system.
															// Every process or thread that needs access to this channel
															// can open via name_open once the server has created it via name_attach.
/* We specify the header as being at least a pulse */
typedef struct _pulse msg_header_t;

// Message sent from client to the teller server
typedef struct _tellerRequest
{
	msg_header_t hdr ;		// standard QNX header
	// client to server data goes here
	// if wanted you can use the same message going both ways with some fields
	// filled in by the client and the results in other fields filled in by the server.
	// TODO Add Customer Class
	
	
} tellerRequest ;

#endif /* TELLERINTERFACE_H_ */
