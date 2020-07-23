#include "mqttutils.h"

const char *MQTT_TAG = "MQTT_SAMPLE";

mqtt_client * gb_mqttClient = NULL;

void connected_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    mqtt_client *client = (mqtt_client *)self;
    const char topic_subscribe[] = "sensors/configuration";
    mqtt_subscribe(client, topic_subscribe, 1);
}

void subscribe_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    ESP_LOGI(MQTT_TAG, "[APP] Subscribe ok, test publish msg");
    const char topic_publish[] = "sensors/values";
    char body[25];
    sprintf(body, "AGORAS");
    mqtt_client *client = (mqtt_client *)self;
    mqtt_publish(client, topic_publish, body, strlen(body), 1, 0);                            // sizeof()-1 to compensate for the trailing '\0' in the string
}

void publish_cb(mqtt_client *self, mqtt_event_data_t *params)
{
    xEventGroupSetBits(esp32_event_group, MQTT_PUBLISHED_BIT);
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

    //DecodePublishResponse(topic, data);

    free(topic);
    free(data);
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
    //.port = WEB_PORT,
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
    //.disconnected_cb = disconnected_cb,
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