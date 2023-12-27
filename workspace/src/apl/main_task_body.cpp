/************************************************************************************/
/*		FILE: main_task_body.cpp															*/
/*		Ver. 1.0																	*/
/*		DATE: 2023/12/26																*/
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

/****************************************************************************/
/* 関数名	: apl_main_task_setup()										　　*/
/* 機能		: プログラム起動時にハードウエアの初期化をしてZumoを初期位置に移動　*/
/*            起動時に1回だけ呼ばれる　　　　　　　　　　　　　　　　　　　　　*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/****************************************************************************/
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

/****************************************************************************/
/* 関数名	: apl_main_task_body()										　　*/
/* 機能		: 初期化が終了後、プログラムが終了するまで何度も呼ばれ続ける　　　　*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/****************************************************************************/
void apl_main_task_body(void)
{

    doOperation();  //　制御コマンドを一つ読みだしてそれに従いzumoを走行させる

    return;
}
