/*
 * msg_proc_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_PROC_MC_H_
#define SRC_MSG_PROC_MC_H_

#include "cb_ctx_mc.h"

int mc_login(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_gps(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_ping(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_alarm(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_status(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_sms(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_msg(short seq, const char* msg, short len, CB_CTX* ctx);
int mc_data(short seq, const char* msg, short len, CB_CTX* ctx);

#endif /* SRC_MSG_PROC_MC_H_ */
