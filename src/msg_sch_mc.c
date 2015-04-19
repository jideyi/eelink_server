/*
 * schedule_mc.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <stdio.h>

#include "msg_mc.h"
#include "msg_proc_mc.h"

typedef int (*MSG_PROC)(short seq, char* msg, short length);
typedef struct
{
	char cmd;
	MSG_PROC pfn;
}MC_MSG_PROC;



static MC_MSG_PROC msgProcs[] =
{
		{CMD_LOGIN,	mc_login},
		{CMD_GPS,	mc_gps},
		{CMD_PING,	mc_ping},
		{CMD_ALARM, mc_alarm},
		{CMD_STATUS,mc_status},
		{CMD_SMS, 	mc_sms},
		{CMD_MSG,	mc_msg},
		{CMD_DATA,	mc_data},
};

int handle_mc_msg(char* m, int msgLen, void* ctx)
{
	MC_MSG_HEADER* msg = (MC_MSG_HEADER*)m;

	//check the msg header
	if (msg->header[0] != 0x67 || msg->header[1] != 0x67)
	{
		return -1;
	}

	for (size_t i = 0; i < sizeof(msgProcs) / sizeof(msgProcs[0]); i++)
	{
		if (msgProcs[i].cmd == msg->cmd)
		{
			MSG_PROC pfn = msgProcs[i].pfn;
			if (pfn)
			{
				return pfn(msg->seq, msg->data, msg->length - sizeof(msg->seq));
			}
		}
	}

	return -1;
}
