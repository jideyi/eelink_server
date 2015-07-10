/*
 * msg_proc_simcom.c
 *
 *  Created on: 2015年6月29日
 *      Author: jk
 */


#include "msg_proc_simcom.h"
#include "protocol.h"
#include "log.h"
#include "cb_ctx_simcom.h"
#include "object_mc.h"

typedef int (*MSG_PROC)(const void* msg, SIMCOM_CTX* ctx);
typedef struct
{
    char cmd;
    MSG_PROC pfn;
}MC_MSG_PROC;

static int simcom_login(const void* msg, SIMCOM_CTX* ctx);
static int simcom_ping(const void* msg, SIMCOM_CTX* ctx);
static int simcom_alarm(const void* msg, SIMCOM_CTX* ctx);


static MC_MSG_PROC msgProcs[] =
{
        {CMD_LOGIN, simcom_login},
        {CMD_PING,  simcom_ping},
        {CMD_ALARM, simcom_alarm},
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
                return pfn(msg, arg);
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


static int simcom_msg_send(void* msg, size_t len, SIMCOM_CTX* ctx)
{
    if (!ctx)
    {
        return -1;
    }

    SIMCOM_MSG_SEND pfn = ctx->pSendMsg;
    if (!pfn)
    {
        LOG_ERROR("device offline");
        return -1;
    }

    pfn(ctx->bev, msg, len);

    LOG_DEBUG("send msg(cmd=%d), length(%ld)", get_msg_cmd(msg), len);
    LOG_HEX(msg, len);

    free(msg);

    return 0;
}

static int simcom_login(const void* msg, SIMCOM_CTX* ctx)
{
    const MSG_LOGIN_REQ* req = msg;


    OBJ_MC* obj = ctx->obj;
    if (!obj)
    {
        LOG_DEBUG("mc IMEI(%s) login", req->IMEI);

        obj = mc_get(req->IMEI);

        if (!obj)
        {
            LOG_INFO("the first time of simcom IMEI(%s)'s login", req->IMEI);

            obj = mc_obj_new();

            memcpy(obj->IMEI, get_IMEI(req->IMEI), IMEI_LENGTH);
            memcpy(obj->DID, req->IMEI, strlen(req->IMEI));

            leancloud_saveDid(obj);
            mc_obj_add(obj);
        }

        ctx->obj = obj;
    }
    else
    {
        LOG_DEBUG("simcom IMEI(%s) already login", get_IMEI_STRING(req->IMEI));
    }

    obj->isOnline = 1;
//    obj->session = ctx;

    MSG_LOGIN_RSP *rsp = alloc_rspMsg(msg);
    if (rsp)
    {
        simcom_msg_send(rsp, sizeof(MSG_LOGIN_RSP), ctx);
    }
    else
    {
        //TODO: LOG_ERROR
    }

    return 0;
}

static int simcom_ping(const void* msg, SIMCOM_CTX* ctx)
{
    return 0;
}

static int simcom_alarm(const void* msg, SIMCOM_CTX* ctx)
{
    return 0;
}
