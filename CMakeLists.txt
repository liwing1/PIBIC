# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp/mqtt_test/components/espmqtt)
set(EXTRA_COMPONENT_DIRS E:/liwka/Documents/esp/mqtt_test/components/htu21d)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mqtt)