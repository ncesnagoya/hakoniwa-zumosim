set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-zumosim-runner
    LANGUAGES C CXX
)

set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})
MESSAGE(STATUS "HAKO_CPP_LIBDIR=" ${HAKO_CPP_LIBDIR})

add_executable(
    zumosim-runner
    zumosim_runner.cpp
)
target_link_libraries(zumosim-runner PRIVATE zumosim hakoarun hako pthread)
target_link_directories(zumosim-runner PRIVATE ${HAKO_CPP_LIBDIR})

target_include_directories(
    zumosim-runner 
    PRIVATE /usr/local/include
    PRIVATE /mingw64/include
    PRIVATE ${HAKO_ZUMO_SOURCE_DIR}/include
    PRIVATE ${HAKO_CPP_SOURCE_DIR}/src/hakoc/src
)

