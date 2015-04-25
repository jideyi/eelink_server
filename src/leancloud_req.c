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

    struct curl_slist *headerlist=NULL;
    headerlist = curl_slist_append(headerlist, "X-AVOSCloud-Application-Id: 5wk8ccseci7lnss55xfxdgj9xn77hxg3rppsu16o83fydjjn");
    headerlist = curl_slist_append(headerlist, "X-AVOSCloud-Application-Key: yovqy5zy16og43zwew8i6qmtkp2y6r9b18zerha0fqi5dqsw");
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.leancloud.cn/1.1/classes/GPS");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);


	/* pass in a pointer to the data - libcurl will not copy */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	/* size of the POST data */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);


#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Perform the request, res will get the return code */
    CURLcode res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    //TODO: cleanup when the connection is disconnected
    //curl_easy_cleanup(curl);

}
