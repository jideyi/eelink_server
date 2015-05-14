/*
 * msg_app.h
 *
 *  Created on: May 12, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_APP_H_
#define SRC_MSG_APP_H_

enum CMD
{
	CMD_WILD		= 0x0,
	CMD_FENCE_SET	= 0x01,
	CMD_FENCE_DEL,
	CMD_FENCE_GET,
	CMD_TEST_GPS    = 0xfe,
	CMD_TEST_ALARM	= 0xff,

};

//Message definition
typedef struct
{
	short header;
	short cmd;
	unsigned short length;
	unsigned short seq;
	char data[];
}__attribute__((__packed__)) APP_MSG;


typedef struct
{
	short header;
	int timestamp;
	int lat;
	int lng;
	short course;
	char speed;
	char isGPS;
}__attribute__((__packed__)) GPS_MSG;


#endif /* SRC_MSG_APP_H_ */
