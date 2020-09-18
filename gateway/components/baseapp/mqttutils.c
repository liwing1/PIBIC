#include "mqttutils.h"

const char *MQTT_TAG = "MQTT_SAMPLE";

mqtt_client * gb_mqttClient = NULL;

void mqtt_update(void)
{
    char rx_buf[16];
    if(rxQueue != 0){
        if(xQueueReceive(rxQueue, (void*)rx_buf, (TickType_t)5)){
            char topic_temp[20];
            char topic_humi[20];
            char temp_str[4];
            char humi_str[4];

            char *head_id = strtok(rx_buf, "-");

            if(!(head_id[0] >= 'a' && head_id[0] <= 'c')){
                ESP_LOGI("ERROR", "INVALID HEADER");
                return;
            }
                

            sprintf(topic_temp, "node_%s/temp", head_id);
            sprintf(topic_humi, "node_%s/humi", head_id);

            sprintf(temp_str, "%s", strtok(NULL, "-"));
            mqtt_publish(gb_mqttClient, topic_temp, temp_str, strlen(temp_str), 1, 0);
            printf("%s\n", temp_str);

            sprintf(humi_str, "%s", strtok(NULL, "-"));
            mqtt_publish(gb_mqttClient, topic_humi, humi_str, strlen(humi_str), 1, 0);
            printf("%s\n", humi_str);
        }
    }
}

bool DecodePublishResponse(char *topic, char *data)
{
    if(strstr(topic, "AC/onoff") != NULL){
        if(strstr(data, "1")){
            ESP_LOGI("DecodePublishResponse","AC=ON");
            lora_gw_send( "ON" );
        }
        else if(strstr(data, "0")){
            ESP_LOGI("DecodePublishResponse","AC=OFF");
            lora_gw_send( "OFF" );
        }
        return true;
    }
    if(strstr(topic, "AC/therm") != NULL){
        ESP_LOGI("DecodePublishResponse","AC_REF_UPDATE");
        lora_gw_send( data );
        return true;
    }

    return false;
}

void connected_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    mqtt_subscribe(self, "AC/therm", 1);
}

void subscribe_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    ESP_LOGI(MQTT_TAG, "[APP] Subscribe ok, test publish msg");
    mqtt_publish(self, "general/log", "MQTT_UPDATE", strlen("MQTT_UPDATE"), 1, 0);
}

void publish_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    //xEventGroupSetBits(esp32_event_group, MQTT_PUBLISHED_BIT);
}

void data_cb(mqtt_client *client, mqtt_event_data_t *event_data)
{
    char *topic = NULL, *data = NULL;

    if(event_data->data_offset == 0) {

        topic = malloc(event_data->topic_length + 1);
        memcpy(topic, event_data->topic, event_data->topic_length);
        topic[event_data->topic_length] = 0;
        ESP_LOGI(MQTT_TAG, "[APP] Publish topic: %s", topic);
    }

    data = malloc(event_data->data_length + 1);
    memcpy(data, event_data->data, event_data->data_length);
    data[event_data->data_length] = 0;
    ESP_LOGI(MQTT_TAG, "[APP] Publish data[%d/%d bytes]",
             event_data->data_length + event_data->data_offset,
             event_data->data_total_length);
    ESP_LOGI(MQTT_TAG, "Publish Data: %s", data);

    DecodePublishResponse(topic, data);

    free(topic);
    free(data);
}

void disconnected_cb(mqtt_client *client, mqtt_event_data_t *event_data)
{

}

mqtt_settings settings = {
    .host = WEB_SERVER,
/*
#if defined(CONFIG_MQTT_SECURITY_ON)
    .port = 8883, // encrypted
#else
    .port = 1883, // unencrypted
#endif
*/
    .port = WEB_PORT,
    .client_id = " ",
    .username = " ",
    .password = " ",
    .clean_session = 0,
    .keepalive = 120,
    .lwt_topic = "",
    .lwt_msg = "",
    .lwt_qos = 0,
    .lwt_retain = 0,
    .connected_cb = connected_cb,
    .disconnected_cb = disconnected_cb,
    //.reconnect_cb = reconnect_cb,
    .subscribe_cb = subscribe_cb,
    .publish_cb = publish_cb,
    .data_cb = data_cb
};

void wifi_conn_init(void)
{
    tcpip_adapter_init();
    esp32_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_LOGI(MQTT_TAG, "start the WIFI SSID:[%s] password:[%s]", CONFIG_WIFI_SSID, "******");
    ESP_ERROR_CHECK(esp_wifi_start());
}

esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(esp32_event_group, WIFI_CONNECTED_BIT);
            gb_mqttClient = mqtt_start(&settings);
            //init app here
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            // This is a workaround as ESP32 WiFi libs don't currently auto-reassociate.
            esp_wifi_connect();
            mqtt_stop();
            gb_mqttClient = NULL;
            xEventGroupClearBits(esp32_event_group, WIFI_CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}