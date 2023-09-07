#include "zumo_actuator.hpp"
#include "apl/zumosim_api.hpp"

void zumosim_led_ctrl(bool on)
{
    zumo_pdu_actuator.led.green = on;
}
