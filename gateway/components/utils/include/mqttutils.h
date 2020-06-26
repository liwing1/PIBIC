#ifndef __MQTTUTILS_H__
#define __MQTTUTILS_H__

#include "header.h"

#define WIFI_SSID                   "NEO 2G"
#define WIFI_PASSWORD               "6138777771"
#define WEB_SERVER                  "192.168.0.85"
#define WEB_PORT                    1883                                // should be an integer and not a string

#define WIFI_CONNECTED_BIT           BIT0
#define MQTT_PUBLISHED_BIT           BIT1
#define MQTT_INITIATE_PUBLISH_BIT    BIT2


void connected_cb(void *self, void *params);
void subscribe_cb(void *self, void *params);
void publish_cb(void *self, void *params);
void data_cb(void *self, void *params);
void wifi_conn_init(void);
esp_err_t wifi_event_handler(void *ctx, system_event_t *event);

extern mqtt_client * gb_mqttClient;
extern mqtt_settings settings;
extern EventGroupHandle_t esp32_event_group;

#endif //__MQTTUTILS_H__