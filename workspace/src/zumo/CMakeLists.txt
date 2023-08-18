set(HAKO_CMAKE_VERSION ${HAKO_CMAKE_VERSION})

project(hakoniwa-core
    LANGUAGES C CXX
)

set(OS_TYPE "posix")
if(WIN32)
    set(OS_TYPE "win")
endif(WIN32)
MESSAGE(STATUS "OS_TYPE=" ${OS_TYPE})

add_library(
    zumosim STATIC
    actuator/zumo_led.cpp
    actuator/zumo_motor.cpp
    sensor/zumo_imu.cpp
    sensor/zumo_ir.cpp
    sensor/zumo_lf.cpp
)

target_include_directories(
    zumosim
    PRIVATE /usr/local/include
    PRIVATE /mingw64/include
    PRIVATE ${HAKO_ZUMO_SOURCE_DIR}/include
)

