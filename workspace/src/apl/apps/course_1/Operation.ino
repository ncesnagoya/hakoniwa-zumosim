/************************************************************************************/
/*		FILE: Operation.ino															*/
/*		Ver. 1.3																	*/
/*		ATE: 2023/11/30																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム										*/
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/
#include "common.h"

/************************/
/*		マクロ定義		*/
/************************/
#define DEBUG_MOTOR_SPEED			(0)					/* 0: 運用時のモータ速度 / 0以外: デバッグ時のモータ速度 */
#define TEST_PTN					(11)				/* コマンド設定配列 1:テスト1 / 2:テスト2 / 3:テスト3 */

/* モータ速度 */
#define STOP_SPEED					(0)					/* モーター停止速度					*/
#define MOTOR_SPEED					(120)				/* 直進時のモータ走行速度			*/
#define MOTOR_SPEED_REVERSE			(-120)				/* 直進時のモータ走行速度(逆回転)	*/

#if DEBUG_MOTOR_SPEED == 0
/* 運用時のモータ速度 */
#define TURN_SPEED					(120)				/* 回転時のモータ走行速度			*/
#define TURN_SPEED_REVERSE			(-120)				/* 回転時のモータ走行速度(逆回転)	*/
#else
/* デバッグ時のモータ速度 */
#define TURN_SPEED					(240)				/* 回転時のモータ走行速度			*/
#define TURN_SPEED_REVERSE			(-240)				/* 回転時のモータ走行速度(逆回転)	*/
#endif

/* 待機時間 */
#define INTERSECTION_OSTIM			(350)				/* 交差点の横棒を検出してから交差点中心までの走行時間 msec	*/
#define LED_FLASHCYCLE				(1000)				/* LEDの点滅周期 1秒間隔 msec	*/

/* リフレクタセンサ */
#if SIMULATION_ENV == 0
#define REFLECTANCE_THRESHOLD		(400)				/* 【実】白と黒を区別するリフレクタセンサの出力値	*/
#else
#define REFLECTANCE_THRESHOLD		(400)				/* 【シ】白と黒を区別するリフレクタセンサの出力値	*/
#endif

/* 制御コマンド*/
#define MAX_COMMAND					(10)				/* 制御コマンドの入力可能数 */
#define SERIAL_SUB_NUM				(100)				/* コマンド入力配列数		*/

/********************************/
/*		グローバル変数定義		*/
/********************************/
char g_cCommand[MAX_COMMAND];							/* 制御コマンドを記憶する配列 */

/*****************************/
/* コマンド設定配列			 */
/*****************************/

#if TEST_PTN == 1
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "X.";						/* テスト1 コマンド設定配列 */
#elif TEST_PTN == 2
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "frrDLF.";					/* テスト2 コマンド設定配列 */
#elif TEST_PTN == 3
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "LLRRLLRRll";				/* テスト3 コマンド設定配列 */
#elif TEST_PTN == 4
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "BRBLLLDLLFFLRLRLF.";		/* テスト4 コマンド設定配列 */
#elif TEST_PTN == 5
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "BRBLLLDLLFFLRLRL.";		/* テスト5 コマンド設定配列 */
#elif TEST_PTN == 6
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = ".";						/* テスト6 コマンド設定配列 */
#elif TEST_PTN == 7
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "R.";						/* テスト7 コマンド設定配列 */
#elif TEST_PTN == 8
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "RFL.";					/* テスト8 コマンド設定配列 */
#elif TEST_PTN == 9
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "RFLL.";					/* テスト9 コマンド設定配列 */
#elif TEST_PTN == 10
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "RFLLrl.";					/* テスト10 コマンド設定配列 */
#elif TEST_PTN == 11
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "RFLLlr.";					/* テスト11 コマンド設定配列 */
#else
/*************************************0123456789****/
char g_cSerialSubs[SERIAL_SUB_NUM] = "rfl.";					/* テスト12 コマンド設定配列 */
#endif

/****************************/
/*		関数形式マクロ		*/
/****************************/
/****************************************************************************/
/* マクロ名	: ON_LINE(val)													*/
/* 機能		: リフレクトタセンサ 黒線検出									*/
/* 引数		: val: リフレクタセンサ値										*/
/* 戻り値	: 0: 黒線未検出 / 1: 黒線検出									*/
/****************************************************************************/
#define ON_LINE(val)				((val) > REFLECTANCE_THRESHOLD)

