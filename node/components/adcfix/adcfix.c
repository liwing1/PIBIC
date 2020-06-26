#include <adcfix.h>

QueueHandle_t myQueue;

esp_adc_cal_characteristics_t characteristics;

void config_adc1(){

	adc1_config_width ( ADC_WIDTH_BIT_12 );
	adc1_config_channel_atten ( ADC1_CHANNEL_0, ADC_ATTEN_DB_11 );
	
	esp_adc_cal_characterize ( ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &characteristics);

}

void GetAdcValue_Task(){

	char mybuf[32];
	myQueue = xQueueCreate(2, sizeof(mybuf));

	config_adc1();

	while(1){
		uint32_t voltage = 0, gb_voltage = 0;

		for( int i = 0; i < 100; i++){
			voltage += adc1_get_raw( ADC1_CHANNEL_0 );
			ets_delay_us( 30 );
		}
		voltage /= 100;
		voltage = esp_adc_cal_raw_to_voltage( voltage, &characteristics );

		if( gb_voltage - voltage >= 5 || gb_voltage - voltage >= -5)
		{
			gb_voltage = voltage;
			ESP_LOGI("ADC CAL", "Read mV: %u", voltage);

			sprintf(mybuf, "%d", gb_voltage);
			xQueueSend(myQueue, (void*) mybuf, (TickType_t) 0);
		}
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}