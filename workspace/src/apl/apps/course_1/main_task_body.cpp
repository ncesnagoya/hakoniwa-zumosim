/************************************************************************************/
/*		FILE: main_task_body.cpp															*/
/*		Ver. 0.8																	*/
/*		DATE: 2023/12/19																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム										*/
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/
#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"

static IZumoLED& led = zumo_get_led();
static IZumoMotors& motors = zumo_get_motors();
static IZumoReflectanceSensorArray& reflectances = zumo_get_reflectance_sensor_array();
static IZumoCompass& compass = zumo_get_compass();
static IZumoSerial& Serial = zumo_get_serial();

// int g_iStatus;			/* 状態遷移変数 */

#define delay(arg) zumo_delay(arg)

#include "common.h" 
#include "Driver.ino"   /* Operation.inoより先にインクルードする必要がある */
#include "Operation.ino"    

void apl_main_task_setup(void)
{
    g_iDoCmdNum = 0;  //制御コマンド格納配列内のコマンド数を0にする

    //　センサ、アクチュエータの初期化
    led = zumo_get_led();           //LED
    motors = zumo_get_motors();     //モータ
    compass = zumo_get_compass();   //コンパス
    reflectances = zumo_get_reflectance_sensor_array();  // リフレクタセンサ
    while (1) {     
        if (reflectances.value(1) == ZUMOSIM_REFLECT_MAX_VALUE) {
            SerialPrintIn("INFO: wait for arriving sensor data for sim");
            reflectances.update();
        }
        else {
            break;
        }
    }

    getCommand();  // 制御コマンドの設定

	goStraight();  //直進して初期位置に移動する

    return;
}

void apl_main_task_body(void)
{

    //　制御コマンドに従いzumoを走行させる
    doOperation();

    return;
}
