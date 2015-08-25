/*
 * leancloud_req.c
 *
 *  Created on: Apr 25, 2015
 *      Author: jk
 */

#include <string.h>
#include <stdlib.h>

#include "leancloud_req.h"
#include "leancloud_rsp.h"
#include "cb_ctx_mc.h"
#include "object_mc.h"
#include "cJSON.h"
#include "log.h"

#define LEANCLOUD_URL_BASE "https://api.leancloud.cn/1.1"

static void leancloud_post(CURL *curl, const char* class, const void* data, int len)
{
	char url[256] = {0};


	snprintf(url, 256, "%s/classes/%s", LEANCLOUD_URL_BASE, class);

    curl_easy_setopt(curl, CURLOPT_URL, url);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	/* pass in a pointer to the data - libcurl will not copy */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	/* size of the POST data */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

    LOG_INFO("Post leancloud: url->%s, data->%s", url, data);

    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);


    /* Check for errors */
    if(res != CURLE_OK)
    {
    	LOG_ERROR("curl_easy_perform() failed: %s",curl_easy_strerror(res));
    }

    //cleanup when the connect is down, see server_mc.c
}

void leancloud_saveGPS(OBJ_MC* obj, void* arg)
{
	CB_CTX* ctx = arg;
	CURL *curl = ctx->curlOfLeancloud;

	cJSON *root = cJSON_CreateObject();

	cJSON_AddStringToObject(root,"IMEI", 	get_IMEI_STRING(obj->IMEI));
	cJSON_AddStringToObject(root,"did", 	obj->DID);
	cJSON_AddNumberToObject(root,"lat",	obj->lat / 30000.0);
	cJSON_AddNumberToObject(root,"lon",	obj->lon / 30000.0);
	cJSON_AddNumberToObject(root,"speed",	obj->speed);
	cJSON_AddNumberToObject(root,"course",	obj->course);
	cJSON_AddNumberToObject(root,"time",obj->timestamp);
	char* data = cJSON_PrintUnformatted(root);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, leancloud_onSaveGPS);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, arg);
	leancloud_post(curl, "GPS", data, strlen(data));
	cJSON_Delete(root);
	free(data);
//	char data[100] = {0};	//use macro
//	snprintf(data, 100, "{\"did\":\"%s\",\"lat\":%d,\"lon\":%d,\"speed\":%d,\"course\":%d}", obj->DID, obj->lat, obj->lon, obj->speed, obj->course);
//	leancloud_post(curl, data, strlen(data));
}

void leancloud_saveDid(OBJ_MC* obj, void* arg)
{
	CB_CTX* ctx = arg;
	CURL *curl = ctx->curlOfLeancloud;

	cJSON *root = cJSON_CreateObject();

	cJSON_AddStringToObject(root,"did", 	obj->DID);
	cJSON_AddStringToObject(root,"IMEI",	get_IMEI_STRING(obj->IMEI));
	cJSON_AddStringToObject(root,"password",obj->pwd);
	char* data = cJSON_PrintUnformatted(root);
	leancloud_post(curl, "DID", data, strlen(data));
	cJSON_Delete(root);
	free(data);
}
