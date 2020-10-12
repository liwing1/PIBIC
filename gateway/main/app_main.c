#include "header.h"
#include "lora.h"
#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"


EventGroupHandle_t esp32_event_group = NULL;
SemaphoreHandle_t xMutex;

QueueHandle_t rxQueue;


// static const int I2CDisplayAddress = 0x3C;
// static const int I2CDisplayWidth = 128;
// static const int I2CDisplayHeight = 32;
// static const int I2CResetPin = 16;

// struct SSD1306_Device I2CDisplay;


// bool DefaultBusInit( void ) {
//    assert( SSD1306_I2CMasterInitDefault( ) == true );
//    assert( SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true );

//    return true;
// }

// void SetupDemo( struct SSD1306_Device* DisplayHandle, const struct SSD1306_FontDef* Font ) {
//    SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
//    SSD1306_SetFont( DisplayHandle, Font );
// }

// void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText ) {
//    SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
//    SSD1306_FontDrawAnchoredString( DisplayHandle, TextAnchor_Center, HelloText, SSD_COLOR_WHITE );
//    SSD1306_Update( DisplayHandle );
// }


void lora_gw_receive(void *p)
{
   uint8_t lora_rx_buf[16];
   rxQueue = xQueueCreate(5, sizeof(lora_rx_buf));
   int x;
   for(;;) {
      if ( xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1 ){
         lora_disable_invertiq();
         lora_receive();
         while(lora_received()) {
            x = lora_receive_packet(lora_rx_buf, sizeof(lora_rx_buf));
            lora_rx_buf[x] = 0;
            printf("Received: %s\n", lora_rx_buf);
            lora_receive();
            xQueueSend(rxQueue, (void*)lora_rx_buf, (TickType_t) 0);
            xEventGroupSetBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT);
         }
         xSemaphoreGive(xMutex);
         
      }
      vTaskDelay(pdMS_TO_TICKS(100));
   }
}

void lora_gw_send(void *p)
{
   if ( xSemaphoreTake(xMutex, (TickType_t) 0xFFFFFFFF) == 1 ){
      lora_idle();
      lora_enable_invertiq();

      lora_send_packet((uint8_t*) p, sizeof(p));

      lora_disable_invertiq();
      lora_receive();

      xSemaphoreGive(xMutex);
   }
}

void mqtt_tsk( void* p )
{
   while(1){
      // xEventGroupWaitBits(esp32_event_group, MQTT_PUBLISHED_BIT       , true, true, portMAX_DELAY);
      xEventGroupWaitBits(esp32_event_group, MQTT_INITIATE_PUBLISH_BIT, true, true, portMAX_DELAY);
      mqtt_update();
      
      vTaskDelay(pdMS_TO_TICKS(500));
   }
}

void app_main()
{
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();

   nvs_flash_init();
   wifi_conn_init();

   // if ( DefaultBusInit( ) == true ) {
   //    printf( "BUS Init lookin good...\n" );
   //    SetupDemo( &I2CDisplay, &Font_liberation_mono_17x30 );
   //    SayHello( &I2CDisplay, "Gatway" );
   // }

   xMutex = xSemaphoreCreateMutex();

   xTaskCreatePinnedToCore(&lora_gw_receive,"lora_gw_receive", 2048, NULL  , 5, NULL, 0);

   xTaskCreatePinnedToCore(&mqtt_tsk,       "mqtt_task"      , 2048, NULL  , 4, NULL, 1);
}