/*
 * gizwits_rsp.c
 *
 *  Created on: Apr 21, 2015
 *      Author: jk
 */

#include "fsm.h"
#include "cb_ctx_mc.h"

int mc_register_rsp(int response_code, const char* msg, CB_CTX* ctx)
{
	if (response_code == 201)
	{
		//TODO: process the msg

		fsm_run(EVT_GOT_DID, ctx);
		return 0;
	}

	return -1;
}

