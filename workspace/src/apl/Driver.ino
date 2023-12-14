/************************************************************************************/
/*		FILE: Driver.ino															*/
/*		Ver. 1.4																	*/
/*		ATE: 2023/12/08																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム										*/
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/
#include "common.h"

#if	SIMULATION_ENV == 0
/* シミュレーション環境はインクルード不要 */
#include <Wire.h>
#include <ZumoShield.h>
#endif

/************************/
/*		マクロ定義		*/
/************************/
/* ブザー */
#define	BUZZER_PLAY_CNT				(1)					/* ブザー吹鳴時間 sec */

/* リフレクタセンサ */
#define	REFLECTA_1					(1)					/* 1番目のリフレクタセンサ */
#define	REFLECTA_2					(2)					/* 2番目のリフレクタセンサ */
#define	REFLECTA_3					(3)					/* 3番目のリフレクタセンサ */
#define	REFLECTA_4					(4)					/* 4番目のリフレクタセンサ */
#define	REFLECTA_5					(5)					/* 5番目のリフレクタセンサ */
#define	REFLECTA_6					(6)					/* 6番目のリフレクタセンサ */

/* モータ速度 */
#define MOTORSPEED_MIN				(-400)				/* モータ速度 下限値 */
#define MOTORSPEED_MAX				(400)				/* モータ速度 上限値 */

/********************************/
/*		グローバル変数定義		*/
/********************************/
unsigned int	g_reflectances_value1;					/* 1番目のリフレクタセンサ値 */
unsigned int	g_reflectances_value2;					/* 2番目のリフレクタセンサ値 */
unsigned int	g_reflectances_value3;					/* 3番目のリフレクタセンサ値 */
unsigned int	g_reflectances_value4;					/* 4番目のリフレクタセンサ値 */
unsigned int	g_reflectances_value5;					/* 5番目のリフレクタセンサ値 */
unsigned int	g_reflectances_value6;					/* 6番目のリフレクタセンサ値 */

#if	SIMULATION_ENV == 0
/********************************/
/*		関数: PUSHBUTTON		*/
/********************************/
/****************************************************************************/
/* 関数名	: PushbuttonWaitForPress()										*/
/* 機能		: プッシュボタンが押下されるまで待機する						*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/****************************************************************************/
void PushbuttonWaitForPress(void) {

	button.waitForPress();

	return;
}

/********************************/
/*		関数: ブザー			*/
/********************************/
/****************************************************************************/
/* 関数名	: BuzzerInit()													*/
/* 機能		: ブザーを初期化する											*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/****************************************************************************/
void BuzzerInit(void) {

	buzzer.playOn();						/* ブザーの初期化 */

	return;
}

/****************************************************************************/
/* 関数名	: BuzzerPlay()													*/
/* 機能		: ブザーを吹鳴させる											*/
/* 引数		: なし  														*/
/* 戻り値	: なし															*/
/****************************************************************************/
void BuzzerPlay(void) {

	buzzer.playNum(BUZZER_PLAY_CNT);		/* ブザーを鳴らす */

	return;
}
#endif

/********************************/
/*		関数: LED				*/
/********************************/
/****************************************************************************/
/* 関数名	: LedOff()														*/
/* 機能		: LEDを消灯させる												*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/****************************************************************************/
void LedOff(void) {

	led.off();						/* 消灯 */

	return;
}

/****************************************************************************/
/* 関数名	: LedOn()														*/
/* 機能		: LEDを点灯させる												*/
/* 引数		: なし  														*/
/* 戻り値	: なし															*/
/****************************************************************************/
void LedOn(void) {

	led.on();						/* 点灯 */

	return;
}

/********************************/
/*		関数: モータ			*/
/********************************/
/****************************************************************************/
/* 関数名	: MotorsSetSpeeds()						 						*/
/* 機能		: モータ速度を設定する											*/
/* 引数		: leftspeed  : 左側のモータ速度 (-400～400)						*/
/*  		: rightspeed : 右側のモータ速度 (-400～400)						*/
/* 戻り値	: なし															*/
/****************************************************************************/
void MotorsSetSpeeds(int leftspeed, int rightspeed) {

	/* 引数上下限ガード(leftspeed) */
	if (leftspeed < MOTORSPEED_MIN) {
		leftspeed = MOTORSPEED_MIN;
	} else if (leftspeed > MOTORSPEED_MAX) {
		leftspeed = MOTORSPEED_MAX;
	} else {
		/* 何もしない */
	}

	/* 引数上下限ガード(rightspeed) */
	if (rightspeed < MOTORSPEED_MIN) {
		rightspeed = MOTORSPEED_MIN;
	} else if (rightspeed > MOTORSPEED_MAX) {
		rightspeed = MOTORSPEED_MAX;
	} else {
		/* 何もしない */
	}

	motors.setSpeeds(leftspeed, rightspeed);	/* モータ速度設定 */

	return;
}

/********************************/
/*		関数: リフレクタセンサ	*/
/********************************/
/****************************************************************************/
/* 関数名	: ReflectancesUpdate()											*/
/* 機能		: リフレクタセンサ値を更新する  								*/
/* 引数		: なし  														*/
/* 戻り値	: なし															*/
/****************************************************************************/
void ReflectancesUpdate(void) {

	reflectances.update();				/* リフレクタセンサ更新 */

	/* リフレクタセンサ 公開変数 更新 */
	g_reflectances_value1 = reflectances.value(REFLECTA_1);
	g_reflectances_value2 = reflectances.value(REFLECTA_2);
	g_reflectances_value3 = reflectances.value(REFLECTA_3);
	g_reflectances_value4 = reflectances.value(REFLECTA_4);
	g_reflectances_value5 = reflectances.value(REFLECTA_5);
	g_reflectances_value6 = reflectances.value(REFLECTA_6);

	return;
}

/********************************/
/*		関数: シリアル通信		*/
/********************************/
#if	SIMULATION_ENV == 0
/****************************************************************************/
/* 関数名	: SerialBegin()													*/
/* 機能		: シリアル通信の初期化をする									*/
/* 引数		: speed：転送レート 											*/
/* 戻り値	: なし															*/
/****************************************************************************/
void SerialBegin(int speed) {

	Serial.begin(speed);				/* シリアル通信のデータ転送レートを設定 */

	return;
}
#endif

/****************************************************************************/
/* 関数名	: SerialPrintIn()												*/
/* 機能		: シリアルモニタ出力をする(改行あり)							*/
/* 引数		: data[]: 出力データ											*/
/* 戻り値	: なし															*/
/****************************************************************************/
void SerialPrintIn(const char data[]) {

    (void)Serial.println(&data[0]);

    return;
}
