
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "gizwits_req.h"
#include "gizwits_rsp.h"
#include "http.h"


void mc_register(void* ctx)
{
	HTTP_SESSION *session = malloc(sizeof(HTTP_SESSION));
	init_session(session, ctx, mc_register_rsp);

	char * data = "product_key=e08e87504a2a4b5c934e99e3983bc219&passcode=gokit&mac=112233445566&type=normal";
	http_post(session, "http://api.gizwits.com/dev/devices", data);

}
