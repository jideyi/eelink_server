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
	/*
	 * IMEI = TAC(6) + FAC(2) + SNR(6) + SP(1)
	 * TAC: Type Approval Code �ͺź�׼����
	 * FAC: Final Assembly Code ���װ���
	 * SNR: Serial Number
	 * SP: У����
	 *
	 * We use the SNR for MAC
	 */

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

	char DID[MAX_DID_LEN];
	char pwd[MAX_PWD_LEN];

	char m2m_host[100];
    int m2m_Port;

    int device_id;
    int sensor_id;

	struct mosquitto *mosq;
}OBJ_MC;

typedef struct
{
	char DID[MAX_DID_LEN];
	char clientID[CLIENT_ID_LEN];
}APP_SESSION;

void mc_obj_initial();
OBJ_MC* mc_obj_new();
void mc_obj_del(OBJ_MC* obj);
OBJ_MC* mc_get(char IMEI[]);

int isYeelinkDeviceCreated(OBJ_MC* obj);
int mc_obj_did_got(OBJ_MC* obj);
const char* get_IMEI_STRING(const unsigned char* IMEI);
const char* getMacFromIMEI(const unsigned char* IMEI);

#endif /* SRC_OBJECT_MC_ */
