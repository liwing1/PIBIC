cd /D E:\liwka\Documents\esp-idf\projects\PIBIC\gateway_teste\build || exit /b
E:\liwka\.espressif\python_env\idf4.0_py2.7_env\Scripts\python.exe E:/liwka/Documents/esp-idf/tools/kconfig_new/confgen.py --kconfig E:/liwka/Documents/esp-idf/Kconfig --sdkconfig-rename E:/liwka/Documents/esp-idf/sdkconfig.rename --config E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/sdkconfig --env-file E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/build/config.env --env IDF_TARGET=esp32 --dont-write-deprecated --output config E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/sdkconfig || exit /b
E:\liwka\.espressif\tools\cmake\3.13.4\bin\cmake.exe -E env "COMPONENT_KCONFIGS=E:/liwka/Documents/esp-idf/components/app_trace/Kconfig E:/liwka/Documents/esp-idf/components/bt/Kconfig E:/liwka/Documents/esp-idf/components/driver/Kconfig E:/liwka/Documents/esp-idf/components/efuse/Kconfig E:/liwka/Documents/esp-idf/components/esp-tls/Kconfig E:/liwka/Documents/esp-idf/components/esp32/Kconfig E:/liwka/Documents/esp-idf/components/esp_adc_cal/Kconfig E:/liwka/Documents/esp-idf/components/esp_common/Kconfig E:/liwka/Documents/esp-idf/components/esp_eth/Kconfig E:/liwka/Documents/esp-idf/components/esp_event/Kconfig E:/liwka/Documents/esp-idf/components/esp_gdbstub/Kconfig E:/liwka/Documents/esp-idf/components/esp_http_client/Kconfig E:/liwka/Documents/esp-idf/components/esp_http_server/Kconfig E:/liwka/Documents/esp-idf/components/esp_https_ota/Kconfig E:/liwka/Documents/esp-idf/components/esp_https_server/Kconfig E:/liwka/Documents/esp-idf/components/esp_wifi/Kconfig E:/liwka/Documents/esp-idf/components/espcoredump/Kconfig E:/liwka/Documents/esp-idf/components/fatfs/Kconfig E:/liwka/Documents/esp-idf/components/freemodbus/Kconfig E:/liwka/Documents/esp-idf/components/freertos/Kconfig E:/liwka/Documents/esp-idf/components/heap/Kconfig E:/liwka/Documents/esp-idf/components/libsodium/Kconfig E:/liwka/Documents/esp-idf/components/log/Kconfig E:/liwka/Documents/esp-idf/components/lwip/Kconfig E:/liwka/Documents/esp-idf/components/mbedtls/Kconfig E:/liwka/Documents/esp-idf/components/mdns/Kconfig E:/liwka/Documents/esp-idf/components/mqtt/Kconfig E:/liwka/Documents/esp-idf/components/newlib/Kconfig E:/liwka/Documents/esp-idf/components/nvs_flash/Kconfig E:/liwka/Documents/esp-idf/components/openssl/Kconfig E:/liwka/Documents/esp-idf/components/pthread/Kconfig E:/liwka/Documents/esp-idf/components/spi_flash/Kconfig E:/liwka/Documents/esp-idf/components/spiffs/Kconfig E:/liwka/Documents/esp-idf/components/tcpip_adapter/Kconfig E:/liwka/Documents/esp-idf/components/unity/Kconfig E:/liwka/Documents/esp-idf/components/vfs/Kconfig E:/liwka/Documents/esp-idf/components/wear_levelling/Kconfig E:/liwka/Documents/esp-idf/components/wifi_provisioning/Kconfig E:/liwka/Documents/esp-idf/components/wpa_supplicant/Kconfig E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/lora/Kconfig E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/tarablessd1306/Kconfig" "COMPONENT_KCONFIGS_PROJBUILD=E:/liwka/Documents/esp-idf/components/app_update/Kconfig.projbuild E:/liwka/Documents/esp-idf/components/bootloader/Kconfig.projbuild E:/liwka/Documents/esp-idf/components/esptool_py/Kconfig.projbuild E:/liwka/Documents/esp-idf/components/partition_table/Kconfig.projbuild E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/main/Kconfig.projbuild" IDF_CMAKE=y KCONFIG_CONFIG=E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/sdkconfig IDF_TARGET=esp32 E:/liwka/.espressif/tools/mconf/v4.6.0.0-idf-20190628/mconf-idf.exe E:/liwka/Documents/esp-idf/Kconfig || exit /b
E:\liwka\.espressif\python_env\idf4.0_py2.7_env\Scripts\python.exe E:/liwka/Documents/esp-idf/tools/kconfig_new/confgen.py --kconfig E:/liwka/Documents/esp-idf/Kconfig --sdkconfig-rename E:/liwka/Documents/esp-idf/sdkconfig.rename --config E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/sdkconfig --env-file E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/build/config.env --env IDF_TARGET=esp32 --output config E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/sdkconfig || exit /b
