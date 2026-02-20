set(BSP_REQUIRES
    drv_kts1622
    drv_bq25622
    i2c_bus
)

if(NOT "${IDF_TARGET}" STREQUAL "linux")
    list(APPEND BSP_REQUIRES driver)
endif()

idf_component_register(
    SRCS
        "src/bsp_gsm4_rev4.c"
        "src/bsp_init.c"
        "src/bsp_gpio.c"
        "src/bsp_i2c.c"
        "src/bsp_interrupts.c"
    INCLUDE_DIRS "include"
    REQUIRES ${BSP_REQUIRES}
)
