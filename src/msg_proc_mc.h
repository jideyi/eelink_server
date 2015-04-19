/*
 * msg_proc_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_PROC_MC_H_
#define SRC_MSG_PROC_MC_H_

int mc_login(short seq, char* msg, short len);
int mc_gps(short seq, char* msg, short len);
int mc_ping(short seq, char* msg, short len);
int mc_alarm(short seq, char* msg, short len);
int mc_status(short seq, char* msg, short len);
int mc_sms(short seq, char* msg, short len);
int mc_msg(short seq, char* msg, short len);
int mc_data(short seq, char* msg, short len);

#endif /* SRC_MSG_PROC_MC_H_ */
