idf_component_register(SRCS "mqtt.c" "mqtt_msg.c" "ringbuf.c"
                    INCLUDE_DIRS "include"
                    REQUIRES openssl)
