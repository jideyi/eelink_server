/*
 * gizwits_rsp.c
 *
 *  Created on: Apr 21, 2015
 *      Author: jk
 */

#include "fsm.h"
#include "log.h"
#include "gizwits_rsp.h"

#define LOG_DEBUG(...) \
	zlog(cat[MOD_GIZWITS_RSP], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
	zlog(cat[MOD_GIZWITS_RSP], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARNNING(...) \
	zlog(cat[MOD_GIZWITS_RSP], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_WARNNING, __VA_ARGS__)

#define LOG_ERROR(...) \
	zlog(cat[MOD_GIZWITS_RSP], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(...) \
	zlog(cat[MOD_GIZWITS_RSP], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__)



int mc_register_rsp(int response_code, const char* msg, CB_CTX* ctx)
{
	LOG_DEBUG("process register response msg");

	if (response_code == 201)
	{
		//TODO: process the msg

		fsm_run(EVT_GOT_DID, ctx);
		return 0;
	}

	return -1;
}

int mc_provision_rsp(int response_code, const char* msg, CB_CTX* ctx)
{
	LOG_DEBUG("process provision response msg");

	if (response_code == 200)
	{
		//TODO: process the msg

		fsm_run(EVT_GOT_M2M, ctx);
		return 0;
	}

	return -1;
}


