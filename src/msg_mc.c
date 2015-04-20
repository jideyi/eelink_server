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

inline void set_msg_cmd(MC_MSG_HEADER* msg, char cmd)
{
	msg->cmd = cmd;
}

inline char get_msg_cmd(MC_MSG_HEADER* msg)
{
	return msg->cmd;
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

MC_MSG_HEADER* alloc_msg(char cmd, short length, short seq)
{
	MC_MSG_HEADER* msg = malloc(sizeof(MC_MSG_HEADER) + length);
	fill_msg_header(msg);
	set_msg_cmd(msg, cmd);
	set_msg_len(msg, sizeof(msg->seq) + length);
	set_msg_seq(msg,seq);

	return msg;
}

void free_msg(MC_MSG_HEADER* msg)
{
	free(msg);
}
