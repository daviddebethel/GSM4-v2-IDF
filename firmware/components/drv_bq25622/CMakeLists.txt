idf_component_register(
    SRCS "src/drv_bq25622.c"
    INCLUDE_DIRS "include"
    REQUIRES i2c_bus
)
