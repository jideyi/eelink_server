/*
 * curl.c
 *
 *  Created on: Apr 30, 2015
 *      Author: jk
 */
#include <stdio.h>

#include "curl.h"

static CURL* initCurlHandle()
{
	CURL* curl = curl_easy_init();

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
    curl_easy_setopt(curlOfLeancloud, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curlOfLeancloud, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    return curl;
}

CURL* initCurlHandleOfLeancloud()
{
	CURL* curl = initCurlHandle();

	if (!curl)
	{
		return NULL;
	}

    struct curl_slist *headerlist=NULL;
    headerlist = curl_slist_append(headerlist, "X-AVOSCloud-Application-Id: 5wk8ccseci7lnss55xfxdgj9xn77hxg3rppsu16o83fydjjn");
    headerlist = curl_slist_append(headerlist, "X-AVOSCloud-Application-Key: yovqy5zy16og43zwew8i6qmtkp2y6r9b18zerha0fqi5dqsw");
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    return curl;
}

CURL* initCurlHandleOfYeelink()
{
	CURL* curl = initCurlHandle();

	if (!curl)
	{
		return NULL;
	}

    struct curl_slist *headerlist=NULL;
    headerlist = curl_slist_append(headerlist, "U-ApiKey:f8864ad808704dc4003f0d135774beaf");
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

    return curl;
}
