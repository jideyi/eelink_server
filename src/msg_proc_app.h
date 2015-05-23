/*
 * msg_proc_app.h
 *
 *  Created on: May 12, 2015
 *      Author: jk
 */

#ifndef SRC_MSG_PROC_APP_H_
#define SRC_MSG_PROC_APP_H_

void app_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message);
void app_connect_callback(struct mosquitto *mosq, void *userdata, int result);
void app_disconnect_callback(struct mosquitto *mosq, void *userdata, int rc);
void app_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void app_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str);
void app_publish_callback(struct mosquitto *mosq, void *userdata, int mid);



#endif /* SRC_MSG_PROC_APP_H_ */
