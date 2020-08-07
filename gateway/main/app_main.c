#include "header.h"
#include "lora.h"

EventGroupHandle_t esp32_event_group = NULL;
SemaphoreHandle_t xMutex;

void lora_gw_receive(void *p)
{
   uint8_t lora_rx_buf[16];
   int x;
   for(;;) {
      if ( xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1 ){
         lora_disable_invertiq();
         lora_receive();    // put into receive mode
         while(lora_received()) {
            x = lora_receive_packet(lora_rx_buf, sizeof(lora_rx_buf));
            lora_rx_buf[x] = 0;
            printf("Received: %s\n", lora_rx_buf);
            printf("sizeof: %d\n", sizeof(lora_rx_buf));
            lora_receive();

            switch (decode_lora_msg(lora_rx_buf))
            {
            case 'a': mqtt_publish(gb_mqttClient, "node_a/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
               break;
            case 'b': mqtt_publish(gb_mqttClient, "node_b/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
               break;
            case 'c': mqtt_publish(gb_mqttClient, "node_c/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
               break;                           
            default:
               break;
            }
         }
         xSemaphoreGive(xMutex);
      }
      vTaskDelay(pdMS_TO_TICKS(100));
   }
}

void lora_gw_send(void *p)
{
   while(1){
      if ( xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1 ){
         lora_idle();
         lora_enable_invertiq();

         lora_send_packet((uint8_t*) p, sizeof(p));

         lora_disable_invertiq();
         lora_receive();

         xSemaphoreGive(xMutex);
      }
      vTaskDelay(pdMS_TO_TICKS(3000));
   }
}

void mqtt_tsk( void* p )
{
   while(1){
      xEventGroupWaitBits(esp32_event_group, MQTT_PUBLISHED_BIT       , true, true, portMAX_DELAY);
      xEventGroupWaitBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT, true, true, portMAX_DELAY);
      
      vTaskDelay(pdMS_TO_TICKS(500));
   }
}

// void node_publish_tsk( void *p )
// {
//    while (1)
//    {
//       uint32_t node_bits = xEventGroupWaitBits(esp32_event_group, MQTT_ALL_NODES, true, false, portMAX_DELAY);
      
//       if(node_bits && MQTT_NODE_A_PUBLISH == MQTT_NODE_A_PUBLISH){
//          mqtt_publish(gb_mqttClient, "node_a/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
//       }

//       if(node_bits && MQTT_NODE_A_PUBLISH == MQTT_NODE_B_PUBLISH){
//          mqtt_publish(gb_mqttClient, "node_b/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
//       }

//       if(node_bits && MQTT_NODE_A_PUBLISH == MQTT_NODE_C_PUBLISH){
//          mqtt_publish(gb_mqttClient, "node_c/temp", (char*) lora_rx_buf, strlen((char*) lora_rx_buf), 1, 0);
//       }

//       vTaskDelay(pdMS_TO_TICKS(500));      
//    }
   
// }

void app_main()
{
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();

   nvs_flash_init();
   wifi_conn_init();

   xMutex = xSemaphoreCreateMutex();

   xTaskCreatePinnedToCore(&lora_gw_receive,"lora_gw_receive", 2048, NULL  , 5, NULL, 1);

   //xTaskCreatePinnedToCore(&lora_gw_send,   "lora_gw_send"   , 2048, "gate", 5, NULL, 0);

   xTaskCreatePinnedToCore(&mqtt_tsk,       "mqtt_task"      , 2048, NULL  , 4, NULL, 0);
}