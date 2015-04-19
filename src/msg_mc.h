/*
 * msg_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_MC_H_
#define SRC_MSG_MC_H_

enum PROTOCOL
{
	CMD_LOGIN 	= 0x01,
	CMD_GPS		= 0x02,
	CMD_PING	= 0x03,
	CMD_ALARM	= 0x04,
	CMD_STATUS	= 0x05,
	CMD_SMS		= 0x06,
	CMD_MSG		= 0x80,
	CMD_DATA	= 0x81,
};

typedef struct
{
	char header[2];
	char cmd;
	short length;
	short seq;
	char data[0];
}MC_MSG_HEADER;

//Login message structure
typedef struct
{
	char devID[8];
	char language;
	char locale;
}MC_MSG_LOGIN_REQ;

typedef MC_MSG_HEADER MC_MSG_LOGIN_RSP;


//GPS message structure
typedef struct
{
	short mcc;
	short mnc;
	short lac;
	char ci[3];
}CELL;
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CELL cell;
	char location;
	short status;
	short voltage;
	short rxl;
	short anlog1;
	short anlog2;
}MC_MSG_GPS_REQ;
//GPS message requires no response

//PING message structure
typedef struct
{
	short status;
}PING_REQ;

typedef MC_MSG_HEADER MC_MSG_PING_RSP;


//alarm message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CELL cell;
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

//status message structure
typedef struct
{
	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CELL cell;
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

#endif /* SRC_MSG_MC_H_ */
