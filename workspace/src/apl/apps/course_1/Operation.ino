//
// FILE: Operation.ino
// Ver. 1.2
// DATE: 2023/10/20
//
//  enPiT-Reskill Emb
//     PBL 課題1 Zumoコマンド走行プログラム
//     Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN
//

#if 0
#define TURN_SPEED  120 //運用時のモーター走行速度
#else
#define TURN_SPEED  240 //デバッグ時のモーター走行速度
#endif

#define SIMULATION_ENV 1

#define STOP_SPEED	0	//モーターをストップさせる速度
#define MOTOR_SPEED 120   // 交差点検出後、交差点の中心まで走行する際のモーターの走行速度
#define INTERSECTION_OSTIM     350  //交差点の横棒を検出してから交差点中心までの走行時間 msec

#define LED_FLASHCYCLE         1000   //LEDの点滅周期 1秒間隔 msec

#define REFLECTANCE_THRESHOLD  400    //白と黒を区別するリフレクタセンサの出力値
#define REFLECTA_1	1	                //1番目のリフレクタセンサ
#define REFLECTA_2	2	                //2番目のリフレクタセンサ
#define REFLECTA_3	3	                //3番目のリフレクタセンサ
#define REFLECTA_4	4	                //4番目のリフレクタセンサ
#define REFLECTA_5	5	                //5番目のリフレクタセンサ
#define REFLECTA_6	6	                //6番目のリフレクタセンサ
#define ON_LINE(val)  ((val) > REFLECTANCE_THRESHOLD)     //リフレクタセンサの読取値が黒を検出（黒線を見つけたと同義）

//
// コマンド入力
//    出力。外部変数：g_cCommand[]  入力したコマンド列
//    シリアルからコマンドを読み取り、g_cCommand[]に格納する
//      'd'/'D'コマンドは、今まで読み取ったコマンドをg_cCommand[]から削除する
//      正規のコマンド以外は、g_cCommand[]に格納しない
//      入力終了条件 (a)または(b)
//        (a) '.' コマンド入力時
//        (b)コマンドの最大数 MAX_COMMAND まで g_cCommand[] に格納
//
void getCommand(void) {
  char cCmd;          //読み取ったコマンド
  int iCmdNum = 0;    //g_cCommand[]に格納したコマンド数
  int iLoopCommand = 0;	    //コマンド配列で用いる繰り返しの添え字

  int iLoopcSeialSubs = 0;  //シリアル入力代替配列で用いる繰り返しの添え字

  Serial.print("Input g_cCommand.");  //コマンド入力を促すメッセージ出力
  Serial.println();	  //改行出力

// コマンドを読み続ける
  while (1) {

    //
    // 入力可能なコマンド数の上限を超えた場合の処理
    //
    if (iCmdNum >= MAX_COMMAND) {	//読み取り可能なコマンド数を超過
      Serial.println("Over Max Command. "); // 入力超過と表示
      for(iLoopCommand = 0; iLoopCommand <= MAX_COMMAND; iLoopCommand++){ // 今まで入力されたコマンドを表示する
        Serial.print(&g_cCommand[iLoopCommand]);
      }
      Serial.println();  //改行

      return;  //コマンド入力を終了する。wile()ループを抜ける
    }

    #ifndef SIMULATION_ENV  // 【実】シリアル入力ができない場合は、読み込みができるまで待つ
    if (!(Serial.available() > 0)) {
      continue;
    }
    #endif

    //
    //コマンドを読み取る
    //
    #ifndef SIMULATION_ENV  //【実】シリアルからコマンドを読み取る
    cCmd = Serial.read();		// コマンドを1文字読み取る
    #else                   //【シ】シリアル入力代替配列からコマンドを読み取る
    cCmd = g_cSeialSubs[iLoopcSeialSubs++];		//コマンドを1文字読み取る
    #endif

    //
    //正規コマンドでない場合は読み飛ばす。以降の処理をしない
    //
    if (!((cCmd == 'r') || (cCmd == 'R')
           ||  (cCmd == 'f') || (cCmd == 'F')
           ||  (cCmd == 'l') || (cCmd == 'L')
           ||  (cCmd == 'd') || (cCmd == 'D')
           ||  (cCmd == '.'))) {

      Serial.println("Wrong Command!");
      continue; // while(1)ループの先頭に戻る
    }

    //
    // 'd'/'D'コマンドの場合、それまで入力されたコマンドを全て削除する
    //
    if (cCmd == 'd' || cCmd == 'D' ) {
      iCmdNum = 0;  //読み取ったコマンドを格納するg_cCommand[]添え字を0にする
      Serial.print("Delete All Command!");
      continue;
    }

    //
    //読み取ったコマンドを、g_cCommand[]に格納する
    //
    g_cCommand[iCmdNum++] = cCmd;

    //
    //'.'コマンド（＝以上コマンド）
    //
    if (cCmd == '.') {
      for(iLoopCommand = 0; iLoopCommand < iCmdNum; iLoopCommand++){
        Serial.print(&g_cCommand[iLoopCommand]);    //入力コマンドの出力
      }
      Serial.println();  //改行
      return;     //コマンド入力を終了する。wile()ループを抜ける
    }
  }
}


