/*
 * msg_proc_simcom.c
 *
 *  Created on: 2015年6月29日
 *      Author: jk
 */


#include "msg_proc_simcom.h"
#include "protocol.h"
#include "log.h"

typedef int (*MSG_PROC)(const void* msg);
typedef struct
{
    char cmd;
    MSG_PROC pfn;
}MC_MSG_PROC;

static int mc_login(const void* msg);
static int mc_ping(const void* msg);
static int mc_alarm(const void* msg);


static MC_MSG_PROC msgProcs[] =
{
        {CMD_LOGIN, mc_login},
        {CMD_PING,  mc_ping},
        {CMD_ALARM, mc_alarm},
};


int handle_simcom_msg(const char* m, size_t msgLen, void* arg)
{
    MSG_HEADER* msg = (MSG_HEADER*)m;

    if (msgLen < sizeof(MSG_HEADER))
    {
        LOG_ERROR("message length not enough: %zu(at least(%zu)", msgLen, sizeof(MSG_HEADER));

        return -1;
    }

    if (msg->signature != ntohs(START_FLAG))
    {
        LOG_ERROR("message head signature error:%d", msg->signature);
        return -1;
    }

    for (size_t i = 0; i < sizeof(msgProcs) / sizeof(msgProcs[0]); i++)
    {
        if (msgProcs[i].cmd == msg->cmd)
        {
            MSG_PROC pfn = msgProcs[i].pfn;
            if (pfn)
            {
                return pfn(msg);
            }
            else
            {
                LOG_ERROR("Message %d not processed", msg->cmd);
                return -1;
            }
        }
    }

    LOG_ERROR("unknown message cmd(%d)", msg->cmd);
    return -1;
}


static int mc_login(const void* msg)
{
    return 0;
}

static int mc_ping(const void* msg)
{
    return 0;
}

static int mc_alarm(const void* msg)
{
    return 0;
}
