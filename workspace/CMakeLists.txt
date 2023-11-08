set(HAKO_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-zumosim
    VERSION 1.0.0
    DESCRIPTION "hakoniwa-zumosim project"
    LANGUAGES C CXX
)

enable_testing()

set(ZUMO_INTERFACE_DIR "${PROJECT_SOURCE_DIR}/../hakoniwa-zumoIf/include")

set(HAKO_ZUMO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/zumo")
set(HAKO_APL_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/apl")
message(STATUS "INF=" ${ZUMO_INTERFACE_DIR})
if (DEFINED HAKO_CPP_SOURCE_DIR)
    message(STATUS "HAKO_CPP_SOURCE_DIR" ${HAKO_CPP_SOURCE_DIR})
else()
    set(HAKO_CPP_SOURCE_DIR "/root/hakoniwa-core-cpp-client")
endif()

set(HAKO_CPP_LIBDIR "/usr/local/lib/hakoniwa")

if (DEFINED HAKO_CLIENT_OPTION_FILEPATH)
    include(${HAKO_CLIENT_OPTION_FILEPATH})
else()
    include(default-cmake-options.cmake)
endif()

message(STATUS "BUILD_TYPE" ${BUILD_TYPE})
message(STATUS "GCOV" ${GCOV})

add_subdirectory(src/apl)
add_subdirectory(src/zumo)
add_subdirectory(src/runner)
