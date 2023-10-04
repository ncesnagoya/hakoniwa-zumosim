#include "zumo_actuator.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>


void zumosim_motor_ctrl(ZumoSimMotorDirType dir, int power)
{
    zumo_pdu_actuator.motors[dir].power = power;
}
