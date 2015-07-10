/*
 * msg_simcon.h
 *
 *  Created on: 2015/6/29
 *      Author: jk
 */

#ifndef SRC_MSG_SIMCOM_H_
#define SRC_MSG_SIMCOM_H_

#include "protocol.h"

MSG_HEADER* alloc_msg(char cmd, size_t length);
MSG_HEADER* alloc_rspMsg(const MSG_HEADER* pMsg);

void free_msg(MC_MSG_HEADER* msg);

#endif /* SRC_MSG_SIMCOM_H_ */
