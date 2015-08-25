/*
 * mc_object.h
 *
 *  Created on: Apr 19, 2015
 *      Author: jk
 */

#ifndef SRC_OBJECT_
#define SRC_OBJECT_

#include "macro.h"


typedef struct
{
//	char DID[MAX_DID_LEN];
//	char clientID[CLIENT_ID_LEN];
	short cmd;
	unsigned short seq;
}APP_SESSION;


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

	char IMEI[IMEI_LENGTH + 1];
	char language;
	char locale;

	int timestamp;
	int lat;
	int lon;
	char speed;
	short course;
	CGI_MC cell;
	char isGPSlocated;
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
} OBJECT;

void obj_table_initial(void (*func)(const char *));
void obj_table_destruct();

OBJECT *obj_new();
void obj_add(OBJECT *obj);
void obj_del(OBJECT *obj);
OBJECT *obj_get(const char IMEI[]);


int obj_did_got(OBJECT *obj);
const char* get_IMEI_STRING(const unsigned char* IMEI);
const char* getMacFromIMEI(const unsigned char* IMEI);

#endif /* SRC_OBJECT_ */
