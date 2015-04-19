#include <event2/event.h>

#include "log.h"
#include "server_mc.h"
#include "server_gizwits.h"

int main(int argc, char **argv)
{
    struct event_base *base = NULL;

    setvbuf(stdout, NULL, _IONBF, 0);

    base = event_base_new();
    if (!base)
        return 1; /*XXXerr*/

    int ret = log_init();
    if (ret)
    {
    	return ret;
    }

    server_mc_start(base);
    
    zlog_info(cat[MOD_MAIN], "start mc server sucessfully");

    server_gizwits_start(base);

    event_base_dispatch(base);

    zlog_fini();

    return 0;
}
