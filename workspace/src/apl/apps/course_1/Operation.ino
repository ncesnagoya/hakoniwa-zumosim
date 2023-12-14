/************************************************************************************/
/*		FILE: Operation.ino															*/
/*		Ver. 1.4																	*/
/*		ATE: 2023/12/08																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム										*/
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/
#include "common.h"

/************************/
/*		マクロ定義		*/
/************************/
#define DEBUG_MOTOR_SPEED			(0)						/* 0:運用時のモータ速度 / 0以外:デバッグ時のモータ速度 */
#define TEST_INPUT_PTN				(8)						/* コマンド入力配列 1:テスト1 / 2:テスト2 / 3:テスト3... */
#define DOTURN_SENSOR_RIGHT_LEFT	(0)						/* 0:回転方向のセンサでエッジを検出 / 0以外:T字路対応のため左右のセンサでエッジを検出 */

/* モータ速度 */
#define STOP_SPEED					(0)						/* モーター停止速度					*/
#define MOTOR_SPEED					(120)					/* 前進時のモータ走行速度			*/
#define MOTOR_SPEED_REVERSE			(-120)					/* 前進時のモータ走行速度(逆回転)	*/

#if DEBUG_MOTOR_SPEED == 0
/* 運用時のモータ速度 */
#define TURN_SPEED					(120)					/* 旋回時のモータ走行速度			*/
#define TURN_SPEED_REVERSE			(-120)					/* 旋回時のモータ走行速度(逆回転)	*/
#else
/* デバッグ時のモータ速度 */
#define TURN_SPEED					(240)					/* 旋回時のモータ走行速度			*/
#define TURN_SPEED_REVERSE			(-240)					/* 旋回時のモータ走行速度(逆回転)	*/
#endif

/* 回転時のエッジ検出回数 */
#define TURN_EDGE_CNT				(2)						/* 旋回時のエッジ検出回数			*/

/* 待機時間 */
#define INTERSECTION_OSTIM			((unsigned long)350)	/* 交差点の横棒を検出してから交差点中心までの走行時間 msec	*/
#define LED_FLASHCYCLE				((unsigned long)1000)	/* LEDの点滅周期 msec	*/

/* リフレクタセンサ */
#if SIMULATION_ENV == 0
#define REFLECTANCE_THRESHOLD		((unsigned int)400)		/* 白と黒を区別するリフレクタセンサの出力値(設置後に閾値変更)		*/
#else
#define REFLECTANCE_THRESHOLD		((unsigned int)400)		/* 白と黒を区別するリフレクタセンサの出力値(シミュレーション環境)	*/
#endif

/* 制御コマンド*/
#define MAX_COMMAND					(10)					/* 制御コマンドの入力可能数 */
#define INPUT_COMMAND_NUM			(100)					/* コマンド入力配列数		*/

/********************************/
/*		グローバル変数定義		*/
/********************************/
char g_cCommand[MAX_COMMAND];								/* 制御コマンド格納配列					*/
int g_iDoCmdNum;											/* 制御コマンド格納配列のインデックス	*/

/*****************************/
/* コマンド入力配列			 */
/*****************************/
#if TEST_INPUT_PTN == 1
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "RLLLLLLLL.";				/* テスト1 コマンド入力配列 */
#elif TEST_INPUT_PTN == 2
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "X.";						/* テスト2 コマンド入力配列 */
#elif TEST_INPUT_PTN == 3
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "frrDLF.";				/* テスト3 コマンド入力配列 */
#elif TEST_INPUT_PTN == 4
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "LLRRLLRRllr";			/* テスト4 コマンド入力配列 */
#elif TEST_INPUT_PTN == 5
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = ".";						/* テスト5 コマンド入力配列 */
#elif TEST_INPUT_PTN == 6
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "R.";						/* テスト6 コマンド入力配列 */
#elif TEST_INPUT_PTN == 7
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "f.";						/* テスト7 コマンド入力配列 */
#else
/******************************************0123456789****/
char g_cInputCommand[INPUT_COMMAND_NUM] = "fRlr.";					/* テスト8 コマンド入力配列 */
#endif

/****************************/
/*		関数形式マクロ		*/
/****************************/
/****************************************************************************/
/* マクロ名	: ON_LINE(val)													*/
/* 機能		: リフレクトタセンサ 黒線検出判定								*/
/* 引数		: val: リフレクタセンサ値										*/
/* 戻り値	: 0: 黒線未検出 / 1: 黒線検出									*/
/****************************************************************************/
#define ON_LINE(val)				((val) > REFLECTANCE_THRESHOLD)

