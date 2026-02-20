cmake_minimum_required(VERSION 3.16)

set(COMPONENTS
    main
    gsm4_example
    bsp_gsm4_rev4
    drv_kts1622
    drv_bq25622
    i2c_bus
)

list(APPEND EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../../../components"
    "$ENV{IDF_PATH}/tools/mocks/freertos"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(gsm4_unit_host_test)
