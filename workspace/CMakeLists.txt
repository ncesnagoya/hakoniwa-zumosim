set(HAKO_CMAKE_VERSION 3.5.1)
cmake_minimum_required(VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-suzmosim
    VERSION 1.0.0
    DESCRIPTION "hakoniwa-suzmosim project"
    LANGUAGES C CXX
)

enable_testing()

set(HAKO_ZUMO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/zumo")

if (DEFINED HAKO_CLIENT_OPTION_FILEPATH)
    include(${HAKO_CLIENT_OPTION_FILEPATH})
else()
    include(default-cmake-options.cmake)
endif()

message(STATUS "BUILD_TYPE" ${BUILD_TYPE})
message(STATUS "GCOV" ${GCOV})

add_subdirectory(src/zumo)
