/*
 * macro_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MACRO_MC_H_
#define SRC_MACRO_MC_H_

#define MSG_SIGNATURE_LENGTH (2)
#define IMEI_LENGTH (8)
#define MAX_DID_LEN (24)
#define DID_LEN (22)
#define MAX_PWD_LEN (16)

#define PRODUCT_KEY "e08e87504a2a4b5c934e99e3983bc219"
#define MAGIC_NUMBER 0x12345678

typedef struct
{
	short mcc;	//mobile country code
	short mnc;	//mobile network code
	short lac;	//local area code
	char ci[3]; //cell id, why the length is 3, not 2 ???
}CGI;

#endif /* SRC_MACRO_MC_H_ */
