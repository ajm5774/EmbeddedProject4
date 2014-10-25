/*
 * customer.h
 *
 *  Created on: Oct 20, 2014
 *      Author: ajm5774
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

typedef struct Customer {
	int id;
	struct Customer* behind;
	int timeWaiting;
	int timeWithTeller;
} Customer;


#endif /* CUSTOMER_H_ */