//
//
// 走行コマンドに従ってZumoを移動させる
//
//
void doOperation(void) {
  char cCmd;   //読み取った走行コマンド
  int iDoCmdNum = 0;  //g_cCommand[]のコマンドを先頭から順に取り出す添え字

  // 配列に入っているコマンドを一つずつ読みとる
  cCmd = g_cCommand[iDoCmdNum++];

  // 交差点の横棒を見つけるまで前進し、横棒を見つけたら停止する
  goStraight();

  //交差点の中心まで進む
  motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
  delay(INTERSECTION_OSTIM); //交差点の横棒を検出してから交差点中心までの走行時間

  //
  //コマンド処理
  //
  //停止コマンド
  if (cCmd == '.') {
     motors.setSpeeds(STOP_SPEED, STOP_SPEED);  // Zumo走行停止
      //停止モード
      while (1) {  //LEDをLED_FLASHCYCLE間隔で点滅続ける
        led.on(); //点灯
        bool bRet = delay(LED_FLASHCYCLE);  //点灯時間
        if (bRet == false) {
          return;
        }
        led.off(); //消灯
        bRet = delay(LED_FLASHCYCLE);  //消灯時間
        if (bRet == false) {
          return;
        }
      }
  }

  //左右への回転コマンド
  else if ((cCmd == 'r') || (cCmd == 'R') || (cCmd == 'l') || (cCmd == 'L')) {
    doTurn(cCmd);  // 回転

  //直進コマンド
  } else if ((cCmd == 'f') || (cCmd == 'F')) {
    // ここでは何もしない
    // 次に doOperationが呼ばれたときに横棒を見つけるまで直進する
  }
}


//
//
//直進
//
//
void goStraight(void) {
  motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);

  while (1) {
    reflectances.update();  // リフレクタセンサを読み取る

    //直進する
    if (ON_LINE(reflectances.value(REFLECTA_2))) {  // 右に傾いている
      motors.setSpeeds(-MOTOR_SPEED, MOTOR_SPEED);  // 左方向に車体を向けて進む
    } else if (ON_LINE(reflectances.value(REFLECTA_5))) { // 左に傾いている
      motors.setSpeeds(MOTOR_SPEED, -MOTOR_SPEED);  // 右方向に車体を向けて進む
    } else {  // 直進している
      motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    }

    //横線を見つけたら停止するためにこの関数を抜ける
    if (ON_LINE(reflectances.value(REFLECTA_1)) || ON_LINE(reflectances.value(REFLECTA_6))) { //リフレクタセンサの両端が共に黒線を検出
      return;
    }
  }
}


//
//
// Zumoを左右に回転させる
//  引数 cDir ： 回転方向
//
void doTurn(char cDir) {
  int iCount = 0;
  int iPstate;

  reflectances.update();  // リフレクタセンサを読み取る

  switch (cDir) {
    // 右折
    case 'r':
    case 'R':
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);  // 右回転をする
      iPstate = reflectances.value(REFLECTA_5);   // 左側のリフレクタセンサを読み取る
      while (iCount < 2) {    // 右回転をして次の進行方向を見つける（左側のリフレクタセンサが2回黒を検出）
        reflectances.update();
        if ((ON_LINE(reflectances.value(REFLECTA_5)) ^ ON_LINE(iPstate))) {   // 左側のリフレクタセンサが黒を検出
          iCount++;
        }
        iPstate = reflectances.value(REFLECTA_5);
      }
      break;

    // 左折
    case 'l':
    case 'L':
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);  //  左回転をする
      iPstate = reflectances.value(REFLECTA_2);  // 右側のリフレクタセンサを読み取る
      while (iCount < 2) {   // 左回転をして次の進行方向を見つける（右側のリフレクタセンサが2回黒を検出）
        reflectances.update();
        if ((ON_LINE(reflectances.value(REFLECTA_2)) ^ ON_LINE(iPstate))) {   // 右側のリフレクタセンサが黒を検出
          iCount++;
        }
        iPstate = reflectances.value(REFLECTA_2);
      }
      break;
  }
}