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
#include "leancloud_req.h"
#include "object_mc.h"
#include "yeelink_req.h"
#include "mqtt.h"
#include "msg_proc_app.h"
#include "cJSON.h"
#include "yunba_push.h"
#include "log.h"


int mc_msg_send(void* msg, size_t len, CB_CTX* ctx)
{
	msg_send pfn = ctx->pSendMsg;

	pfn(ctx->bev, msg, len);

	LOG_DEBUG("send response msg of cmd(%d), length(%ld)", get_msg_cmd(msg), len);
	LOG_HEX(msg, len);

	free(msg);

	return 0;
}

int mc_login(const void* msg, CB_CTX* ctx)
{
	const MC_MSG_LOGIN_REQ* req = msg;


	OBJ_MC* obj = ctx->obj;
	if (!obj)
	{
		LOG_DEBUG("mc IMEI(%s) login", get_IMEI_STRING(req->IMEI));

		obj = mc_get(req->IMEI);

		if (!obj)
		{
			LOG_DEBUG("the first time of IMEI(%s)'s login", get_IMEI_STRING(req->IMEI));

			obj = mc_obj_new();
            if(NULL == obj)
            {
                LOG_ERROR("malloc IMEI(%s) obj failed", get_IMEI_STRING(req->IMEI));
                return -1;
            }

			memcpy(obj->IMEI, req->IMEI, IMEI_LENGTH);
			const char* strIMEI = get_IMEI_STRING(req->IMEI);
			memcpy(obj->DID, strIMEI, strlen(strIMEI));
			obj->language = req->language;
			obj->locale = req->locale;

			leancloud_saveDid(obj, ctx);
			mc_obj_add(obj);
		}

		ctx->obj = obj;
	}
	else
	{
		LOG_DEBUG("mc IMEI(%s) already login", get_IMEI_STRING(req->IMEI));
	}

	MC_MSG_LOGIN_RSP *rsp = alloc_rspMsg(msg);
	if (rsp)
	{
		mc_msg_send(rsp, sizeof(MC_MSG_LOGIN_RSP), ctx);
	}
	else
	{
		//TODO: LOG_ERROR
	}

	if (!obj->mosq)
	{
		struct mosquitto* mosq = mqtt_login(get_IMEI_STRING(req->IMEI), "127.0.0.1", 1883,
				app_log_callback,
				app_connect_callback,
				app_disconnect_callback,
				app_message_callback,
				app_subscribe_callback,
				app_publish_callback,
				ctx);
		if (mosq)
		{
			LOG_INFO("%s connect to MQTT successfully", get_IMEI_STRING(req->IMEI));
			obj->mosq = mosq;
		}
		else
		{
			LOG_ERROR("%s failed to connect to MQTT", get_IMEI_STRING(req->IMEI));
		}
	}

	return 0;
}

int mc_gps(const void* msg, CB_CTX* ctx)
{
	const MC_MSG_GPS_REQ* req = msg;

	if (!req)
	{
		LOG_ERROR("msg handle empty");
		return -1;
	}

	if (req->header.length < sizeof(MC_MSG_LOGIN_REQ) - MC_MSG_HEADER_LEN)
	{
		LOG_ERROR("message length not enough");
		return -1;
	}

	LOG_INFO("GPS: lat(%f), lon(%f), speed(%d), course(%d)",
			ntohl(req->lat) / 30000.0 / 60.0, ntohl(req->lon) / 30000.0 / 60.0, req->speed, ntohs(req->course));

	OBJ_MC* obj = ctx->obj;
	if (!obj)
	{
		LOG_WARN("MC must first login");
		return -1;
	}
	//no response message needed

	if (!isYeelinkDeviceCreated(obj))
	{
		yeelink_createDevice(obj, ctx);
	}


	if (obj->lat == ntohl(req->lat)
		&& obj->lon == ntohl(req->lon)
		&& obj->speed == req->speed
		&& obj->course == ntohs(req->course))
	{
		LOG_INFO("No need to upload data");
		return 0;
	}

	//update local object
	obj->lat = ntohl(req->lat);
	obj->lon = ntohl(req->lon);
	obj->speed = req->speed;
	obj->course = ntohs(req->course);
	obj->cell = req->cell;
	obj->timestamp = ntohl(req->timestamp);
	obj->isGPSlocated = req->location & 0x01;

	yeelink_saveGPS(obj, ctx);

	leancloud_saveGPS(obj, ctx);
	dev_sendGpsMsg2App(ctx);

	return 0;
}

