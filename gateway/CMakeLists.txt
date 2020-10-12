# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway/components/lora)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway/components/espmqtt)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway/components/baseapp)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/gateway/components/tarablessd106)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lora-teste)