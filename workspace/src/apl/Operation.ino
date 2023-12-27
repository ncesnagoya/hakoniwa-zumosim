/************************************************************************************/
/*		FILE: Operation.ino															*/
/*		Ver. 1.0																	*/
/*		DATE: 2023/12/26																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム										*/
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/
#include "common.h"
#include <string.h>

/************************/
/*		マクロ定義		*/
/************************/
/* モータ速度 */
#define STOP_SPEED					(0)						/* モーター停止回転速度					*/
#define STRAIGHT_SPEED				(120)					/* 要調整：前進時のモータ回転速度			*/
#define STRAIGHT_SPEED_1			(60)					/* 要調整：直進制御のためのモータ回転速度１	*/
#define STRAIGHT_SPEED_2			(0)						/* 要調整：直進制御のためのモータ回転速度２	*/
#define STRAIGHT_SPEED_3			(-60)					/* 要調整：直進制御のためのモータ回転速度３ */
#define TURN_SPEED					(120)					/* 要調整：旋回時のモータ走行速度			*/
#define TURN_SPEED_REVERSE			(-120)					/* 要調整：旋回時のモータ走行速度(逆回転)	*/

/* 待機時間 */
#define INTERSECTION_OSTIM			((unsigned long)400)	/* 要調整：横棒を検出後のzumo中心部を交差点中心まで移動させる msec	*/
#define TURN_OSTIM					((unsigned long)1650)	/* 要調整：交差点での90度回転時間 msec */
#define LED_FLASHCYCLE				((unsigned long)1000)	/* LEDの点滅周期 msec	*/

/* リフレクタセンサの閾値 */
#define REFLECTANCE_THRESHOLD		((unsigned int)400)		/* 要調整：白と黒を区別するリフレクタセンサの出力値(シミュレーション環境)	*/

/* コマンド入力配列と制御コマンド格納配列関連 */
#define INPUT_COMMAND_NUM			(50)					/* プログラム中に初期値としてコマンド入力配列に格納可能なコマンド数	*/
#define MAX_COMMAND					(10)					/* プログラム実行中に制御コマンド格納配列に格納可能なコマンド数 */

/* リフレクトタセンサ 黒線検出判定      */
#define ON_LINE(val)				((val) > REFLECTANCE_THRESHOLD)  /* リフレクトタセンサ 黒線検出判定      */
																	 /*　引数：リフレクトセンサ値            */
																	 /*　戻り値：0: 黒線未検出、 1: 黒線検出 */

/********************************/
/*		グローバル変数定義		*/
/********************************/
char g_cCommand[MAX_COMMAND];								/* 制御コマンド格納配列					*/
int g_iDoCmdNum;											/* 制御コマンド格納配列のインデックス	*/
int g_iDoCmdSet;											/* 格納されている制御コマンド数	*/
/* コマンド入力配列（初期値が与えられている。入力可能な文字数は　INPUT_COMMAND_NUM以下)	 */
#define TEST_INPUT_PTN				(4)						/* どのコマンド入力配列 のテストパターンを使うかを定義する */
#if TEST_INPUT_PTN == 0
char g_cInputCommand[INPUT_COMMAND_NUM] = "";					/* テストパターン0  */
#elif TEST_INPUT_PTN == 1
char g_cInputCommand[INPUT_COMMAND_NUM] = "frl.";				/* テストパターン1  */
#elif TEST_INPUT_PTN == 2
char g_cInputCommand[INPUT_COMMAND_NUM] = "FRL.";				/* テストパターン2  */
#elif TEST_INPUT_PTN == 3
char g_cInputCommand[INPUT_COMMAND_NUM] = "X.";					/* テストパターン3  */
#elif TEST_INPUT_PTN == 4
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "frrdfRlrrffr.";		/* テストパターン4  */
#elif TEST_INPUT_PTN == 5
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "frrDfRlrrffr.";		/* テストパターン5  */
#elif TEST_INPUT_PTN == 6
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "RRLLfllfllfrrr.";	/* テストパターン6  */
#endif


