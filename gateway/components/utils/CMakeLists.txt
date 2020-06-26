idf_component_register(SRCS "mqttutils.c"
                    INCLUDE_DIRS "include"
                    REQUIRES espmqtt
                    REQUIRES lwip
                    REQUIRES nvs_flash
                    REQUIRES lora)
