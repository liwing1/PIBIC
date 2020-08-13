#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lora.h"
//#include "adcfix.h"
#include "DHT22.h"

SemaphoreHandle_t xMutex;
QueueHandle_t myQueue;

void lora_nd_receive(void *p)
{
   uint8_t buf[32];
   int x;
   for(;;){
      if( xSemaphoreTake(xMutex, (TickType_t)0xFFFFFFFF) ==1 ){
         lora_enable_invertiq();
         lora_receive();    // put into receive mode
         while(lora_received()) {
            x = lora_receive_packet(buf, sizeof(buf));
            buf[x] = 0;
            printf("Received: %s\n", buf);
            lora_receive();
         }

         xSemaphoreGive(xMutex);
      }
      vTaskDelay(pdMS_TO_TICKS(100));
   }
}

void lora_nd_send(void *p)
{
   char mybuf[16];
   while(1){
      if(myQueue != 0){
         if(xQueueReceive(myQueue, (void*) mybuf, (TickType_t) 5)){
            if( xSemaphoreTake(xMutex, (TickType_t)0xFFFFFFFF) == 1 ){
               lora_idle();
               lora_disable_invertiq();

               header_lora_msg( (uint8_t*)p, (uint8_t*) mybuf );
               lora_send_packet( (uint8_t*)mybuf, sizeof(mybuf) );
               printf("lora send: %s\n", mybuf);

               lora_enable_invertiq();
               lora_receive();
               
               xSemaphoreGive(xMutex);
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
         }
      }
   }
}

void DHT_task(void *pvParameter)
{
	setDHTgpio( 23 );
   char payload[16];
   myQueue = xQueueCreate(3, sizeof(payload));
   float gb_temp = 0, gb_humi = 0;

	while(1) {
      float temp = 0, humi = 0;
		int ret = readDHT();
		
		errorHandler(ret);

      for (int i = 0; i < 100; i++)
      {
         temp += getTemperature();
         humi += getHumidity();
      }
      temp = (temp/100);
      humi = (humi/100);
      
      if (abs(temp - gb_temp) >= 0.2 || abs(humi - gb_humi) >= 0.2)
      {
         gb_temp = temp;
         gb_humi = humi;

         sprintf(payload, "%.1f-%.1f\n", gb_temp, gb_humi);
         xQueueSend(myQueue, (void*) payload, (TickType_t) 0);         
      }

		vTaskDelay( pdMS_TO_TICKS(5000) );
	}
}

void app_main()
{
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();
   
   xMutex = xSemaphoreCreateMutex();

   xTaskCreatePinnedToCore(&DHT_task, "DHT_tsk", 2048, NULL, 5, NULL, 1);

   xTaskCreatePinnedToCore(&lora_nd_receive, "lora_nd_receive", 2048, NULL, 5, NULL, 0);

   xTaskCreatePinnedToCore(&lora_nd_send, "lora_nd_send", 2048, "b", 5, NULL, 1);
}