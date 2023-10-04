#include "zumo_actuator.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

void zumosim_led_ctrl(bool on)
{
    //std::cout << "led " << on << std::endl;
    zumo_pdu_actuator.led.green = on;
}
