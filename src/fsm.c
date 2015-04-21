/*
 * fsm.c
 *
 *  Created on: 2015��4��20��
 *      Author: jk
 */

#include "fsm.h"
#include "log.h"
#include "cb_ctx_mc.h"
#include "gizwits_req.h"

#define LOG_DEBUG(...) \
	zlog(cat[MOD_FSM], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
	zlog(cat[MOD_FSM], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARNNING(...) \
	zlog(cat[MOD_FSM], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_WARNNING, __VA_ARGS__)

#define LOG_ERROR(...) \
	zlog(cat[MOD_FSM], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(...) \
	zlog(cat[MOD_FSM], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__)



#define CUR_STATUS(ctx)	((CB_CTX*)ctx)->cur_status

typedef int ACTION(void* ctx);

int action_req_did(void* ctx);


ACTION* state_transitions[STS_MAX][EVT_MAX] =
{
						/* EVT_SIGN_IN		EVT_GOT_DID 	EVT_OTA		EVT_FIRMWARE	EVT_LOGIN	EVT_PING	EVT_MQTT */
/* STS_INITIAL 		*/	{action_req_did,	},
/* STS_WAIT_DID		*/	{},
/* STS_WAIT_M2MINFO	*/	{},
/* STS_WAIT_OTA		*/	{},
/* STS_WAIT_FIRMWARE*/	{},
/* STS_LOGINING		*/	{},
/* STS_RUNNING		*/	{},
};

int fsm_run(EVENT event, void* ctx)
{
	LOG_DEBUG("run FSM State(%d), Event(%d)", CUR_STATUS(ctx), event);
	ACTION* action = state_transitions[CUR_STATUS(ctx)][event];
	if (action)
	{
		return action(ctx);
	}
}

int start_fsm(void* ctx)
{
	return fsm_run(EVT_SIGN_IN, ctx);
}

int action_req_did(void* ctx)
{
	mc_register(ctx);

	CUR_STATUS(ctx) = STS_WAIT_DID;

	return 0;
}

int action_provision(void* ctx)
{

	CUR_STATUS(ctx) = STS_WAIT_M2MINFO;

	return 0;
}

int action_ota(void* ctx)
{

	CUR_STATUS(ctx) = STS_WAIT_OTA;

	return 0;
}

int action_firmware(void* ctx)
{

	CUR_STATUS(ctx) = STS_WAIT_FIRMWARE;

	return 0;
}

