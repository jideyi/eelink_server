#include <event2/event.h>
#include <mosquitto.h>
#include <curl/curl.h>
 #include <signal.h>
 #include <openssl/ssl.h>

#include "log.h"
#include "version.h"
#include "server_mc.h"
#include "curl.h"
#include "yunba_push.h"
#include "object_mc.h"

struct event_base *base = NULL;

static void sig_usr(int signo)
{
	if (signo == SIGINT)
	{
		printf("oops! catch CTRL+C!!!\n");
		event_base_loopbreak(base);
	}

	if (signo == SIGTERM)
	{
		printf("oops! being killed!!!\n");
		event_base_loopbreak(base);
	}
}

int main(int argc, char **argv)
{

    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Electrombile Server %s, with event %s, mosquitto %d, curl %s\n",
    		VERSION_STR,
			LIBEVENT_VERSION,
			mosquitto_lib_version(NULL, NULL, NULL),
			curl_version());

    base = event_base_new();
    if (!base)
        return 1; /*XXXerr*/

    int rc = log_init();
    if (rc)
    {
    	return rc;
    }

    mc_obj_initial();

    struct evconnlistener* listener = server_mc_start(base);
    if (listener)
    {
    	LOG_INFO("start mc server sucessfully");
    }
    else
    {
    	LOG_FATAL("start mc server failed");
    	return 2;
    }
    

    if (signal(SIGINT, sig_usr) == SIG_ERR)
    {
        LOG_ERROR("Can't catch SIGINT");
    }
    if (signal(SIGTERM, sig_usr) == SIG_ERR)
    {
    	LOG_ERROR("Can't catch SIGTERM");
    }

    rc = mosquitto_lib_init();
    if (rc != MOSQ_ERR_SUCCESS)
    {
    	LOG_ERROR("mosquitto lib initial failed: rc=%d", rc);
    	return -1;
    }

    rc = yunba_connect();
    if (rc)
    {
    	LOG_FATAL("connect to yunba failed");
    	return -1;
    }

    rc = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (rc != CURLE_OK)
    {
    	LOG_FATAL("curl lib initial failed:%d", rc);
    }


    //start the event loop
    LOG_INFO("start the event loop");
    event_base_dispatch(base);

    //cleanup all resouce
    mc_obj_destruct();
    evconnlistener_free(listener);
    event_base_free(base);
    yunba_disconnect();
	mosquitto_lib_cleanup();
    curl_global_cleanup();
    clcanupLeancloudHeader();
    cleanupYeelinkHeader();

//    sk_free(SSL_COMP_get_compression_methods());
    LOG_INFO("stop mc server sucessfully");

    zlog_fini();

    return 0;
}
