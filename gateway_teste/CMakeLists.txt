# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/lora)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/espmqtt)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/baseapp)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway_teste/components/tarablessd1306)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lora-teste)