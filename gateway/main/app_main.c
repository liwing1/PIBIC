#include <header.h>

QueueHandle_t rxQueue;

void lora_gw_receive(void *p)
{
   uint8_t buf[32];
   rxQueue = xQueueCreate(3, sizeof(buf));

   int x;
   for(;;) {
      lora_disable_invertiq();
      lora_receive();    // put into receive mode
      while(lora_received()) {
         x = lora_receive_packet(buf, sizeof(buf));
         buf[x] = 0;
         printf("Received: %s\n", buf);
         lora_receive();
      }
      xQueueSend(rxQueue, (void*) buf, (TickType_t) 0);
      vTaskDelay(pdMS_TO_TICKS(10));
   }
}

void app_main()
{
   nvs_flash_init();
   wifi_conn_init();

   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();

   xTaskCreatePinnedToCore(
      &lora_gw_receive,
      "lora_gw_receive",
      configMINIMAL_STACK_SIZE * 3,
      NULL,
      tskIDLE_PRIORITY + 5,
      NULL,
      0
   );
}