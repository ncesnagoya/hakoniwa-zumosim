#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

static int ccommand = 0;
static IZumoLED& led = zumo_get_led();
static IZumoMotors& motors = zumo_get_motors();
static IZumoReflectanceSensorArray& reflectances = zumo_get_reflectance_sensor_array();
static IZumoCompass& compass = zumo_get_compass();

void apl_main_task_setup(void)
{
    std::cout << "EVENT RESET" << std::endl;
    ccommand = 0;
    led = zumo_get_led();
    motors = zumo_get_motors();
    reflectances = zumo_get_reflectance_sensor_array();
    compass = zumo_get_compass();
}
static const char *command = "rflflr.";
void doTurn(char dir);
void goStraight(void);
#define delay(arg) zumo_delay(arg)

#include "Operation.ino"

void apl_main_task_body(void)
{
    while (1) {
        if (reflectances.value(1) == 0) {
            reflectances.update();
            continue;
        }
        break;
    }
    std::cout << "apl_main_task_body: enter" << std::endl;
    doOperation();    
    return;
}
