set(GSM4_EXAMPLE_COMPONENT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../components/gsm4_example")

idf_component_register(
    SRCS
        "test_runner_mock.c"
        "${GSM4_EXAMPLE_COMPONENT_DIR}/test/test_gsm4_example_clock.c"
    INCLUDE_DIRS
        "${GSM4_EXAMPLE_COMPONENT_DIR}/include"
    PRIV_REQUIRES
        unity
        cmock
        gsm4_example
        esp_timer
)
