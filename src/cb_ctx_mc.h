/*
 * cb_ctx_mc.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_CB_CTX_MC_H_
#define SRC_CB_CTX_MC_H_

#include <stdio.h>
#include <curl/curl.h>
#include <event2/bufferevent.h>


typedef void (*MSG_SEND)(struct bufferevent* bev, const void* buf, size_t n);

typedef struct
{
	struct event_base* base;
	struct bufferevent* bev;
	CURL *curl;
	void* obj;
	MSG_SEND pSendMsg;
	int cur_status;
}CB_CTX;

#endif /* SRC_CB_CTX_MC_H_ */
