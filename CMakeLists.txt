cmake_minimum_required(VERSION 3.22)

include("$ENV{IDF_PATH}/tools/cmake/project.cmake")
project(ledriver)

set_property(
    DIRECTORY
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/version.json"
)

file(
    READ "${PROJECT_SOURCE_DIR}/version.json"
    VERSION_JSON
)

string(
    JSON LEDRIVER_HARDWARE_VERSION
    GET "${VERSION_JSON}" hardware_version
)
string(
    JSON LEDRIVER_FIRMWARE_BUILD_NUMBER
    GET "${VERSION_JSON}" build_number firmware
)
string(
    JSON LEDRIVER_WEBPANEL_BUILD_NUMBER
    GET "${VERSION_JSON}" build_number webpanel
)

message(STATUS "LEDriver hardware version: ${LEDRIVER_HARDWARE_VERSION}")
message(STATUS "LEDriver firmware build number: ${LEDRIVER_FIRMWARE_BUILD_NUMBER}")
message(STATUS "LEDriver webpanel build number: ${LEDRIVER_WEBPANEL_BUILD_NUMBER}")

file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/webpanel/dist")
file(WRITE "${PROJECT_SOURCE_DIR}/webpanel/dist/build_number" ${LEDRIVER_WEBPANEL_BUILD_NUMBER})

configure_file(
    "${PROJECT_SOURCE_DIR}/components/ledriver_device/device_info.h.in"
    "${PROJECT_SOURCE_DIR}/components/ledriver_device/include/ledriver/device_info.h"
    @ONLY
)

foreach(BUILD_NUMBER
    LEDRIVER_FIRMWARE_BUILD_NUMBER
    LEDRIVER_WEBPANEL_BUILD_NUMBER
)
    if(NOT "${${BUILD_NUMBER}}" MATCHES "^[1-9][0-9]*$")
        message(FATAL_ERROR "${BUILD_NUMBER} must be a natural number greater than 0")
    endif()
endforeach()

include("${PROJECT_SOURCE_DIR}/cmake/webpanel.cmake")
