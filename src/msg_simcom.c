/*
 * msg_simcom.c
 *
 *  Created on: 2015/6/29
 *      Author: jk
 */

static unsigned short seq = 0;

MSG_HEADER* alloc_msg(char cmd, size_t length)
{
    MSG_HEADER* msg = malloc(length);

    if (msg)
    {
        msg->signature = htons(START_FLAG);
        msg->cmd = cmd;
        msg->seq = htons(seq++);
        msg->length = htons(length - MSG_HEADER_LEN);
    }

    return msg;
}

MSG_HEADER* alloc_rspMsg(const MSG_HEADER* pMsg)
{
    size_t msgLen = 0;
    switch (pMsg->cmd)
    {
    case CMD_LOGIN:
        msgLen = sizeof(MSG_LOGIN_RSP);
        break;

    case CMD_PING:
        msgLen = sizeof(MSG_PING_RSP);
        break;

    case CMD_SMS:
        msgLen = sizeof(MSG_SMS_RSP);    //FIXME: without any sms contents
        break;

    default:
        return NULL;
    }

    MC_MSG_HEADER* msg = malloc(msgLen);

    fill_msg_header(msg);
    set_msg_cmd(msg, pMsg->cmd);
    set_msg_len(msg, msgLen - MSG_HEADER_LEN);
    set_msg_seq(msg, get_msg_seq(pMsg));

    return msg;
}


void free_msg(MSG_HEADER* msg)
{
    free(msg);
}

