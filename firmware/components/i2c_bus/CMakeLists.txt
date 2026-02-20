set(I2C_BUS_REQUIRES "")
if(NOT "${IDF_TARGET}" STREQUAL "linux")
    list(APPEND I2C_BUS_REQUIRES driver)
endif()

idf_component_register(
    SRCS "src/i2c_bus_idf.c"
    INCLUDE_DIRS "include"
    REQUIRES ${I2C_BUS_REQUIRES}
)
