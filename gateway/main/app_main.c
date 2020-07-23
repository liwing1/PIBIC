#include "header.h"
#include "lora.h"

EventGroupHandle_t esp32_event_group = NULL;
SemaphoreHandle_t xMutex;
// QueueHandle_t rxQueue;

void lora_gw_receive(void *p)
{
   uint8_t buf[32];
   // rxQueue = xQueueCreate( 5, sizeof(buf) );

   int x;
   for(;;) {
      if ( xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1 ){
         lora_disable_invertiq();
         lora_receive();    // put into receive mode
         while(lora_received()) {
            x = lora_receive_packet(buf, sizeof(buf));
            buf[x] = 0;
            printf("Received: %s\n", buf);
            lora_receive();
         }
         // xQueueSend(rxQueue, (void*) buf, (TickType_t) 0);
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
      subscribe_cb(gb_mqttClient, NULL);

      vTaskDelay(pdMS_TO_TICKS(100));
   }
}

// void temp_update( void* p )
// {
//    uint8_t buf[32];
//    while(1){

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

   xTaskCreatePinnedToCore(&lora_gw_receive,"lora_gw_receive", 2048, NULL, 5, NULL, 1);

   xTaskCreatePinnedToCore(&lora_gw_send, "lora_gw_send", 2048, "gate", 5, NULL, 0);

   xTaskCreatePinnedToCore( mqtt_tsk, "mqtt_task",  2048, NULL, 4, NULL, 0);
}