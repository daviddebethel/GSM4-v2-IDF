set(COMPONENTS_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../components")
set(HOST_COMMON_DIR "${CMAKE_CURRENT_LIST_DIR}/../../common")

set(GSM4_EXAMPLE_COMPONENT_DIR "${COMPONENTS_DIR}/gsm4_example")
set(BSP_COMPONENT_DIR "${COMPONENTS_DIR}/bsp_gsm4_rev4")
set(KTS_COMPONENT_DIR "${COMPONENTS_DIR}/drv_kts1622")
set(BQ_COMPONENT_DIR "${COMPONENTS_DIR}/drv_bq25622")
set(I2C_BUS_COMPONENT_DIR "${COMPONENTS_DIR}/i2c_bus")

idf_component_register(
    SRCS
        "test_runner_unit.c"
        "test_fake_i2c_bus_unit.c"
        "${GSM4_EXAMPLE_COMPONENT_DIR}/test/test_gsm4_example_math.c"
        "${KTS_COMPONENT_DIR}/test/test_drv_kts1622.c"
        "${BQ_COMPONENT_DIR}/test/test_drv_bq25622.c"
        "${BSP_COMPONENT_DIR}/test/test_bsp_phase0_logic.c"
        "${HOST_COMMON_DIR}/fake_i2c_bus.c"
    INCLUDE_DIRS
        "."
        "${GSM4_EXAMPLE_COMPONENT_DIR}/include"
        "${BSP_COMPONENT_DIR}/include"
        "${KTS_COMPONENT_DIR}/include"
        "${BQ_COMPONENT_DIR}/include"
        "${I2C_BUS_COMPONENT_DIR}/include"
        "${HOST_COMMON_DIR}"
    PRIV_REQUIRES
        unity
        gsm4_example
        bsp_gsm4_rev4
        drv_kts1622
        drv_bq25622
        i2c_bus
)
