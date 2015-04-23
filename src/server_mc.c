#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "log.h"
#include "fsm.h"
#include "server_mc.h"

#include "msg_sch_mc.h"

#define LOG_DEBUG(...) \
	zlog(cat[MOD_SERVER_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
	zlog(cat[MOD_SERVER_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_INFO, __VA_ARGS__)

#define LOG_WARNNING(...) \
	zlog(cat[MOD_SERVER_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_WARNNING, __VA_ARGS__)

#define LOG_ERROR(...) \
	zlog(cat[MOD_SERVER_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_ERROR, __VA_ARGS__)

#define LOG_FATAL(...) \
	zlog(cat[MOD_SERVER_MC], __FILE__, sizeof(__FILE__) - 1, __func__, sizeof(__func__) - 1, __LINE__, ZLOG_LEVEL_FATAL, __VA_ARGS__)


static void send_msg(struct bufferevent* bev, const void* buf, size_t n)
{
	LOG_DEBUG("Send msg rsp %p(len=%zu)", buf, n);
	bufferevent_write(bev, buf, n);
}

static void read_cb(struct bufferevent *bev, void *ctx)
{
	char buf[1024];
	size_t n;
	/* This callback is invoked when there is data to read on bev. */
//	struct evbuffer *input = bufferevent_get_input(bev);
//	struct evbuffer *output = bufferevent_get_output(bev);

//    size_t len = evbuffer_get_length(input);

	LOG_DEBUG("Receive message");

    while ((n = bufferevent_read(bev, buf, sizeof(buf))) > 0)
    {
    	hzlog_debug(cat[MOD_SERVER_MC], buf, n);
    	handle_mc_msg(buf, n, ctx);
    }

	/* Copy all the data from the input buffer to the output buffer. */
	//evbuffer_add_buffer(output, input);
}

static void write_cb(struct bufferevent* bev, void *ctx)
{
	return;
}

static void event_cb(struct bufferevent *bev, short events, void *ctx)
{
	if (events & BEV_EVENT_CONNECTED)
	{
		LOG_DEBUG("Connect okay.\n");
	}
	else if (events & BEV_EVENT_ERROR)
	{
		LOG_ERROR("Error from bufferevent");
	}
	else if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR))
	{
		if (events & BEV_EVENT_ERROR)
		{
			 int err = bufferevent_socket_get_dns_error(bev);
			 if (err)
			 {
				 LOG_ERROR("DNS error: %s\n", evutil_gai_strerror(err));
			 }
		}
		LOG_INFO("Closing");
		bufferevent_free(bev);
	}
}

static void accept_conn_cb(struct evconnlistener *listener,
    evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	/* We got a new connection! Set up a bufferevent for it. */
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	CB_CTX* cb_ctx = malloc(sizeof(CB_CTX));
	cb_ctx->base = base;
	cb_ctx->bev = bev;
	cb_ctx->obj = 0;
	cb_ctx->pSendMsg = send_msg;
	cb_ctx->cur_status = STS_INITIAL;

	//TODO: set the water-mark and timeout
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, cb_ctx);

	bufferevent_enable(bev, EV_READ|EV_WRITE);
}

static void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}

void server_mc_start(struct event_base* base)
{
    struct evconnlistener *listener;
    struct sockaddr_in sin;
    int port = 9876;

    /* Clear the sockaddr before using it, in case there are extra
     * platform-specific fields that can mess us up. */
    memset(&sin, 0, sizeof(sin));
    /* This is an INET address */
    sin.sin_family = AF_INET;
    /* Listen on 0.0.0.0 */
    sin.sin_addr.s_addr = htonl(0);
    /* Listen on the given port. */
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, accept_conn_cb, NULL,
            LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1,
            (struct sockaddr*)&sin, sizeof(sin));
    if (!listener) {
            perror("Couldn't create listener");
            return;
    }
    evconnlistener_set_error_cb(listener, accept_error_cb);

}

