idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES bsp_gsm4_rev4
)

set(GSM4_WEB_DIST_DIR "${CMAKE_CURRENT_LIST_DIR}/../../web/dist")
set(GSM4_WEB_FALLBACK_DIR "${CMAKE_CURRENT_LIST_DIR}/../web_fallback")

if(EXISTS "${GSM4_WEB_DIST_DIR}")
    set(GSM4_WEB_FS_SOURCE_DIR "${GSM4_WEB_DIST_DIR}")
else()
    set(GSM4_WEB_FS_SOURCE_DIR "${GSM4_WEB_FALLBACK_DIR}")
endif()

if(NOT EXISTS "${GSM4_WEB_FS_SOURCE_DIR}")
    message(FATAL_ERROR "Missing web filesystem source directory: ${GSM4_WEB_FS_SOURCE_DIR}")
endif()

message(STATUS "GSM4 web filesystem source: ${GSM4_WEB_FS_SOURCE_DIR}")
fatfs_create_spiflash_image(webfs "${GSM4_WEB_FS_SOURCE_DIR}" FLASH_IN_PROJECT PRESERVE_TIME)
