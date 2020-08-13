idf_component_register(SRCS "adcfix.c"
                    INCLUDE_DIRS "include"
		            REQUIRES esp_adc_cal
                    REQUIRES driver)
