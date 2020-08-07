#ifndef __HEADER_H__
#define __HEADER_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "mqttutils.h"

#define WIFI_SSID                   "NEO 2G"
#define WIFI_PASSWORD               "6138777771"

#define WEB_SERVER                  "192.168.0.144"
#define WEB_PORT                    1883                                // should be an integer and not a string

#define WIFI_CONNECTED_BIT          BIT0
#define MQTT_PUBLISHED_BIT          BIT1
#define MQTT_INITIATE_PUBLISH_BIT   BIT2
#define MQTT_NODE_A_PUBLISH         BIT3
#define MQTT_NODE_B_PUBLISH         BIT4
#define MQTT_NODE_C_PUBLISH         BIT5
#define MQTT_ALL_NODES              (BIT3|BIT4|BIT5)

extern EventGroupHandle_t esp32_event_group;

esp_err_t wifi_event_handler(void *ctx, system_event_t *event);

#endif //__HEADER_H__