/*
 * yeelink_req.c
 *
 *  Created on: Apr 30, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yeelink_req.h"
#include "cb_ctx_mc.h"
#include "object_mc.h"
#include "cJSON.h"
#include "curl.h"
#include "yeelink_rsp.h"

#define YEELINK_URL_BASE "http://api.yeelink.net/v1.0/devices"

static void yeelink_post(CURL *curl, const char* url, const void* data, int len)
{
    curl_easy_setopt(curl, CURLOPT_URL, url);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	/* pass in a pointer to the data - libcurl will not copy */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	/* size of the POST data */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);


    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    //cleanup when the connect is down, see server_mc.c
}



void yeelink_createDevice(OBJ_MC* obj, void* arg)
{
	CURL *curl = initCurlHandleOfYeelink();
	if (!curl)
	{
		//TODO:
		return;
	}

	cJSON *root = cJSON_CreateObject();

	cJSON_AddStringToObject(root,"title", 	get_IMEI_STRING(obj->IMEI));
	cJSON_AddStringToObject(root,"about", 	obj->DID);
	cJSON_AddStringToObject(root,"tags", 	"[\"GPS\",\"Electrombile\"]");

	cJSON* location = cJSON_CreateObject();
	cJSON_AddStringToObject(location,"local", "Wuhan");
	cJSON_AddNumberToObject(location,"latitude", obj->lat / 30000.0);
	cJSON_AddNumberToObject(location,"longitude",obj->lon / 30000.0);

	cJSON_AddItemToObject(root, "location", location);

	char* data = cJSON_Print(root);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, yeelink_onCreateDevice);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, arg);

	yeelink_post(curl, YEELINK_URL_BASE, data, strlen(data));

	cJSON_Delete(root);
	free(data);

	curl_easy_cleanup(curl);
}

void yeelink_createSensor(int device_id, void* arg)
{
	char url[256] = {0};

	snprintf(url, 256, "http://api.yeelink.net/v1.0/device/%d/sensors", device_id);


	CURL *curl = initCurlHandleOfYeelink();
	if (!curl)
	{
		//TODO:
		return;
	}

	cJSON *root = cJSON_CreateObject();

	cJSON_AddStringToObject(root,"type", "gps");
	cJSON_AddStringToObject(root,"title", "location");
	cJSON_AddStringToObject(root,"about", "the electrombile location");
	cJSON_AddStringToObject(root,"tags", "[\"lat\",\"lng\"]");


	char* data = cJSON_Print(root);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, yeelink_onCreateSensor);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, arg);

	yeelink_post(curl, url, data, strlen(data));

	cJSON_Delete(root);
	free(data);

	curl_easy_cleanup(curl);

}

void yeelink_saveGPS(OBJ_MC* obj, void* arg)
{
	CB_CTX* ctx = arg;
	CURL *curl = ctx->curlOfYeelink;

	char url[256] = {0};

	snprintf(url, 256, "http://api.yeelink.net/v1.0/device/%d/sensor/%d/datapoints", obj->device_id, obj->sensor_id);

	cJSON *root = cJSON_CreateObject();

	cJSON *gps = cJSON_CreateObject();
	cJSON_AddNumberToObject(gps,"lat", obj->lat / 30000.0 / 60.0);
	cJSON_AddNumberToObject(gps,"lng", obj->lon / 30000.0 / 60.0);
	cJSON_AddNumberToObject(gps,"speed", obj->speed);

	cJSON_AddItemToObject(root, "value", gps);

	char* data = cJSON_Print(root);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);

	yeelink_post(curl, url, data, strlen(data));

	cJSON_Delete(root);
	free(data);

}
