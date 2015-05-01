/*
 * leancloud_rsp.c
 *
 *  Created on: May 1, 2015
 *      Author: jk
 */
#include "leancloud_rsp.h"
#include "cb_ctx_mc.h"
#include "object_mc.h"
#include "cJSON.h"

size_t leancloud_onSaveGPS(void *contents, size_t size, size_t nmemb, void *userp)
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
		printf("response: %s", rsp);
	}

	cJSON_Delete(json);
	free(rsp);

	return size * nmemb;
}
