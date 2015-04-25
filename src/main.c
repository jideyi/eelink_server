#include <event2/event.h>
#include <mosquitto.h>
#include <curl/curl.h>

#include "gizwits_req.h"
#include "log.h"
#include "version.h"
#include "server_mc.h"
#include "object_mc.h"

#define LOG(...) \
	zlog(cat[MOD_MAIN], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

int main(int argc, char **argv)
{
    struct event_base *base = NULL;

    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Electrombile Server %s, with event %s, mosquitto %d\n", VERSION_STR, LIBEVENT_VERSION, mosquitto_lib_version(NULL, NULL, NULL));

    base = event_base_new();
    if (!base)
        return 1; /*XXXerr*/

    int ret = log_init();
    if (ret)
    {
    	return ret;
    }

    mc_obj_initial();

    server_mc_start(base);
    
    LOG("start mc server sucessfully");

    mosquitto_lib_init();

    curl_global_init(CURL_GLOBAL_DEFAULT);

    event_base_dispatch(base);

	mosquitto_lib_cleanup();
	curl_global_cleanup();
    zlog_fini();

    return 0;
}
