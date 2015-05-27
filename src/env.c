/*
 * env.c
 *
 *  Created on: May 27, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>

#include "env.h"

ENVIRONMENT env = {
		.curl_leancloud = NULL,
		.curl_yeelink = NULL
};

void env_initial()
{
    env.curl_leancloud = initCurlHandleOfLeancloud();
    env.curl_yeelink = initCurlHandleOfYeelink();

    env.chunk.memory = malloc(1);
    env.chunk.size = 0;
}

void env_cleanup()
{
    //cleanup all resource
    clcanupLeancloudHeader();
    cleanupYeelinkHeader();

	cleanupLeancloudCurlHandle(env.curl_leancloud);
	cleanupYeelinkCurlHandle(env.curl_yeelink);

	free(env.chunk.memory);
}

ENVIRONMENT* env_get()
{
	return &env;
}

