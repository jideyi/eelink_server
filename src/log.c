/*
 * log.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#include <string.h>

#include "log.h"


zlog_category_t* cat[MOD_MAX];

static const char* all_modules[] =
{
		"MAIN",
		"SERVER_MC",
		"SCH_MC",
		"PROC_MC",
		"FSM",
		"HTTP",
		"MQTT",
};

int log_init()
{
    int rc;

    rc = zlog_init("../conf/log.conf");
    if (rc) {
    	printf("%s:%d log init failed\n", __FILE__, __LINE__);
    	return -1;
    }

    memset(cat, 0, sizeof(cat));

    for (size_t i = 0; i < sizeof(all_modules) / sizeof(const char*); i++)
    {
    	zlog_category_t* c = zlog_get_category(all_modules[i]);
        if (!c)
        {
        	printf("get category %s fail\n", all_modules[i]);

        	zlog_fini();
        	return -2;
        }
        cat[i] = c;
    }



    return 0;
}
