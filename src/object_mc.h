/*
 * mc_object.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_OBJECT_MC_
#define SRC_OBJECT_MC_

#include "macro_mc.h"

typedef struct
{
	char IMEI[IMEI_LENGTH];
	char language;
	char locale;

	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI cell;
	char location;
	short status;
	short voltage;
	short rxl;
	short analog1;
	short analog2;
}OBJ_MC;

void mc_obj_initial();
OBJ_MC* mc_obj_new();
void mc_obj_del(OBJ_MC* obj);

#endif /* SRC_OBJECT_MC_ */
