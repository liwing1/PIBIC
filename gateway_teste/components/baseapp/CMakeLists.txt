idf_component_register(SRCS "mqttutils.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                    REQUIRES esp_adc_cal
                    REQUIRES nvs_flash
                    REQUIRES espmqtt)
