# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/node/components/lora)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/node/components/adcfix)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp-idf/projects/PIBIC/node/components/dht)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lora-teste)