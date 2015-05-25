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
#include "log.h"
#include "macro_mc.h"

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
		LOG_ERROR("error parse respone:%s", rsp);
	}
	else
	{
		LOG_INFO("response: %s", rsp);
	}

	cJSON_Delete(json);
	free(rsp);

	return size * nmemb;
}



size_t leancloud_onRev(void *contents, size_t size, size_t nmemb, void *userp)
{
    char *rsp;
    size_t revsize = size * nmemb;
    CB_CTX *ctx = userp;

    rsp = malloc(revsize + 1);
    memcpy(rsp, contents, revsize);
    rsp[revsize] = 0;

    ctx->RevBuffer = realloc(ctx->RevBuffer, ctx->Revsize + revsize +1);
    memcpy(ctx->RevBuffer + ctx->Revsize, rsp, revsize + 1);
    ctx->Revsize += revsize;

    LOG_DEBUG("receiving leancloud");

    free(rsp);
    return revsize;
}