int mc_ping(const void* msg, CB_CTX* ctx)
{
	const MC_MSG_PING_REQ *req = msg;

	short status = ntohs(req->status);

	LOG_INFO("GPS located:%s", (status & 1) ? "YES" : "NO");

	MC_MSG_PING_RSP* rsp = alloc_rspMsg(msg);
	if (rsp)
	{
		mc_msg_send(rsp, sizeof(MC_MSG_PING_RSP), ctx);
	}

	return 0;
}

int mc_alarm(const void* msg, CB_CTX* ctx)
{
	OBJ_MC* obj = ctx->obj;

	if (!obj)
	{
		LOG_WARN("MC does not login");
		return -1;
	}

	const MC_MSG_ALARM_REQ* req = msg;

	switch (req->type)
	{
	case FENCE_IN:
	{
		LOG_INFO("FENCE_IN Alarm");
		break;
	}
	case FENCE_OUT:
		LOG_INFO("FENCE_OUT Alarm");
		break;

	default:
		LOG_INFO("Alarm type = %#x", req->type);
	}


	obj->lat = ntohl(req->lat);
	obj->lon = ntohl(req->lon);
	obj->speed = req->speed;
	obj->course = ntohs(req->course);
	obj->cell = req->cell;

	size_t rspMsgLength = sizeof(MC_MSG_ALARM_RSP) + 0; //TODO: currently without any message content
	MC_MSG_ALARM_RSP* rsp = alloc_msg(req->header.cmd, rspMsgLength);
	if (rsp)
	{
		set_msg_seq(&rsp->header, get_msg_seq(req));

		mc_msg_send(&rsp->header, rspMsgLength, ctx);
	}

	//send the alarm to YUNBA
	char topic[128];
	memset(topic, 0, sizeof(topic));
	snprintf(topic, 128, "e2link_%s", get_IMEI_STRING(obj->IMEI));

	cJSON *root = cJSON_CreateObject();

	cJSON *alarm = cJSON_CreateObject();
	cJSON_AddNumberToObject(alarm,"type", req->type);

	cJSON_AddItemToObject(root, "alarm", alarm);
    cJSON_AddStringToObject(root, "alert", "alarm");
	cJSON_AddStringToObject(root, "sound", "default");

	cJSON_AddStringToObject(root, "alert", "FENCE alarm");
	cJSON_AddStringToObject(root, "sound", "alarm.mp3");

	yunba_publish(topic, root);

	cJSON_Delete(root);

	return 0;
}

int mc_status(const void* msg, CB_CTX* ctx)
{
	const MC_MSG_STATUS_REQ* req = msg;

	OBJ_MC* obj = ctx->obj;
	if (!obj)
	{
		LOG_WARN("MC must first login");
		return -1;
	}

	LOG_INFO("MC(%s) Status %x", get_IMEI_STRING(obj->IMEI), req->status);


	obj->lat = ntohl(req->lat);
	obj->lon = ntohl(req->lon);
	obj->speed = req->speed;
	obj->course = ntohs(req->course);
	obj->cell = req->cell;


	MC_MSG_STATUS_RSP* rsp = alloc_rspMsg(msg);
	if (rsp)
	{
		mc_msg_send(rsp, sizeof(MC_MSG_PING_RSP), ctx);
	}
	return 0;
}

int mc_sms(const void* msg, CB_CTX* ctx)
{
	const MC_MSG_SMS_REQ* req = msg;

	MC_MSG_SMS_RSP* rsp = alloc_rspMsg(msg);
	if (rsp)
	{
		mc_msg_send(rsp, sizeof(MC_MSG_PING_RSP), ctx);
	}
	return 0;
}

int mc_operator(const void* msg, CB_CTX* ctx)
{
	OBJ_MC* obj = ctx->obj;

	const MC_MSG_OPERATOR_RSP* req = msg;

	switch (req->type)
	{
	case 0x01:
	{
		int session = req->token;

		dev_sendRspMsg2App(obj->session[session].cmd, obj->session[session].seq, req->data, sizeof(MC_MSG_HEADER) + ntohs(req->header.length) - sizeof(MC_MSG_OPERATOR_RSP), ctx);
		break;
	}

	case 0x02:
		//TODO:handle the msg
		break;

	default:
		break;
	}

	LOG_INFO("MC operator response %s", req->data);

	return 0; //TODO:
}

int mc_data(const void* msg, CB_CTX* ctx __attribute__((unused)))
{
	LOG_INFO("MC data message");

	return 0;
}