/********************/
/*		関数		*/
/********************/
/****************************************************************************/
/* 関数名	: getCommand()													*/
/* 機能		: 制御コマンドの入力をする										*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: なし															*/
/****************************************************************************/
void getCommand(void) {

	char cCmd;					/* 読み取ったコマンド							*/
	int iCmdNum = 0;			/* 制御コマンド格納配列に格納したコマンド数		*/
	int iInputCmdNum = 0;		/* コマンド入力配列から読み出したコマンド数		*/

	SerialPrintIn("Input Command.");  		/* コマンド入力を促すメッセージ出力 */
 
	while (1) {

		/************************************************/
		/* 入力可能なコマンド数の上限を超えた場合の処理 */
		/************************************************/
		if (iCmdNum >= MAX_COMMAND) {	/* 読み取り可能なコマンド数を超過 */

			SerialPrintIn("Over Max Command. ");	/* 入力超過メッセージを出力 */
			SerialPrintIn(&g_cCommand[0]);			/* 入力コマンドの出力 */
			break;  /* コマンド入力を終了する */
		}

		/***************************/
		/* コマンドを1文字読み取る */
		/***************************/
		/* コマンド入力配列からコマンドを読み取る */
		cCmd = g_cInputCommand[iInputCmdNum];	
		iInputCmdNum++;

		/**************************************/
		/* 制御コマンドでない場合は読み飛ばす */
		/**************************************/
		if ( ((cCmd != 'r') && (cCmd != 'R')
		  &&  (cCmd != 'f') && (cCmd != 'F')
		  &&  (cCmd != 'l') && (cCmd != 'L')
		  &&  (cCmd != 'd') && (cCmd != 'D')
		  &&  (cCmd != '.'))) {

			SerialPrintIn("Wrong Command!");	/* 誤ったコマンドメッセージを出力 */
	    } else {

			/*******************************************************************/
			/* 'd'/'D'コマンドの場合、それまで入力されたコマンドを全て削除する */
			/*******************************************************************/
			if (cCmd == 'd' || cCmd == 'D' ) {
				iCmdNum = 0;  	/* 制御コマンド格納配列に格納したコマンド数をクリア */
				SerialPrintIn("Delete All Command!");	/* コマンド削除メッセージを出力 */
			} else {
				/********************************************************/
				/* 読み取ったコマンドを、制御コマンド格納配列に格納する */
				/********************************************************/
				g_cCommand[iCmdNum] = cCmd;
				iCmdNum++;

				/*********************************/
				/* '.'コマンド（＝以上コマンド） */
				/*********************************/
				if (cCmd == '.') {
					SerialPrintIn(&g_cCommand[0]);	/* 入力コマンドの出力 */
					break;	/* コマンド入力を終了する */
				}
			}
		}
	}

	return;
}

/****************************************************************************/
/* 関数名	: goStraight()													*/
/* 機能		: 前進処理をする												*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: Zumoの傾きを検出し、補正をしながら前進をする					*/
/*			: 交差点の横線を検出した場合は関数を終了する					*/
/****************************************************************************/
void goStraight(void) {

	MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);				/* 前進時のモータ速度設定 */

	while (1) {
		#if SIM_DEBUG_CONSOLE != 0
		SerialPrintIn("DEBUG: While goStraight");
		#endif
		ReflectancesUpdate();	/* リフレクタセンサ値更新 */

		/* 前進する */
		if (ON_LINE(g_reflectances_value2)) {					/* 右に傾きあり */
			MotorsSetSpeeds(MOTOR_SPEED_REVERSE, MOTOR_SPEED);	/* 左側のモータ速度を逆回転に設定 */
		} else if (ON_LINE(g_reflectances_value5)) {			/* 左に傾きあり */
			MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED_REVERSE);	/* 右側のモータ速度を逆回転に設定 */
		} else {												/* 傾きなし */
			MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);			/* 前進時のモータ速度設定 */
		}

		/* 横線を検出した場合関数を終了する */
		if ( ON_LINE(g_reflectances_value1)
		 ||  ON_LINE(g_reflectances_value6)) {
			break;
		}
	}

	return;
}

