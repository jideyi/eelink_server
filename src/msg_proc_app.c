/*
 * msg_proc_app.c
 *
 *  Created on: May 12, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

#include "cb_ctx_mc.h"
#include "object_mc.h"
#include "msg_app.h"
#include "msg_mc.h"
#include "log.h"
#include "cJSON.h"


void app_sendRawData2mc(const void* msg, int len, CB_CTX* ctx, int session)
{
	MC_MSG_OPERATOR_REQ* req = alloc_msg(CMD_OPERAT, sizeof(MC_MSG_OPERATOR_REQ) + len);
	if (req)
	{
		req->type = 0x01;	//FIXME: use enum instead
		req->token = session;
		memcpy(req->data, msg, len);
		mc_msg_send(req, sizeof(MC_MSG_OPERATOR_REQ) + len, ctx);
	}
}

void dev_sendRawData2App(const char* topic, const char* data, const int len, CB_CTX* ctx)
{
	OBJ_MC* obj = ctx->obj;

	LOG_HEX(data, len);
	mosquitto_publish(obj->mosq, NULL, topic, len, data, 0, false);	//TODO: determine the parameter

}

void dev_sendRspMsg2App(short cmd, short seq, const void* data, const int len, CB_CTX* ctx)
{
	OBJ_MC* obj = ctx->obj;

	APP_MSG* msg = malloc(sizeof(APP_MSG) + len);
	msg->header = htons(0xAA55);
	msg->cmd = htons(cmd);
	msg->length = htons(len + sizeof(msg->seq));
	msg->seq = htons(seq);
	memcpy(msg->data, data, len);

	char topic[1024] = {0}; //FIXME: how long should be?
	snprintf(topic, 1024, "dev2app/%s/e2link/cmd", get_IMEI_STRING(obj->IMEI));

	dev_sendRawData2App(topic, msg, sizeof(APP_MSG) + len, ctx);
}


void dev_sendGpsMsg2App(CB_CTX* ctx)
{
	OBJ_MC* obj = ctx->obj;

	GPS_MSG* msg = malloc(sizeof(GPS_MSG));
	msg->header = htons(0xAA55);
	msg->timestamp = obj->timestamp;
	msg->lat = obj->lat;
	msg->lng = obj->lon;
	msg->course = obj->course;
	msg->speed = obj->speed;
	msg->isGPS = 1; 	//TODO:

	char topic[1024] = {0}; //FIXME: how long should be?
	snprintf(topic, 1024, "dev2app/%s/e2link/gps", get_IMEI_STRING(obj->IMEI));

	dev_sendRawData2App(topic, msg, sizeof(GPS_MSG), ctx);
}

int app_handleApp2devMsg(const char* topic, const char* data, const int len, void* userdata)
{
	CB_CTX* ctx = userdata;
	OBJ_MC* obj = ctx->obj;

	LOG_HEX(data, len);

	//check the IMEI
	const char* pStart = &topic[strlen("app2dev/")];
	const char* pEnd = strstr(pStart, "/");
	if (strncmp(get_IMEI_STRING(obj->IMEI), pStart, pEnd - pStart) != 0)
	{
		LOG_ERROR("App2dev msg IMEI error");
		return -1;
	}

	APP_MSG* pMsg = data;
	//check the msg header
	if (ntohs(pMsg->header) != 0xAA55)
	{
		LOG_ERROR("App2dev msg header error");
		return -1;
	}

	//check the msg length
	if ((ntohs(pMsg->length) + sizeof(short) * 3) != len)
	{
		LOG_ERROR("App2dev msg format error");
		return -1;
	}

	int session = obj->curSession++;
	obj->session[session].cmd = ntohs(pMsg->cmd);
	obj->session[session].seq = ntohs(pMsg->seq);

	switch (ntohs(pMsg->cmd))
	{
	case CMD_WILD:
		LOG_INFO("receive app wildcard cmd");
		app_sendRawData2mc(pMsg->data, ntohs(pMsg->length) - sizeof(pMsg->seq), ctx, session);
		break;
	case CMD_FENCE_SET:
	case CMD_FENCE_DEL:
	case CMD_FENCE_GET:
		LOG_INFO("receive app CMD:%#x", ntohs(pMsg->cmd));
		app_sendRawData2mc(pMsg->data, ntohs(pMsg->length) - sizeof(pMsg->seq), ctx, session);
		break;
	case CMD_TEST_GPS:
		dev_sendGpsMsg2App(ctx);
		break;
	case CMD_TEST_ALARM:
	{
		//send the alarm to YUNBA
		char topic[128];
		memset(topic, 0, sizeof(topic));
		snprintf(topic, 128, "e2link_%s", get_IMEI_STRING(obj->IMEI));

		cJSON *root = cJSON_CreateObject();

		cJSON *alarm = cJSON_CreateObject();
		cJSON_AddNumberToObject(alarm,"type", 0x81);

		cJSON_AddItemToObject(root, "alarm", alarm);

		yunba_publish(topic, root);
		LOG_INFO("send test alarm to app");

		cJSON_Delete(root);
		break;
	}

	default:
		LOG_ERROR("Unknown cmd: %#x", ntohs(pMsg->cmd));
		break;
	}

	return 0;
}

void app_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
	if(message->payloadlen){
		LOG_DEBUG("%s %p", message->topic, message->payload);
	}else{
		LOG_DEBUG("%s no payload(null)", message->topic);
	}
	fflush(stdout);

	LOG_INFO("recieve PUBLISH: %s", message->topic);

	if(strncmp(message->topic,"app2dev/",strlen("app2dev/")) == 0)
	{
		app_handleApp2devMsg(message->topic, message->payload, message->payloadlen, userdata);
	}
	else
	{
		LOG_ERROR("Receive unknown PUBLISH");
	}

}

void app_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
	CB_CTX* ctx = userdata;
	OBJ_MC* obj = ctx->obj;

	if(!result)
	{
		char topic[100];	//TODO: fix magic number
		memset(topic, 0, sizeof(topic));
		/* Subscribe to broker information topics on successful connect. */

		snprintf(topic, 100, "app2dev/%s/e2link/cmd", get_IMEI_STRING(obj->IMEI));
		mosquitto_subscribe(mosq, NULL, topic, 0);
	}
	else
	{
		//TODO: check whether mosquitto_connack_string here is OK
		LOG_ERROR("Connect failed: result = %s", mosquitto_connack_string(result));
	}
}
void app_disconnect_callback(struct mosquitto *mosq, void *userdata, int rc)
{
	CB_CTX* ctx = userdata;
	OBJ_MC* obj = ctx->obj;

	if(rc)
	{
		LOG_ERROR("%s disconnect rc = %d(%s)\n", obj->DID, rc, mosquitto_strerror(rc));

		//mosquitto_reconnect(mosq);
	}
	else
	{
		LOG_ERROR("client disconnect\n");	}
}


void app_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
	LOG_DEBUG("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(int i=1; i<qos_count; i++){
		LOG_DEBUG(", %d", granted_qos[i]);
	}
}

void app_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	switch (level)
	{
	case MOSQ_LOG_DEBUG:
		LOG_DEBUG("%s", str);

		break;
	case MOSQ_LOG_INFO:
	case MOSQ_LOG_NOTICE:
	case MOSQ_LOG_SUBSCRIBE:
	case MOSQ_LOG_UNSUBSCRIBE:
		LOG_INFO("%s", str);
		break;

	case MOSQ_LOG_WARNING:
		LOG_WARN("%s", str);
		break;

	case MOSQ_LOG_ERR:
		LOG_ERROR("%s", str);
		break;

	default:
		LOG_ERROR("unknown level log:%s", str);
	}

}

void app_publish_callback(struct mosquitto *mosq, void *userdata, int mid)
{
	OBJ_MC* obj = userdata;

}
