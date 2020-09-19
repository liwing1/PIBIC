#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lora.h"
//#include "adcfix.h"
#include "DHT22.h"
#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"
#include "driver/gpio.h"

#define  CNT_ON    gpio_set_level(GPIO_NUM_17, 1);
#define  CNT_OFF   gpio_set_level(GPIO_NUM_17, 0);


static const int I2CDisplayAddress = 0x3C;
static const int I2CDisplayWidth = 128;
static const int I2CDisplayHeight = 32;
static const int I2CResetPin = 16;

struct SSD1306_Device I2CDisplay;


SemaphoreHandle_t xMutex;
QueueHandle_t myQueue;

uint32_t gb_temp_ref;


bool DefaultBusInit( void ) {
   assert( SSD1306_I2CMasterInitDefault( ) == true );
   assert( SSD1306_I2CMasterAttachDisplayDefault( &I2CDisplay, I2CDisplayWidth, I2CDisplayHeight, I2CDisplayAddress, I2CResetPin ) == true );

   return true;
}

void SetupDemo( struct SSD1306_Device* DisplayHandle, const struct SSD1306_FontDef* Font ) {
   SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
   SSD1306_SetFont( DisplayHandle, Font );
}

void SayHello( struct SSD1306_Device* DisplayHandle, const char* HelloText ) {
   SSD1306_Clear( DisplayHandle, SSD_COLOR_BLACK );
   SSD1306_FontDrawAnchoredString( DisplayHandle, TextAnchor_Center, HelloText, SSD_COLOR_WHITE );
   SSD1306_Update( DisplayHandle );
}


void DecodeGateWayMessage( char* buf ){
   if(strstr(buf, "ON")){
      printf("RELE ON\n");
      CNT_ON;
   }
   else if(strstr(buf, "OFF")){
      printf("RELE OFF\n");
      gb_temp_ref = 0;
      CNT_OFF;
   }
   else{
      gb_temp_ref = atoi(buf);
   }
}

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
            DecodeGateWayMessage( (char*)buf );
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
   char stemp[16];
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
         sprintf(stemp, "T=%.1f", gb_temp);
         xQueueSend(myQueue, (void*) payload, (TickType_t) 0);
         SayHello( &I2CDisplay, stemp );
      }

      if (gb_temp_ref){
         if(gb_temp > gb_temp_ref + 0.2) CNT_ON;
         if(gb_temp < gb_temp_ref - 0.2) CNT_OFF;
      }

		vTaskDelay( pdMS_TO_TICKS(5000) );
	}
}

void app_main()
{
   lora_init();
   lora_set_frequency(915e6);
   lora_enable_crc();
   
   gpio_pad_select_gpio(GPIO_NUM_17); 
   gpio_set_direction(GPIO_NUM_17,GPIO_MODE_OUTPUT);

   xMutex = xSemaphoreCreateMutex();

   if ( DefaultBusInit( ) == true ) {
      printf( "BUS Init lookin good...\n" );
      SetupDemo( &I2CDisplay, &Font_liberation_mono_17x30 );
   }

   xTaskCreatePinnedToCore(&DHT_task, "DHT_tsk", 2048, NULL, 5, NULL, 1);

   xTaskCreatePinnedToCore(&lora_nd_receive, "lora_nd_receive", 2048, NULL, 5, NULL, 0);

   xTaskCreatePinnedToCore(&lora_nd_send, "lora_nd_send", 2048, "a", 5, NULL, 1);
}