/****************************************************************************/
/* 関数名	: doTurn()														*/
/* 機能		: 旋回処理をする												*/
/* 引数		: cDir :  旋回方向												*/
/* 戻り値	: なし															*/
/* 備考		: 旋回後の前進処理は本関数で実施しない							*/
/* 			: 旋回方向 右旋回：'r', 'R'  左旋回：'l', 'L'					*/
/****************************************************************************/
void doTurn(char cDir) {

#if DOTURN_SENSOR_RIGHT_LEFT == 0

	int iCount = 0;			/* リフレクタセンサ エッジ検出回数 */
	unsigned int iPstate;	/* リフレクタセンサ 前回値 */

	reflectances.update();	/* リフレクタセンサ値更新 */

	switch (cDir) {
		/* 右旋回 */
		case 'r':
		case 'R':
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:doTurn:RIGHT");
			#endif
			MotorsSetSpeeds(TURN_SPEED,TURN_SPEED_REVERSE);	/* 右側のモータ速度を逆回転に設定 */

			iPstate = g_reflectances_value5;			/* 右側のリフレクタセンサ値を取得する */
			while (iCount < TURN_EDGE_CNT) {			/* 右旋回をして次の進行方向を見つける（右側のリフレクタセンサがエッジを2回検出） */
				#if SIM_DEBUG_CONSOLE != 0
				SerialPrintIn("DEBUG: While doTurn");
				#endif

				ReflectancesUpdate();					/* リフレクタセンサ値更新 */
				if ((ON_LINE(g_reflectances_value5) ^ ON_LINE(iPstate))) {		/* 右側のリフレクタセンサがエッジを検出 */
					iCount++;
				}
				iPstate = g_reflectances_value5;		/* 右側のリフレクタセンサ前回値更新 */
			}
			break;

		/* 左旋回 */ 
		case 'l':
		case 'L':
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:doTurn:LEFT");
			#endif

			MotorsSetSpeeds(TURN_SPEED_REVERSE, TURN_SPEED);	/* 左側のモータ速度を逆回転に設定 */

			iPstate = g_reflectances_value2;			/* 左側のリフレクタセンサ値を取得 */
			while (iCount < TURN_EDGE_CNT) {			/* 左旋回をして次の進行方向を見つける（左側のリフレクタセンサがエッジを2回検出） */
				#if SIM_DEBUG_CONSOLE != 0
				SerialPrintIn("DEBUG: While doTurn");
				#endif
				
				ReflectancesUpdate();					/* リフレクタセンサ値更新 */
				if ((ON_LINE(g_reflectances_value2)) ^ ON_LINE(iPstate)) {		/* 左側のリフレクタセンサがエッジを検出 */
					iCount++;
				}
				iPstate = g_reflectances_value2;		/* 左側のリフレクタセンサ前回値更新 */
			}
			break;

		default:
			/* 処理なし */
			break;
	}

#else

	int iCount_right = 0;		/* リフレクタセンサ エッジ検出回数(右) */
	int iCount_left  = 0;		/* リフレクタセンサ エッジ検出回数(左) */
	unsigned int iPstate_right;	/* リフレクタセンサ 前回値(右) */
	unsigned int iPstate_left;	/* リフレクタセンサ 前回値(左) */

	/* リフレクタセンサ値更新 */
	ReflectancesUpdate();

	/* リフレクタセンサ値取得 */
	iPstate_right = g_reflectances_value5;
	iPstate_left  = g_reflectances_value2;

	/* 旋回方向設定 */
	switch (cDir) {
		/* 右旋回 */
		case 'r':
		case 'R':
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:doTurn:RIGHT");
			#endif
			MotorsSetSpeeds(TURN_SPEED,TURN_SPEED_REVERSE);	/* 右側のモータ速度を逆回転に設定 */
			break;

		/* 左旋回 */ 
		case 'l':
		case 'L':
			#if SIM_DEBUG_CONSOLE != 0
			SerialPrintIn("DEBUG:doTurn:LEFT");
			#endif
			MotorsSetSpeeds(TURN_SPEED_REVERSE, TURN_SPEED);	/* 左側のモータ速度を逆回転に設定 */
			break;

		default:
			/* 処理なし */
			break;
	}

	/* 旋回終了判定 */
	switch (cDir) {
		/* 右旋回 */
		case 'r':
		case 'R':
		/* 左旋回 */ 
		case 'l':
		case 'L':

			while( (iCount_right < TURN_EDGE_CNT) 
			    && (iCount_left  < TURN_EDGE_CNT)) {	/* 旋回をして次の進行方向を見つける（左右いずれかのリフレクタセンサがエッジを2回検出） */
				#if SIM_DEBUG_CONSOLE != 0
				SerialPrintIn("DEBUG: While doTurn");
				#endif
				ReflectancesUpdate();

				if ((ON_LINE(g_reflectances_value5) ^ ON_LINE(iPstate_right))) {
					/* 右側のリフレクタセンサがエッジを検出 */
					iCount_right++;
			    } 
				if ((ON_LINE(g_reflectances_value2)) ^ ON_LINE(iPstate_left)){
					/* 左側のリフレクタセンサがエッジを検出 */
					iCount_left++;
				} 

				/* リフレクタセンサ前回値更新 */
				iPstate_right = g_reflectances_value5;
				iPstate_left  = g_reflectances_value2;
			}
			break;

		default:
			/* 処理なし */
			break;
	}
#endif

	return;
}

