/*
 * leancloud_req.c
 *
 *  Created on: Apr 25, 2015
 *      Author: jk
 */

#include <string.h>

#include "leancloud_req.h"
#include "cb_ctx_mc.h"
#include "object_mc.h"

void leancloud_req(OBJ_MC* obj, void* arg)
{
	CB_CTX* ctx = arg;
	CURL *curl = ctx->curl;

	char data[100] = {0};	//TODO: use macro

	snprintf(data, 100, "{\"did\":\"%s\",\"lat\":%d,\"lon\":%d,\"speed\":%d,\"course\":%d}", obj->DID, obj->lat, obj->lon, obj->speed, obj->course);

	leancloud_post(curl, data, strlen(data));
}

void leancloud_post(CURL *curl, const void* data, int len)
{


    curl_easy_setopt(curl, CURLOPT_URL, "https://api.leancloud.cn/1.1/classes/GPS");

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
