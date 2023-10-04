#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

static bool led_status = false;
static IZumoLED& led = zumo_get_led();
static IZumoMotors& motors = zumo_get_motors();
static IZumoReflectanceSensorArray& rf_sensor = zumo_get_reflectance_sensor_array();
static IZumoCompass& compass = zumo_get_compass();

void apl_main_task_setup(void)
{
    led_status = false;
    led = zumo_get_led();
    motors = zumo_get_motors();
    rf_sensor = zumo_get_reflectance_sensor_array();
    compass = zumo_get_compass();
}

#include "Operation.ino"

void apl_main_task_body(void)
{
    doOperation();    
    return;
}
