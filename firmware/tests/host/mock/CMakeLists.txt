cmake_minimum_required(VERSION 3.16)

set(COMPONENTS main gsm4_example esp_timer)
list(APPEND EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../../../components/gsm4_example"
    "$ENV{IDF_PATH}/tools/mocks/freertos"
    "$ENV{IDF_PATH}/tools/mocks/esp_timer")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(gsm4_mock_host_test)
