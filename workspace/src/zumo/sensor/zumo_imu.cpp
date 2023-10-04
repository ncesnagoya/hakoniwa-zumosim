#include "zumo_sensor.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

float zumosim_mag_value()
{
    return (float)zumo_pdu_sensor.imu.mag.z;
}
