/*
 * msg_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_MC_H_
#define SRC_MSG_MC_H_

#include <stdlib.h>

#include "macro_mc.h"


enum PROTOCOL
{
	CMD_LOGIN 	= 0x01,
	CMD_GPS		= 0x02,
	CMD_PING	= 0x03,
	CMD_ALARM	= 0x04,
	CMD_STATUS	= 0x05,
	CMD_SMS		= 0x06,
	CMD_OPERAT	= 0x80,
	CMD_DATA	= 0x81,
};

#pragma pack(push,1)
typedef struct
{
	char header[MSG_SIGNATURE_LENGTH];
	char cmd;
	short length;
	short seq;
	char data[];
}__attribute__((__packed__)) MC_MSG_HEADER;
#pragma pack(pop)

//Login message structure
typedef struct
{
	unsigned char IMEI[IMEI_LENGTH];
	char language;
	char locale;
}MC_MSG_LOGIN_REQ;

typedef MC_MSG_HEADER MC_MSG_LOGIN_RSP;


//GPS message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI cell;
	char location;
	short status;
	short voltage;
	short rxl;
	short analog1;
	short analog2;
}MC_MSG_GPS_REQ;
//GPS message requires no response

//PING message structure
typedef struct
{
	short status;
}MC_MSG_PING_REQ;

typedef MC_MSG_HEADER MC_MSG_PING_RSP;


//alarm message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI cell;
	char location;
	char type;
}MC_MSG_ALARM_REQ;
enum ALARM_TYPE
{
	POWER_FAILURE 	= 0x01,
	SOS				= 0x02,
	BATTERY_LOW		= 0x03,
	VIBRATION		= 0x04,
	MOVE			= 0x05,

	SPEED_LOW		= 0x81,
	SPEED_HIGH		= 0x82,
	FENCE_IN		= 0x83,
	FENCE_OUT		= 0x84,
};

typedef struct
{
	MC_MSG_HEADER header;
	char sms[];
}__attribute__((__packed__)) MC_MSG_ALARM_RSP;

//status message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI cell;
	char location;
	char type;
	short status;
}MC_MSG_STATUS_REQ;

enum STATUS_TYPE
{
	ACC_ON	= 0x01,
	ACC_OFF	= 0x02,
	DIGTAL	= 0x03,
};

typedef MC_MSG_HEADER MC_MSG_STATUS_RSP;

//sms message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI cell;
	char location;

	char telphone[21];
	char sms[];
}__attribute__((__packed__)) MC_MSG_SMS_REQ;

typedef struct
{
	MC_MSG_HEADER header;
	char telphone[21];
	char sms[];
}__attribute__((__packed__)) MC_MSG_SMS_RSP;


//the following message is from Server to MC
typedef struct
{
	char type;
	int token;
	char data[];
}__attribute__((__packed__)) MC_MSG_OPERATOR_REQ;

typedef MC_MSG_OPERATOR_REQ MC_MSG_OPERATOR_RSP;


char get_msg_cmd(const MC_MSG_HEADER* msg);

MC_MSG_HEADER* alloc_msg(char cmd, short length, short seq);
void free_msg(MC_MSG_HEADER* msg);

#endif /* SRC_MSG_MC_H_ */
