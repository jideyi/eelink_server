/*
 * sql.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#include "object_mc.h"
#include "log.h"
#include "sql.h"

/* mysql database settings */
const char *g_host_name = "localhost";
const char *g_user_name = "root";
const char *g_password = "root";
const char *g_db_name = "GPS_DATA";
const unsigned int g_db_port = 3306;
const char *g_table_name = "`gps_data`";

/* global mysql connector */
MYSQL *g_conn;

/* the max size of sql destrction */
#define MAX_BUF_SIZE 1024

/* execute sql */
static int executesql(const char *sql)
{
    /*query the database according the sql*/
    if (mysql_real_query(g_conn, sql, strlen(sql)))
    {
        LOG_ERROR("execute (%s) failed!", sql);
        return -1;
    }

    return 0;
}


/* initial mysql connect */
int mysql_initial()
{
    g_conn = mysql_init(NULL);
    if(!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password,
                           g_db_name, g_db_port, NULL, 0))
    {
        LOG_ERROR("connect mysql failed");
        return -1;
    }
    LOG_DEBUG("connect mysql success!");

    if (executesql("set names utf8"))
    {
        return -1;
    }

    return 0;
}

/* add to mysql table */
int mysql_add(void* arg)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[MAX_BUF_SIZE] = {0};

    OBJ_MC* obj = arg;

    sprintf(sql, "SELECT * FROM %s WHERE IMEI = %s", g_table_name, get_IMEI_STRING(obj->IMEI));
    /* check if exists */
    if (executesql(sql))
    {
        return -1;
    }
    res = mysql_store_result(g_conn);
    /* if not, insert */
    if (!(row = mysql_fetch_row(res)))
    {
        sprintf(sql,
                "INSERT INTO %s (IMEI, DID, LAT, LON, SPEED, COURSE, TIME) VALUES (\"%s\", \"%s\", %d, %d, %d, %d, %d)",
                 g_table_name,
                 get_IMEI_STRING(obj->IMEI),
                 obj->DID,
                 obj->lat,
                 obj->lon,
                 obj->speed,
                 obj->course,
                 obj->timestamp);

        if (executesql(sql))
        {
            /* insert failed */
            LOG_ERROR("mysql insert IMEI(%s) failed", get_IMEI_STRING(obj->IMEI));
            return -1;
        }
    }
    /* if yes, update*/
    else
    {
        sprintf(sql,
                "UPDATE %s SET DID = \"%s\", LAT = %d, LON = %d, SPEED = %d, \
                COURSE = %d, TIME = %d WHERE IMEI = \"%s\"",
                 g_table_name,
                 obj->DID,
                 obj->lat,
                 obj->lon,
                 obj->speed,
                 obj->course,
                 obj->timestamp,
                 get_IMEI_STRING(obj->IMEI));

        if (executesql(sql))
        {
            /* insert failed */
            LOG_ERROR("mysql update IMEI(%s) failed", get_IMEI_STRING(obj->IMEI));
            return -1;
        }
    }

    mysql_free_result(res);
    return 0;
}

/* close mysql connect */
void mysql_quit()
{
    mysql_close(g_conn);
}
