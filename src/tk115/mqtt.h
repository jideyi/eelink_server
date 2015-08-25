/*
 * mqtt.h
 *
 *  Created on: Apr 21, 2015
 *      Author: jk
 */

#ifndef SRC_MQTT_H_
#define SRC_MQTT_H_


void mqtt_initial();
void mqtt_cleanup();

void mqtt_publish(const char *topic, const void *payload, int payloadlen);
void mqtt_subscribe(const char *imei);
void mqtt_unsubscribe(const char *imei);

#endif /* SRC_MQTT_H_ */
