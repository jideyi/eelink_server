/*
 * msg_proc_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_PROC_MC_H_
#define SRC_MSG_PROC_MC_H_

#include "cb_ctx_mc.h"

int mc_login(const void* msg, CB_CTX* ctx);
int mc_gps(const void* msg, CB_CTX* ctx);
int mc_ping(const void* msg, CB_CTX* ctx);
int mc_alarm(const void* msg, CB_CTX* ctx);
int mc_status(const void* msg, CB_CTX* ctx);
int mc_sms(const void* msg, CB_CTX* ctx);
int mc_operator(const void* msg, CB_CTX* ctx);
int mc_data(const void* msg, CB_CTX* ctx);

#endif /* SRC_MSG_PROC_MC_H_ */
