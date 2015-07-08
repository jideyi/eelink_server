/*
 * msg_simcon.h
 *
 *  Created on: 2015/6/29
 *      Author: jk
 */

#ifndef SRC_MSG_SIMCOM_H_
#define SRC_MSG_SIMCOM_H_

#pragma pack(push,1)


typedef struct
{
	short signature;
	short cmd;
	unsigned short length;
	unsigned short seq;
	char data[];
}__attribute__((__packed__)) MSG_SIMCOM;


#define MC_MSG_HEADER_LEN (sizeof(MC_MSG_HEADER) - sizeof(short))

#pragma pack(pop)


#endif /* SRC_MSG_SIMCOM_H_ */
