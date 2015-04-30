/*
 * yeelink_rsp.c
 *
 *  Created on: Apr 30, 2015
 *      Author: jk
 */

#include "yeelink_rsp.h"
#include "cb_ctx_mc.h"
#include "object_mc.h"
#include "cJSON.h"

size_t yeelink_onCreateDevice(void *contents, size_t size, size_t nmemb, void *userp)
{
	CB_CTX* ctx = userp;
	OBJ_MC* obj = ctx->obj;

	char* rsp = malloc(size * nmemb + 1);
	memcpy(rsp, contents, size * nmemb);
	rsp[size * nmemb] = 0;

	cJSON* json = cJSON_Parse(rsp);
	if (!json)
	{
		printf("error parse respone:%s", rsp);
	}
	else
	{
		int device_id = cJSON_GetObjectItem(json, "device_id")->valueint;
		obj->device_id = device_id;
		yeelink_createSensor(device_id, userp);
	}

	return size * nmemb;
}

size_t yeelink_onCreateSensor(void *contents, size_t size, size_t nmemb, void *userp)
{
	CB_CTX* ctx = userp;
	OBJ_MC* obj = ctx->obj;

	char* rsp = malloc(size * nmemb + 1);
	memcpy(rsp, contents, size * nmemb);
	rsp[size * nmemb] = 0;


	cJSON* json = cJSON_Parse(rsp);
	if (!json)
	{
		printf("error parse respone:%s", rsp);
	}
	else
	{
		int sensor_id = cJSON_GetObjectItem(json, "sensor_id")->valueint;
		obj->sensor_id = sensor_id;
	}

	return size * nmemb;
}
