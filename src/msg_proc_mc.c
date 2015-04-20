/*
 * msg_proc_mc.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <assert.h>
#include <string.h>

#include "msg_proc_mc.h"
#include "msg_mc.h"
#include "object_mc.h"

#include "log.h"

#define LOG_DEBUG(...) \
	zlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
	zlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARNNING(...) \
	zlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_WARNNING, __VA_ARGS__)

#define LOG_ERROR(...) \
	zlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(...) \
	zlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__)

#define LOG_DEBUG_HEX(buf, buf_len) \
	hzlog(cat[MOD_PROC_MC], __FILE__, sizeof(__FILE__)-1, __func__, sizeof(__func__)-1, __LINE__, \
	ZLOG_LEVEL_DEBUG, buf, buf_len)

int mc_msg_send(const void* msg, size_t len, CB_CTX* ctx)
{
	MSG_SEND pfn = ctx->pSendMsg;

	pfn(ctx->bev, msg, len);

	LOG_INFO("send response msg of cmd(%d), leng(%ld - %ld)", get_msg_cmd(msg), len, htons(len));
	LOG_DEBUG_HEX(msg, len);

	free(msg);

	return 0;
}

int mc_login(short seq,const char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_LOGIN_REQ* req = msg;

	assert(sizeof(MC_MSG_LOGIN_REQ) <= len);

	char IMEI[IMEI_LENGTH * 2 + 1];
	for (int i = 0; i < IMEI_LENGTH; i++)
	{
		sprintf(IMEI + i * 2, "%02x", req->IMEI[i]);
	}
	IMEI[IMEI_LENGTH * 2] = 0;

	LOG_DEBUG("mc login: IMEI=%s", IMEI);

	OBJ_MC* obj = ctx->obj;
	if (!obj)
	{
		obj = mc_obj_new();

		memcpy(obj->IMEI, req->IMEI, sizeof(IMEI_LENGTH));
		obj->language = req->language;
		obj->locale = req->locale;

		ctx->obj = obj;
	}

	MC_MSG_LOGIN_RSP *rsp = alloc_msg(CMD_LOGIN, 0, seq);

	mc_msg_send(rsp, sizeof(MC_MSG_LOGIN_RSP), ctx);

	return 0;
}

int mc_gps(short seq __attribute__((unused)),const const  char* msg, short len, CB_CTX* ctx __attribute__((unused)))
{
	MC_MSG_GPS_REQ* req = msg;

	if (!req)
	{
		LOG_ERROR("msg handle empty");
		return -1;
	}

	if (len < sizeof(MC_MSG_LOGIN_REQ))
	{
		LOG_ERROR("message length not enough");
		return -1;
	}

	LOG_DEBUG("GPS: lat(%d), lon(%d), speed(%d), course(%d)", req->lat, req->lon, req->speed, req->course);

	OBJ_MC* obj = ctx->obj;
	if (obj)
	{
		obj->lat = req->lat;
		obj->lon = req->lon;
		obj->speed = req->speed;
		obj->course = req->course;
		obj->cell = req->cell;
	}

	//no response message needed

	return 0;
}

int mc_ping(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_PING_REQ *req = msg;

	MC_MSG_PING_RSP* rsp = alloc_msg(CMD_PING, 0, seq);
	mc_msg_send(rsp, sizeof(MC_MSG_PING_RSP), ctx);

	return 0;
}

int mc_alarm(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_ALARM_REQ* req = msg;

	MC_MSG_ALARM_RSP* rsp = alloc_msg(CMD_ALARM, 0,seq);
	mc_msg_send(rsp, sizeof(MC_MSG_PING_RSP), ctx);

	return 0;
}

int mc_status(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_STATUS_REQ* req = msg;

	MC_MSG_STATUS_RSP* rsp = alloc_msg(CMD_STATUS, 0, seq);
	mc_msg_send(rsp, sizeof(MC_MSG_STATUS_RSP), ctx);

	return 0;
}

int mc_sms(short seq, const  char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_SMS_REQ* req = msg;

	MC_MSG_SMS_RSP* rsp = alloc_msg(CMD_SMS, 0, seq);
	mc_msg_send(rsp, sizeof(MC_MSG_SMS_RSP), ctx);

	return 0;
}

int mc_operator(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_OPERATOR_RSP* req = msg;

	LOG_DEBUG("MC response %s", req->data);

	return 0;
}

int mc_data(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}
