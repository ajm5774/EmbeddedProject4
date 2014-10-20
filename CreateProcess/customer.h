/*
 * customer.h
 *
 *  Created on: Oct 20, 2014
 *      Author: ajm5774
 */

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

typedef struct {
	int custNum;
	struct Customer * behind;
} Customer;


#endif /* CUSTOMER_H_ */
