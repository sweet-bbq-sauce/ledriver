cmake_minimum_required(VERSION 3.22)

set(COMPONENTS
    main
)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/../../components/ledriver_protocol"
)

include("$ENV{IDF_PATH}/tools/cmake/project.cmake")
project(ledriver_protocol_tests)
