#ifndef __ADCFIX_H__
#define __ADCFIX_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"


#include <driver/adc.h>
#include <esp_adc_cal.h>

#define V_REF               1100

float GetAdcValue_Task();
void config_adc1();

extern uint32_t gb_voltage;
extern QueueHandle_t myQueue;

#endif //__ADCFIX_H__