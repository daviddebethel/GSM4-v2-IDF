set(GSM4_EXAMPLE_COMPONENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../components/gsm4_example")

idf_component_register(
    SRCS
        "test_runner_unit.c"
        "${GSM4_EXAMPLE_COMPONENT_DIR}/test/test_gsm4_example_math.c"
    INCLUDE_DIRS
        "${GSM4_EXAMPLE_COMPONENT_DIR}/include"
    PRIV_REQUIRES
        unity
        gsm4_example
)