/****************************************************************************/
/* 関数名	: getCommand()													*/
/* 機能		: コマンド入力配列を読み出して制御コマンド格納配列に格納する		*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: なし															*/
/****************************************************************************/
void getCommand(void) {

	char cCmd;					/* 読み取ったコマンド							*/
	int iCmdNum = 0;			/* 制御コマンド格納配列に格納したコマンド数		*/
	int iInputCmdNum = 0;		/* コマンド入力配列から読み出したコマンド数		*/

	SerialPrintIn("Input Command.");  		/* コマンド入力を促すメッセージ出力 */

	/* コマンド入力配列が空の場合は読み取らずに終了　*/
	if(strlen(g_cInputCommand) == 0){
		g_iDoCmdSet = 0;
		return;
	}

	/* コマンド入力配列を読み出して制御コマンド格納配列に格納する */
	while (1) {
		if (iCmdNum >= MAX_COMMAND) {	/* 読み取り可能なコマンド数を超過 */

			SerialPrintIn("Over Max Command. ");	/* 入力超過メッセージを出力 */
			SerialPrintIn(&g_cCommand[0]);			/* 入力コマンドの出力 */
			break;  /* コマンド入力を終了する */
		}

		/* コマンド入力配列からコマンドを読み取る */
		cCmd = g_cInputCommand[iInputCmdNum];	
		iInputCmdNum++;

		/* コマンドに応じた処理 */
		if ( ((cCmd != 'r') && (cCmd != 'R')
		  &&  (cCmd != 'f') && (cCmd != 'F')
		  &&  (cCmd != 'l') && (cCmd != 'L')
		  &&  (cCmd != 'd') && (cCmd != 'D')
		  &&  (cCmd != '.'))) {					/* 制御コマンドでない場合は読み飛ばす */
			SerialPrintIn("Wrong Command!");	  /* 誤ったコマンドメッセージを出力 */
	    } else if (cCmd == 'd' || cCmd == 'D' ) {  /* 'd'/'D'コマンドの場合、それまで入力されたコマンドを全て削除する */
				iCmdNum = 0;  	/* 制御コマンド格納配列に格納したコマンド数をクリア */
				SerialPrintIn("Delete Command already enterd.");	/* コマンド削除メッセージを出力 */
		} else { /* d/D以外の制御コマンド */
			g_cCommand[iCmdNum] = cCmd;   /* 読み取ったコマンドを、制御コマンド格納配列に格納する */
			iCmdNum++;
			if (cCmd == '.') {   	/* '.'コマンド（＝以上コマンド） */
					SerialPrintIn(&g_cCommand[0]);	/* 入力コマンドの出力 */
					break;	/* コマンド入力を終了する */
			}
		}
	}
	
	g_iDoCmdSet = iCmdNum;		/* 有効な制御コマンド数 */

	return;
}

/****************************************************************************/
/* 関数名	: goStraight()													*/
/* 機能		: 次の交差点まで前進する										*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: リフレクタセンサを検出しながら方向を調整して黒線の上を走行する	*/
/****************************************************************************/
void goStraight(void) {

	int iCountONLINE;

	while (1) {
		ReflectancesUpdate();	/* リフレクタセンサ値更新 */

		/* 前進する */
		if(ON_LINE(g_reflectances_value3) && ON_LINE(g_reflectances_value4)){	/* センサ3と4が黒線に乗っている　*/
			MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED);								/* 左右のモーター速度を同一にして前進 */
		} else if(ON_LINE(g_reflectances_value3) && !ON_LINE(g_reflectances_value2) && !ON_LINE(g_reflectances_value4)) {		/* センサ3だけ黒線に乗っている */
			MotorsSetSpeeds(STRAIGHT_SPEED_1, STRAIGHT_SPEED);		/* 左側のモータ速度を少し遅くする */
		} else if(ON_LINE(g_reflectances_value4) && !ON_LINE(g_reflectances_value5) && !ON_LINE(g_reflectances_value3)) {			/* センサ4だけ黒線に乗っている */
			MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED_1);		/* 右側のモータ速度を少し遅くする */
		} else if((ON_LINE(g_reflectances_value2) && !ON_LINE(g_reflectances_value1) && !ON_LINE(g_reflectances_value3))
				    || (ON_LINE(g_reflectances_value2) && ON_LINE(g_reflectances_value3))) {		/* センサ2だけまたはセンサ2,3が黒線に乗っている */
			MotorsSetSpeeds(STRAIGHT_SPEED_2, STRAIGHT_SPEED);		/* 左側のモーター速度を中程度遅くする */
		} else if((ON_LINE(g_reflectances_value5)  && !ON_LINE(g_reflectances_value6) && !ON_LINE(g_reflectances_value4))
				    || (ON_LINE(g_reflectances_value4) && ON_LINE(g_reflectances_value5))) {		/* センサ5だけまたはセンサ4,5が黒線に乗っている */
			MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED_2);		/* 右側のモーター速度を中程度遅くする */
		} else if((ON_LINE(g_reflectances_value1) && !ON_LINE(g_reflectances_value2))
				    || (ON_LINE(g_reflectances_value1) && ON_LINE(g_reflectances_value2))) {			/* センサ1だけまたはセンサ1,2が黒線に乗っている左に大きく寄っている */
			MotorsSetSpeeds(STRAIGHT_SPEED_3, STRAIGHT_SPEED);		/* 左側のモータ速度を大きく遅くする */
		} else if((ON_LINE(g_reflectances_value6) && !ON_LINE(g_reflectances_value5))
				    || (ON_LINE(g_reflectances_value5) && ON_LINE(g_reflectances_value6))) {		/* センサ6だけまたはセンサ5,6が黒線に乗っている */
			MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED_3);		/* 右側のモータ速度を大きく遅くする */
		} else {											/* センサ1-6が全て黒を検出しない。白領域の走行 */
			MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED);	/* 左右のモーター速度を同一にして前進 */
		}

		/* 横棒検出 */
		iCountONLINE = 0;
		if(ON_LINE(g_reflectances_value1))iCountONLINE++;
		if(ON_LINE(g_reflectances_value2))iCountONLINE++;
		if(ON_LINE(g_reflectances_value3))iCountONLINE++;
		if(ON_LINE(g_reflectances_value4))iCountONLINE++;
		if(ON_LINE(g_reflectances_value5))iCountONLINE++;
		if(ON_LINE(g_reflectances_value6))iCountONLINE++;
		if(iCountONLINE >= 3){	/*　３つ以上のセンサが黒を検出 */
			break;
		}
	}
	
	/* zumoの中心を、交差点の中心まで進ませる */
	MotorsSetSpeeds(STRAIGHT_SPEED, STRAIGHT_SPEED);	/* 前進時のモータ速度設定 */
	delay(INTERSECTION_OSTIM);					/* 交差点中心までの走行時間 待機する */

	return;
}

