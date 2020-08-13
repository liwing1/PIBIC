#include <adcfix.h>

QueueHandle_t myQueue;

esp_adc_cal_characteristics_t characteristics;

void config_adc1(){

	adc1_config_width ( ADC_WIDTH_BIT_12 );
	adc1_config_channel_atten ( ADC1_CHANNEL_0, ADC_ATTEN_DB_11 );
	
	esp_adc_cal_characterize ( ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, V_REF, &characteristics);

}

void GetAdcValue_Task(){
	float gb_voltage = 0;
	char mybuf[16];
	myQueue = xQueueCreate(3, sizeof(mybuf));

	config_adc1();

	while(1){
		float voltage = 0;

		for( int i = 0; i < 100; i++){
			voltage += adc1_get_raw( ADC1_CHANNEL_0 );
			ets_delay_us( 30 );
		}
		voltage /= 100;
		voltage = esp_adc_cal_raw_to_voltage( voltage, &characteristics );

		if( voltage - gb_voltage >= 2.5 || voltage - gb_voltage <= -2.5)
		{
			ESP_LOGI("ADC CAL", "variation: %f", voltage-gb_voltage);
			gb_voltage = voltage;

			sprintf(mybuf, "%.1f", gb_voltage/10);
			xQueueSend(myQueue, (void*) mybuf, (TickType_t) 0);
		}
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}