/****************************************************************************/
/* 関数名	: doOperation()													*/
/* 機能		: 走行コマンドに従ってZumoを動作させる							*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: なし															*/
/****************************************************************************/
void doOperation(void) {

	char cCmd;		/* 読み取った走行コマンド */

	#if	SIMULATION_ENV != 0
	bool bRet;		/* Delay関数 戻り値格納 */
	#endif

	g_iStatus = STATUS_ZUMO_DRIVE;			/* 状態: Zumo走行モード */
	#if SIM_DEBUG_CONSOLE != 0	
    SerialPrintIn("DEBUG:STATUS_ZUMO_DRIVE");
	#endif

	/* 制御コマンド格納配列からコマンドを読み取る */
	cCmd = g_cCommand[g_iDoCmdNum];
	g_iDoCmdNum++;

	#if SIM_DEBUG_CONSOLE != 0	
	SerialPrintIn("DEBUG:cCmd");
	SerialPrintIn(&cCmd);
	#endif

	/* 交差点の横棒を検出するまで前進 */
	/* 前進コマンド、旋回後の前進処理を実施 */
	goStraight();

	/* 交差点の中心まで進む */
	MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);	/* 前進時のモータ速度設定 */
	delay(INTERSECTION_OSTIM);					/* 交差点中心までの走行時間 待機する */

	/****************/
	/* コマンド処理 */
	/****************/
	if ( (cCmd == '.') || (g_iDoCmdNum >= MAX_COMMAND)) {
		/* 停止コマンド または 実行コマンド数 超過 */ 
		g_iStatus = STATUS_ZUMO_STOP;				/* 状態：停止モード */
		#if SIM_DEBUG_CONSOLE != 0	
   		SerialPrintIn("DEBUG:STATUS_ZUMO_STOP");
		#endif
		MotorsSetSpeeds(STOP_SPEED, STOP_SPEED);	/* 停止時のモータ速度設定 */

		/* 停止モード */
		while (1) {							/* LEDをLED_FLASHCYCLE間隔で点滅続ける */

			LedOn();							/* LED点灯 */
			#if SIM_DEBUG_CONSOLE != 0	
			SerialPrintIn("DEBUG:STATUS_ZUMO_STOP:LEDON");
			#endif
			#if	SIMULATION_ENV == 0
			/* 実機環境 */
			delay(LED_FLASHCYCLE);				/* LEDの点滅周期 待機する */
			#else

			bRet = delay(LED_FLASHCYCLE);		/* LEDの点滅周期 待機する */

			if (bRet == false) {
				break;
			}
			#endif

			LedOff();							/* LED消灯 */
			#if SIM_DEBUG_CONSOLE != 0	
			SerialPrintIn("DEBUG:STATUS_ZUMO_STOP:LEDOFF");
			#endif
			#if	SIMULATION_ENV == 0
			/* 実機環境 */
			delay(LED_FLASHCYCLE);				/* LEDの点滅周期 待機する */
			#else

			bRet = delay(LED_FLASHCYCLE);		/* LEDの点滅周期 待機する */

			if (bRet == false) {
				break;
			}
			#endif
		}
	} else 	if ((cCmd == 'r') || (cCmd == 'R') 
			 || (cCmd == 'l') || (cCmd == 'L')) {

		/* 右折/左折コマンド */
		doTurn(cCmd);	/* 旋回処理 */

	} else {
		/* 前進コマンド */
		/* 処理なし*/
		/* 次の関数呼び出しにて横棒を検出するまで前進するため */
	}

	return;
}