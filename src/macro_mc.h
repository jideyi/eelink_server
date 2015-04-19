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

typedef struct
{
	short mcc;	//mobile country code
	short mnc;	//mobile network code
	short lac;	//local area code
	char ci[3]; //cell id, why the length is 3, not 2 ???
}CGI;

#endif /* SRC_MACRO_MC_H_ */
