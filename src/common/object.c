/*
 * object.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

#include "log.h"
#include "object.h"
#include "db.h"

/* global hash table */
static GHashTable *object_table = NULL;

static void obj_add_hash(OBJECT *obj)
{
	const char* strIMEI = obj->IMEI;
	g_hash_table_insert(object_table, g_strdup(strIMEI), obj);
    LOG_INFO("obj %s added to hashtable", strIMEI);
}

static void obj_add_db(OBJECT *obj)
{
	db_insertOBJ(obj->IMEI, obj->timestamp);
	LOG_INFO("obj %s added to DB", obj->IMEI);
}

/* it is a callback to initialize object_table.Func db_doWithOBJ needs it to handle with every result(imei, lastlogintime).*/
static void obj_initial(const char *imei, int timestamp)
{
	OBJECT *obj = obj_new();
	memcpy(obj->IMEI, imei, IMEI_LENGTH);
	obj->IMEI[IMEI_LENGTH] = 0;
	obj->timestamp = timestamp;
	obj_add_hash(obj);
}

//it is a callback to update obj into db
static void obj_update(gpointer key, gpointer value, gpointer user_data)
{
	OBJECT *obj = (OBJECT *)value;
	db_updateOBJ(obj->IMEI, obj->timestamp);
}

static void obj_table_save()
{
    /* foreach hash */
    g_hash_table_foreach(object_table, obj_update, NULL);
}


void obj_freeKey(gpointer key)
{
    LOG_DEBUG("free key IMEI:%s of object_table", key);
    g_free(key);
}

void obj_freeValue(gpointer value)
{
    OBJECT * obj = (OBJECT *)value;

    LOG_DEBUG("free value IMEI:%s of object_table", get_IMEI_STRING(obj->IMEI));

    g_free(obj);
}

void obj_table_initial(void (*mqtt_sub)(const char *))
{
    /* create hash table */
    object_table = g_hash_table_new_full(g_str_hash, g_str_equal, obj_freeKey, obj_freeValue);
    /* read imei data from db*/
	db_doWithOBJ(obj_initial, mqtt_sub);
}

void obj_table_destruct()
{
	obj_table_save();
    g_hash_table_destroy(object_table);
}

static void make_pwd(char pwd[])
{
    srand(time(NULL));

    for(int i = 0; i < MAX_PWD_LEN; i++)
	{
        pwd[i] = 65 + rand() % (90 - 65);
	}
    pwd[MAX_PWD_LEN - 1] = '\0';
}

OBJECT *obj_new()
{
	OBJECT * obj = g_malloc(sizeof(OBJECT));
	memset(obj, 0, sizeof(OBJECT));

	make_pwd(obj->pwd);

	return obj;
}

/* add item into hash and db */
void obj_add(OBJECT *obj)
{
	obj_add_hash(obj);
	obj_add_db(obj);
}

void obj_del(OBJECT *obj)
{
    OBJECT * t_obj = obj_get(obj->IMEI);
    if(NULL != t_obj)
    {
        g_hash_table_remove(object_table, obj->IMEI);
    }
}

OBJECT *obj_get(const char IMEI[])
{
    return g_hash_table_lookup(object_table, IMEI);
}

#ifdef __GIZWITS_SUPPORT__
int obj_did_got(OBJECT* obj)
{
	return strlen(obj->DID) != 0;
}

const char* getMacFromIMEI(const unsigned char* IMEI)
{
	/*
	 *
	 * IMEI:  xx xx xx xx xx xx xx xx
	 * MAC:         ~~ ~~ ~~ ~~ ~~ ~~
	 */

	static char mac[MAC_MAC_LEN * 2 + 1] = {0};

    sprintf(mac,"%02X%02X%02X%02X%02X%02X", IMEI[2], IMEI[3],IMEI[4],IMEI[5],IMEI[6],IMEI[7]);


	return mac;
}

#endif

//imei of 8 bits to imei of 16 bits, the result ends by '\0'
const char* get_IMEI_STRING(const unsigned char* IMEI)
{
	static char strIMEI[IMEI_LENGTH + 1];
	strcpy(strIMEI, "unknown imei");

	if (!IMEI)
	{
		return strIMEI;
	}
	for (int i = 0; i < ((IMEI_LENGTH + 1) / 2); i++)
	{
		sprintf(strIMEI + i * 2, "%02x", IMEI[i]);
	}
	strIMEI[IMEI_LENGTH] = 0;

	return strIMEI;
}

//imei of 16 bits to imei of 8 bits, the result ends by '\0'
const unsigned char* get_IMEI(const char* strIMEI)
{
    static unsigned char IMEI[IMEI_LENGTH / 2 + 1];
    unsigned char temp[2] = {0};
    int temp_a, temp_b;

    for (int i = 0; i < IMEI_LENGTH; )
    {
        temp[0] = strIMEI[i];
        temp_a = atoi(temp);
        temp[0] = strIMEI[i + 1];
        temp_b = atoi(temp);
        IMEI[i / 2] = temp_a * 16 + temp_b;
        i += 2;
    }
    IMEI[IMEI_LENGTH / 2] = 0;

    return IMEI;
}


int isYeelinkDeviceCreated(OBJECT * obj)
{
	return obj->device_id != 0 && obj->sensor_id != 0 ;
}