/****************************************************************************/
/* 関数名	: doTurn()														*/
/* 機能		: 旋回して次の交差点まで直進する								*/
/* 引数		: cDir :  旋回方向												*/
/* 戻り値	: なし															*/
/* 備考		: 旋回後の前進処理は本関数で実施しない							*/
/* 			: 旋回方向 右旋回：'r', 'R'  左旋回：'l', 'L'					*/
/****************************************************************************/
void doTurn(char cDir) {

	switch (cDir) {
		/* 右旋回 */
		case 'r':
		case 'R':
			MotorsSetSpeeds(TURN_SPEED, TURN_SPEED_REVERSE);	/* 右側のモータ速度を逆回転に設定 */
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:右回転");
			#endif
			break;

		/* 左旋回 */ 
		case 'l':
		case 'L':
			MotorsSetSpeeds(TURN_SPEED_REVERSE, TURN_SPEED);	/* 左側のモータ速度を逆回転に設定 */
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:左回転");
			#endif
			break;

		default:
			/* 処理なし */
			break;
	}
	delay(TURN_OSTIM);				/* 90度回転するまで待つ */
	
	goStraight();	/* 次の交差点まで直進する */
	
	return;
}

/****************************************************************************/
/* 関数名	: doOperation()													*/
/* 機能		: 制御コマンド格納配列から走行コマンドを１つ取り出しZumoを動作させる	*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: なし															*/
/****************************************************************************/
void doOperation(void) {

	char cCmd;		/* 読み取った走行コマンド */

	if(g_iDoCmdNum >= g_iDoCmdSet){	/* 格納されている有効な制御コマンド数を超えたか */
		cCmd = '.';  /* 停止させる */
	} else {	/* 制御コマンド格納配列からコマンドを読み取る */
		cCmd = g_cCommand[g_iDoCmdNum];
		g_iDoCmdNum++;
	}

	#if SIM_DEBUG_CONSOLE != 0	
	SerialPrintIn("DEBUG:制御コマンド：");
	SerialPrintIn(&cCmd);
	#endif

	/* 制御コマンドに従ったZumoの走行 */
	if ( (cCmd == '.') || (g_iDoCmdNum >= MAX_COMMAND+1)) { 		/* 以上（=停止）コマンド または 実行コマンド数 超過 */ 
		MotorsSetSpeeds(STOP_SPEED, STOP_SPEED);	/* 停止時のモータ速度設定 */
		while (1) {					/* LEDをLED_FLASHCYCLE間隔で点滅続ける */
			LedOn();					/* LED点灯 */
			delay(LED_FLASHCYCLE);		/* LEDの点滅周期 待機する */
			LedOff();					/* LED消灯 */
			delay(LED_FLASHCYCLE);		/* LEDの点滅周期 待機する */
		}
	} else 	if ((cCmd == 'r') || (cCmd == 'R') 
			 || (cCmd == 'l') || (cCmd == 'L')) {  	/* 右折/左折コマンド */
		doTurn(cCmd);	/* 旋回処理 */
	} else {   										/* 直進コマンド */
		goStraight();	/* 次の交差点まで直進する */
	}
	
	return;
}