/*
 * object_mc.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "object_mc.h"

#define MAX_MC 100

OBJ_MC* all_mc[100];
int mc_count = 0;

#define CONFIG_FILE "../conf/config.dat"

typedef struct
{
	char IMEI[IMEI_LENGTH];
	char DID[MAX_DID_LEN];
	char PWD[MAX_PWD_LEN];
        int device_id;
        int sensor_id;
}OBJ_SAVED;

int mc_getConfig()
{
	int fd = open(CONFIG_FILE, O_RDONLY);
	if (fd == -1)
	{
        printf("open file fail\r\n");
        return -1;
	}

	while(1)
	{
		OBJ_SAVED objBuf;
		ssize_t readlen = read(fd, &objBuf, sizeof(OBJ_SAVED));
		if (readlen == sizeof(OBJ_SAVED))
		{
			OBJ_MC* obj = mc_obj_new();
			memcpy(obj->IMEI, objBuf.IMEI, IMEI_LENGTH);
			memcpy(obj->DID, objBuf.DID, MAX_DID_LEN);
			memcpy(obj->pwd, objBuf.PWD, MAX_DID_LEN);
                        obj->device_id = objBuf.device_id;
                        obj->sensor_id = objBuf.sensor_id;
		}
		else
		{
			break;
		}
	};

    close(fd);
    return 0;
}

int mc_saveConfig()
{
    int fd = open(CONFIG_FILE, O_RDWR | O_CREAT, S_IRWXU);
    if(-1 == fd)
    {
        printf("open file fail\r\n");
        return -1;
    }

    for (int i = 0; i < mc_count; i++)
    {
    	OBJ_SAVED objBuf;
		memcpy(objBuf.IMEI, all_mc[i]->IMEI, IMEI_LENGTH);
		memcpy(objBuf.DID, all_mc[i]->DID, MAX_DID_LEN);
		memcpy(objBuf.PWD, all_mc[i]->pwd, MAX_PWD_LEN);
                objBuf.device_id = all_mc[i]->device_id;
                objBuf.sensor_id = all_mc[i]->sensor_id;
		ssize_t written = write(fd, &objBuf, sizeof(OBJ_SAVED));
		if (written == -1)
		{
			//TODO:log
			break;
		}
    }

    close(fd);

    return 0;
}

void mc_obj_initial()
{
	memset(all_mc, 0, sizeof(all_mc));

	mc_getConfig();
}

void mc_obj_destruct()
{
	mc_saveConfig();


	for (int i = 0; i < mc_count; i++)
	{
		free(all_mc[i]);
	}
	mc_count = 0;
}

void make_pwd(char pwd[])
{
    srand(time(NULL));

    for(int i = 0; i < MAX_PWD_LEN; i++)
	{
        pwd[i] = 65 + rand() % (90 - 65);
	}
    pwd[MAX_PWD_LEN - 1] = '\0';
}

OBJ_MC* mc_obj_new()
{
	if (mc_count == MAX_MC)
	{
		return NULL;
	}

	OBJ_MC* obj = malloc(sizeof(OBJ_MC));
	memset(obj, 0, sizeof(OBJ_MC));
	all_mc[mc_count++] = obj;

	make_pwd(obj->pwd);

	return obj;
}

void mc_obj_del(OBJ_MC* obj)
{
	for (int i = 0; i < mc_count; i++)
	{
		if (obj == all_mc[i])
		{
			mc_count--;
			for (int j = i; j < mc_count; j++)
			{
				all_mc[j] = all_mc[j + 1];
			}
			break;
		}
	}
}

OBJ_MC* mc_get(char IMEI[])
{
	for (int i = 0; i < mc_count; i++)
	{
		OBJ_MC* obj = all_mc[i];
		if(obj)
		{
			if (strcmp(obj->IMEI, IMEI) == 0)
			{
				return obj;
			}
		}
	}

	return NULL;
}

int mc_obj_did_got(OBJ_MC* obj)
{
	return strlen(obj->DID) != 0;
}


const char* get_IMEI_STRING(const unsigned char* IMEI)
{
	static char strIMEI[IMEI_LENGTH * 2 + 1];
	for (int i = 0; i < IMEI_LENGTH; i++)
	{
		sprintf(strIMEI + i * 2, "%02x", IMEI[i]);
	}
	strIMEI[IMEI_LENGTH * 2] = 0;

	return strIMEI;
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

int isYeelinkDeviceCreated(OBJ_MC* obj)
{
	return obj->device_id != 0 && obj->sensor_id != 0 ;
}
