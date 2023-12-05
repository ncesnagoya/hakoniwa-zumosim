#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"

static IZumoLED& led = zumo_get_led();
static IZumoMotors& motors = zumo_get_motors();
static IZumoReflectanceSensorArray& reflectances = zumo_get_reflectance_sensor_array();
static IZumoCompass& compass = zumo_get_compass();
static IZumoSerial& Serial = zumo_get_serial();

int g_iStatus;			/* 状態遷移変数 */
int g_iDoCmdNum;		/* g_cCommand[]のコマンドを先頭から順に取り出す添え字 */

#define delay(arg) zumo_delay(arg)

#include "common.h" 
#include "Driver.ino"   /* Operation.inoより先にインクルードする必要がある */
#include "Operation.ino"    

void apl_main_task_setup(void)
{
    Serial.println("EVENT RESET");
 
    g_iStatus = STATUS_INIT;
    g_iDoCmdNum = 0;

    led = zumo_get_led();
    motors = zumo_get_motors();
    reflectances = zumo_get_reflectance_sensor_array();
    compass = zumo_get_compass();

    g_iStatus = STATUS_COMMAND_INPUT;

    getCommand();

}

void apl_main_task_body(void)
{
    // skip initial sensor data
    while (1) {
        if (reflectances.value(1) == ZUMOSIM_REFLECT_MAX_VALUE) {
            Serial.println("INFO: wait for arriving sensor data for sim");
            reflectances.update();
        }
        else {
            break;
        }
    }
    doOperation();
    return;
}
