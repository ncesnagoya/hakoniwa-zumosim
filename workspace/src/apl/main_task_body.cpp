#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

static bool led_status = false;

void apl_main_task_setup(void)
{
    led_status = false;
}

void apl_main_task_body(void)
{
    IZumoLED& led = zumo_get_led();
    IZumoReflectanceSensorArray& rf_sensor = zumo_get_reflectance_sensor_array();

    if (led_status) {
        led_status = false;
        led.off();
    }
    else {
        led_status = true;
        led.on();
    }
    rf_sensor.update();
    int i;
    for (i = 0; i < ZUMOSIM_REFLECT_SENSOR_NUM; i++) {

        std::cout << "[" << i << "] = " << rf_sensor.value(i) << std::endl;
    }
    zumo_delay(2000);
    return;
}