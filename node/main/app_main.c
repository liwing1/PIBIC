#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lora.h"
#include "adcfix.h"

void lora_nd_receive(void *p)
{
   uint8_t buf[32];
   int x;
   for(;;){
      lora_enable_invertiq();
      lora_receive();    // put into receive mode
      while(lora_received()) {
         x = lora_receive_packet(buf, sizeof(buf));
         buf[x] = 0;
         printf("Received: %s\n", buf);
         lora_receive();
      }
      vTaskDelay(pdMS_TO_TICKS(100));
   }
}

void lora_nd_send(void *p)
{
   char mybuf[32];
   while(1){
      if(myQueue != 0){
         if(xQueueReceive(myQueue, (void*) mybuf, (TickType_t) 5))
         {
            lora_idle();
            lora_disable_invertiq();

            lora_send_packet((uint8_t*) mybuf, sizeof(mybuf));

            lora_enable_invertiq();
            lora_receive();

            printf("packet sent...\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
         }
      }
   }
}

void app_main()
{
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();

   xTaskCreatePinnedToCore(&GetAdcValue_Task, "getadc_tsk", 2048, NULL, 5, NULL, 1);

   xTaskCreatePinnedToCore(&lora_nd_receive, "lora_nd_receive", 2048, NULL, 5, NULL, 0);

   xTaskCreatePinnedToCore(&lora_nd_send, "lora_nd_send", 2048, NULL, 5, NULL, 1);
}