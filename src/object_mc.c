/*
 * object_mc.c
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object_mc.h"

#define MAX_MC 100

OBJ_MC* all_mc[100];
int mc_count = 0;

void mc_obj_initial()
{
	memset(all_mc, 0, sizeof(all_mc));
}

void mc_obj_destruct()
{
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
