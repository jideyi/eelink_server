/*
 * server_admin.h
 *
 *  Created on: 2015/6/25
 *      Author: jk
 */

#ifndef SRC_SERVER_ADMIN_H_
#define SRC_SERVER_ADMIN_H_

#include <event2/event.h>

struct evconnlistener* admin_start(struct event_base*, int);

#endif /* SRC_SERVER_ADMIN_H_ */
