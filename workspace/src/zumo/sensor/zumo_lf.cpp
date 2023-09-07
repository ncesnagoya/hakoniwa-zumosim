#include "zumo_sensor.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

bool zumosim_reflect_values(unsigned int values[ZUMOSIM_REFLECT_SENSOR_NUM])
{
    if (values == NULL) {
        return false;
    }
    int i;
    for (i = 0; i < ZUMOSIM_REFLECT_SENSOR_NUM; i++) {
        values[i] = zumo_pdu_sensor.line_sensors[i].brightness;
    }
    return true;
}