/********************/
/*		関数		*/
/********************/
/****************************************************************************/
/* 関数名	: getCommand()													*/
/* 機能		: コマンド入力													*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: 出力 外部変数: g_cCommand[] 入力したコマンド列				*/
/*			: シリアルからコマンドを読み取り、g_cCommand[]に格納			*/
/*			: 'd'/'D'コマンド:読み取ったコマンドをg_cCommand[]から削除		*/
/*			: 正規のコマンド以外は、g_cCommand[]に格納しない				*/
/*			: 入力終了条件 (a)または(b)										*/
/*			: (a) '.' コマンド入力時										*/
/*			: (b)コマンドの最大数 MAX_COMMAND まで g_cCommand[] に格納		*/
/****************************************************************************/
void getCommand(void) {

	char cCmd;					/* 読み取ったコマンド							*/
	int iCmdNum = 0;			/* g_cCommand[]に格納したコマンド数				*/
	int iLoopcSerialSubs = 0;	/* コマンド設定配列で用いる繰り返しの添え字		*/

	#if SIM_DEBUG_CONSOLE != 0
	Serial.println("Input g_cCommand.");  /* コマンド入力を促すメッセージ出力 */
	#endif

	/* コマンドを読み続ける */ 
	while (1) {

		/************************************************/
		/* 入力可能なコマンド数の上限を超えた場合の処理 */
		/************************************************/
		if (iCmdNum >= MAX_COMMAND) {	/* 読み取り可能なコマンド数を超過 */

			Serial.println("Over Max Command. "); /* 入力超過と表示 */
			Serial.println(&g_cCommand[0]);
			break;  /* コマンド入力を終了する */
		}

		/***************************/
		/* コマンドを1文字読み取る */
		/***************************/
		/*【実/シ】シリアル入力代替配列からコマンドを読み取る */
		cCmd = g_cSerialSubs[iLoopcSerialSubs];	
		iLoopcSerialSubs++;

		/**************************************/
		/* 正規コマンドでない場合は読み飛ばす */
		/**************************************/
		if ( ((cCmd != 'r') && (cCmd != 'R')
		  &&  (cCmd != 'f') && (cCmd != 'F')
		  &&  (cCmd != 'l') && (cCmd != 'L')
		  &&  (cCmd != 'd') && (cCmd != 'D')
		  &&  (cCmd != '.'))) {

			Serial.println("Wrong Command!");
	    } else {

			/*******************************************************************/
			/* 'd'/'D'コマンドの場合、それまで入力されたコマンドを全て削除する */
			/*******************************************************************/
			if (cCmd == 'd' || cCmd == 'D' ) {
				iCmdNum = 0;  	/*【シ】【実】シリアル入力代替配列からコマンドを読み取る */
				Serial.println("Delete All Command!");
			} else {
				/************************************************/
				/* 読み取ったコマンドを、g_cCommand[]に格納する */
				/************************************************/
				g_cCommand[iCmdNum] = cCmd;
				iCmdNum++;

				/*********************************/
				/* '.'コマンド（＝以上コマンド） */
				/*********************************/
				if (cCmd == '.') {
					Serial.println(&g_cCommand[0]);	/* 入力コマンドの出力 */
					break;	/* コマンド入力を終了する */
				}
			}
		}
	}

	return;
}

/****************************************************************************/
/* 関数名	: goStraight()													*/
/* 機能		: 直進処理														*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: 傾きを調整しながら直進する									*/
/****************************************************************************/
void goStraight(void) {

	MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);

	while (1) {
		#if SIM_DEBUG_CONSOLE != 0
		Serial.println("DEBUG: While goStraight");
		#endif
		ReflectancesUpdate();	/* リフレクタセンサを読み取る */

		/* 直進する */
		if (ON_LINE(g_reflectances_value2)) {			/* 右に傾いている */
			MotorsSetSpeeds(MOTOR_SPEED_REVERSE, MOTOR_SPEED);	/* 左方向に車体を向けて進む */
		} else if (ON_LINE(g_reflectances_value5)) {	/* 左に傾いている */
			MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED_REVERSE);	/* 右方向に車体を向けて進む */
		} else {												/* 直進している */
			MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);
		}

		/* 横線を見つけたら停止するためにこの関数を抜ける */
		if ( ON_LINE(g_reflectances_value1)
		 ||  ON_LINE(g_reflectances_value6)) {
			break;
		}
	}

	return;
}

