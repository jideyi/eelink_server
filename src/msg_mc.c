/*
 * msg_mc.c
 *
 *  Created on: Apr 20, 2015
 *      Author: jk
 */
#include <arpa/inet.h>

#include "msg_mc.h"

#define local_htons(x) ((unsigned short int)((((unsigned short int)(x) & 0x00ff)<<8)| \
                                            (((unsigned short int)(x) & 0xff00)>>8)))

#define local_htonl(x) ((unsigned long int)((((unsigned long int)(x) & 0x000000ff)<<24)| \
                                            (((unsigned long int)(x) & 0x0000ff00)<<8)| \
                                            (((unsigned long int)(x) & 0x00ff0000)>>8)| \
                                            (((unsigned long int)(x) & 0xff000000)>>24)))

inline void fill_msg_header(MC_MSG_HEADER* msg)
{
	msg->header[0] = 0x67;
	msg->header[1] = 0x67;
}

inline char get_msg_cmd(const MC_MSG_HEADER* msg)
{
	return msg->cmd;
}

inline const short get_msg_seq(const MC_MSG_HEADER* msg)
{
	return msg->seq;
}

inline void set_msg_cmd(MC_MSG_HEADER* msg, char cmd)
{
	msg->cmd = cmd;
}


inline void set_msg_len(MC_MSG_HEADER* msg, short length)
{
	msg->length = htons(length);
	//msg->length = local_htons(length);
}

inline void set_msg_seq(MC_MSG_HEADER* msg, short seq)
{
	msg->seq = seq;
}

MC_MSG_HEADER* alloc_rspMsg(const MC_MSG_HEADER* pMsg)
{
	size_t msgLen = 0;
	switch (pMsg->cmd)
	{
	case CMD_LOGIN:
		msgLen = sizeof(MC_MSG_LOGIN_RSP);
		break;
	case CMD_PING:
		msgLen = sizeof(MC_MSG_PING_RSP);
		break;
	default:
		return NULL;
	}

	MC_MSG_HEADER* msg = malloc(msgLen);

	fill_msg_header(msg);
	set_msg_cmd(msg, pMsg->cmd);
	set_msg_len(msg, msgLen - sizeof(MC_MSG_HEADER) + sizeof(msg->seq));
	set_msg_seq(msg, get_msg_seq(pMsg));

	return msg;
}


void free_msg(MC_MSG_HEADER* msg)
{
	free(msg);
}
