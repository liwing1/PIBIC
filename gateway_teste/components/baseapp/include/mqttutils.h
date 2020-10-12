#ifndef __MQTTUTILS_H__
#define __MQTTUTILS_H__

#include "header.h"

void connected_cb(mqtt_client *self, mqtt_event_data_t *params);
void subscribe_cb(mqtt_client *self, mqtt_event_data_t *params);
void publish_cb(mqtt_client *self, mqtt_event_data_t *params);
void data_cb(mqtt_client *self, mqtt_event_data_t *params);
void wifi_conn_init(void);
esp_err_t wifi_event_handler(void *ctx, system_event_t *event);
void mqtt_update(void);

extern mqtt_client * gb_mqttClient;
extern mqtt_settings settings;

#endif //__MQTTUTILS_H__