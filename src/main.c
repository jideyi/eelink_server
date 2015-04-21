#include <event2/event.h>

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

    printf("Electrombile Server V%s\n", VERSION_STR);

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

    event_base_dispatch(base);

    zlog_fini();

    return 0;
}
