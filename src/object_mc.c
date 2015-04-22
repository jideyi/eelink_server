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

#include "object_mc.h"

#define MAX_MC 100

OBJ_MC* all_mc[100];
int mc_count = 0;

#define CONFIG_FILE "../conf/config.dat"

typedef struct
{
	char IMEI[IMEI_LENGTH];
	char DID[MAX_DID_LEN];
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
		write(fd, &objBuf, sizeof(OBJ_SAVED));
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

OBJ_MC* mc_obj_new()
{
	if (mc_count == MAX_MC)
	{
		return NULL;
	}

	OBJ_MC* obj = malloc(sizeof(OBJ_MC));
	memset(obj, 0, sizeof(OBJ_MC));
	all_mc[mc_count++] = obj;

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
