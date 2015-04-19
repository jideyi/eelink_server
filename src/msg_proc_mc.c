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

int mc_msg_send(const void* msg, size_t len, CB_CTX* ctx)
{
	MSG_SEND pfn = ctx->pSendMsg;

	pfn(ctx->bev, msg, len);

	free(msg);

	return 0;
}

int mc_login(short seq,const char* msg, short len, CB_CTX* ctx)
{
	MC_MSG_LOGIN_REQ* req = msg;

	assert(sizeof(MC_MSG_LOGIN_REQ) < len);


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

	assert(sizeof(MC_MSG_LOGIN_REQ) < len);

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
	return 0;
}

int mc_alarm(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}

int mc_status(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}

int mc_sms(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}

int mc_msg(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}

int mc_data(short seq,const  char* msg, short len, CB_CTX* ctx)
{
	return 0;
}
