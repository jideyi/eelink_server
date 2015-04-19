/*
 * msg_proc_mc.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <assert.h>

#include "msg_mc.h"

int mc_msg_send(MC_MSG_HEADER* pMsg)
{
	return 0;
}

int mc_login(short seq, char* m, short len)
{
	MC_MSG_LOGIN_REQ* msg = m;

	assert(sizeof(MC_MSG_LOGIN_REQ) == len);

	MC_MSG_LOGIN_RSP rsp;
	rsp.header[0] = 0x67;
	rsp.header[1] = 0x67;
	rsp.cmd = CMD_LOGIN;
	rsp.length = sizeof(seq);

	mc_msg_send(&rsp);

	return 0;
}
int mc_gps(short seq, char* msg, short len)
{
	return 0;
}

int mc_ping(short seq, char* m, short len)
{
	return 0;
}

int mc_alarm(short seq, char* m, short len)
{
	return 0;
}

int mc_status(short seq, char* m, short len)
{
	return 0;
}

int mc_sms(short seq, char* m, short len)
{
	return 0;
}

int mc_msg(short seq, char* m, short len)
{
	return 0;
}

int mc_data(short seq, char* m, short len)
{
	return 0;
}
