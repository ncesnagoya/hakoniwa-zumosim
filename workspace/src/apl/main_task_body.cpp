#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"

static int ccommand = 0;
static IZumoLED& led = zumo_get_led();
static IZumoMotors& motors = zumo_get_motors();
static IZumoReflectanceSensorArray& reflectances = zumo_get_reflectance_sensor_array();
static IZumoCompass& compass = zumo_get_compass();
static IZumoSerial& Serial = zumo_get_serial();

void apl_main_task_setup(void)
{
    Serial.println("EVENT RESET");
    ccommand = 0;
    led = zumo_get_led();
    motors = zumo_get_motors();
    reflectances = zumo_get_reflectance_sensor_array();
    compass = zumo_get_compass();
}
static const char *command = "rflflr.";
char g_cSeialSubs[100] = "frrDLF."; //テスト2　シリアル入力代替配列
#define MAX_COMMAND  10   // 制御コマンドの入力可能数
char g_cCommand[MAX_COMMAND]; //制御コマンドを記憶する配列

void doTurn(char dir);
void goStraight(void);
#define delay(arg) zumo_delay(arg)

#include "Operation.ino"

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
