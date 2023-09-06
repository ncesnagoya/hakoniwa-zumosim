set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-zumo
    LANGUAGES C CXX
)

set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

add_library(
    zumoapl STATIC
    main_task_body.cpp
)

target_include_directories(
    zumoapl
    PRIVATE /usr/local/include
    PRIVATE /mingw64/include
    PRIVATE ${HAKO_APL_SOURCE_DIR}
    PRIVATE ${HAKO_ZUMO_SOURCE_DIR}/include
)