/****************************************************************************/
/* 関数名	: doTurn()														*/
/* 機能		: Zumoを左右に回転させる										*/
/* 引数		: cDir :  回転方向												*/
/* 戻り値	: なし															*/
/* 備考		: 回転後の前進処理は本関数で実施しない							*/
/****************************************************************************/
void doTurn(char cDir) {

	int iCount_right = 0;
	int iCount_left  = 0;
	int iPstate_right;
	int iPstate_left;

	ReflectancesUpdate();	/* リフレクタセンサを読み取る */
	iPstate_right = g_reflectances_value5;			/* 右側のリフレクタセンサを読み取る */
	iPstate_left  = g_reflectances_value2;			/* 左側のリフレクタセンサを読み取る */

	/* 回転方向設定 */
	switch (cDir) {
		/* 右折 */
		case 'r':
		case 'R':
			#if SIM_DEBUG_CONSOLE != 0
			Serial.println("DEBUG:doTurn:RIGHT");
			#endif
			MotorsSetSpeeds(TURN_SPEED,TURN_SPEED_REVERSE);	/* 右回転をする */
			break;

		/* 左折 */ 
		case 'l':
		case 'L':
			#if SIM_DEBUG_CONSOLE != 0
			Serial.println("DEBUG:doTurn:LEFT");
			#endif
			MotorsSetSpeeds(TURN_SPEED_REVERSE, TURN_SPEED);	/* 左回転をする */
			break;

		default:
			/* 処理なし */
			break;
	}

	/* 回転終了判定 */
	switch (cDir) {
		/* 右折 */
		case 'r':
		case 'R':
		/* 左折 */ 
		case 'l':
		case 'L':

			while( (iCount_right < 2) 
			    || (iCount_left  < 2)) {	/* 回転をして次の進行方向を見つける（左右いずれかのリフレクタセンサが白⇒黒、黒⇒白のエッジを検出） */
				#if SIM_DEBUG_CONSOLE != 0
				Serial.println("DEBUG: While doTurn");
				#endif
				ReflectancesUpdate();

				if ((ON_LINE(g_reflectances_value5) ^ ON_LINE(iPstate_right))) {
					/* 右側のリフレクタセンサが黒⇔白のエッジを検出 */
					iCount_right++;
			    } else if ((ON_LINE(g_reflectances_value2)) ^ ON_LINE(iPstate_left)){
					/* 左側のリフレクタセンサが黒⇔白のエッジを検出 */
					iCount_left++;
				} else {
					/* 処理なし */
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
	return;
}

/****************************************************************************/
/* 関数名	: doOperation()													*/
/* 機能		: 走行コマンドに従ってZumoを移動させる							*/
/* 引数		: なし															*/
/* 戻り値	: なし															*/
/* 備考		: なし															*/
/****************************************************************************/
void doOperation(void) {

	char cCmd;		/* 読み取った走行コマンド */

	#if	SIMULATION_ENV != 0
	bool bRet;
	#endif

	g_iStatus = STATUS_ZUMO_DRIVE;			/* 状態: Zumo走行モード */
	#if SIM_DEBUG_CONSOLE != 0	
    Serial.println("DEBUG:STATUS_ZUMO_DRIVE");
	#endif

	/* 配列に入っているコマンドを一つずつ読みとる */
	cCmd = g_cCommand[g_iDoCmdNum];
	g_iDoCmdNum++;

	#if SIM_DEBUG_CONSOLE != 0	
	Serial.println("DEBUG:cCmd");
	Serial.println(&cCmd);
	#endif

	/****************/
	/* コマンド処理 */
	/****************/
	if ( (cCmd == '.') || (g_iDoCmdNum >= MAX_COMMAND)) {
		/* 停止コマンド または 実行コマンド数 超過 */ 
		g_iStatus = STATUS_ZUMO_STOP;				/* 状態：停止モード */
		#if SIM_DEBUG_CONSOLE != 0	
   		Serial.println("DEBUG:STATUS_ZUMO_STOP");
		#endif
		MotorsSetSpeeds(STOP_SPEED, STOP_SPEED);	/* Zumo走行停止 */

		/* 停止モード */
		while (1) {							/* LEDをLED_FLASHCYCLE間隔で点滅続ける */

			LedOn();							/* 点灯 */
			#if SIM_DEBUG_CONSOLE != 0	
			Serial.println("DEBUG:STATUS_ZUMO_STOP:LEDON");
			#endif
			#if	SIMULATION_ENV == 0
			/* 実機環境 */
			delay(LED_FLASHCYCLE);				/* 点灯時間 */
			#else

			bRet = delay(LED_FLASHCYCLE);		/* 点灯時間 */

			if (bRet == false) {
				break;
			}
			#endif

			LedOff();						/* 消灯 */
			#if SIM_DEBUG_CONSOLE != 0	
			Serial.println("DEBUG:STATUS_ZUMO_STOP:LEDOFF");
			#endif
			#if	SIMULATION_ENV == 0
			/* 実機環境 */
			delay(LED_FLASHCYCLE);	/* 消灯時間 */
			#else

			bRet = delay(LED_FLASHCYCLE);	/* 消灯時間 */

			if (bRet == false) {
				break;
			}
			#endif
		}
	} else {

		/* 交差点の横棒を見つけるまで前進し、横棒を見つけたら停止する */
		/* 直進コマンド、回転後の前進処理 */
		goStraight();

		/* 交差点の中心まで進む */
		MotorsSetSpeeds(MOTOR_SPEED, MOTOR_SPEED);
		delay(INTERSECTION_OSTIM); /* 交差点の横棒を検出してから交差点中心までの走行時間 */

		if ((cCmd == 'r') || (cCmd == 'R') 
			|| (cCmd == 'l') || (cCmd == 'L')) {

			/* 左右への回転コマンド */
			doTurn(cCmd);	/* 回転 */

		} else if ((cCmd == 'f') || (cCmd == 'F')) {
			/* 	直進コマンド */
			/* ここでは何もしない */
			/* 次に doOperationが呼ばれたときに横棒を見つけるまで直進する */
		} else {
			/* 処理なし */
		}
	}

	return;
}