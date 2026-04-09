//
// FILE: Operation.ino
// Ver. 1.1
// DATE: 2026/04/08
//
//  enPiT-Reskill Emb
//     PBL 課題2 Zumo坂道走行プログラム
//     Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN
//

#include  "common.h"
#include  "common_setting.h"

#if SIMULATION_ENV == 0
/* 実機環境のみ Zumo ライブラリを利用 */
#include  <Wire.h>
#include  <ZumoShield.h>

// Zumoライブラリ利用のための定義
Pushbutton button(ZUMO_BUTTON);
ZumoIMU    imu;
ZumoMotors motors;
ZumoBuzzer buzzer;
#endif /* SIMULATION_ENV == 0 */

// 入力制御機能データ
char  g_cSwStatus = SWITCH_OFF;       // プッシュボタンスイッチ入力情報
int   g_iSlopeDeg;                    // 坂道推定角度情報

// 出力制御機能データ
char  g_cLedOutput = OUT_LED_ODR_OFF; // LED出力指示
char  g_cBuzOutput = OUT_BUZ_ODR_OFF; // ブザー出力指示
char  g_cMtrOutput = OUT_MTR_ODR_OFF; // モータ出力指示

// プロトタイプ宣言
void makeSwitchInput( void );                       // スイッチ入力情報生成処理
void makeGyroSenserInput( void );                   // センサ入力情報生成処理
char getDriveState( char cStateNow, int iDegPre );  // 走行状態変更処理
void setOutputOrder(char cLedOrder, char cBuzOrder, char cMtrOrder);  // 出力指示設定処理

#if SIMULATION_ENV != 0
/* シミュレーション環境用の補助変数 */
/*   - millis() の代替として doOutput() 呼び出し回数からの経過時間擬似値を使う */
/*   - LED の HIGH/LOW 表現用の定数を提供                                       */
#ifndef LOW
#define LOW   (0)
#endif
#ifndef HIGH
#define HIGH  (1)
#endif
static unsigned long sim_loop_tick_ms = 0;        // 擬似経過時間 [msec]
#define SIM_TICK_STEP_MS  (ZUMOSIM_MIN_DELAY_MSEC)
static unsigned long millis( void ) {
  return sim_loop_tick_ms;
}
#endif /* SIMULATION_ENV != 0 */

// =============
// 初期設定機能
// =============
void initializeZumo( void ) {
#if SIMULATION_ENV == 0
  Serial.begin(9600);           // シリアル通信設定(デバッグ用)
  Wire.begin();
  // ジャイロセンサの初期化
  imu.init();
  imu.enableDefault();

  pinMode(LED_PIN, OUTPUT);
#else
  /* シミュレーション環境では main_task_body.cpp で取得済みの IF を利用するため、
     ハードウェア初期化は不要 */
  Serial.println("INFO: course_2 initializeZumo (sim)");
#endif
}

// =============
// 走行制御機能
// =============
void doDrive( void ) {
  static char cMode = ZUMO_MODE_WAIT;         // 動作モード
  static char cDriveState = DRIVE_ROAD_FLAT;  // 走行状態
  static int iSlopeDegPre;                    // 前回の坂道推定角度

  // モード変更処理
  if ( g_cSwStatus == SWITCH_ON ) {
    if ( cMode == ZUMO_MODE_WAIT ) {
      cMode = ZUMO_MODE_DRIVE;  // 走行モードへの遷移
      cDriveState = DRIVE_ROAD_FLAT;
    }
    else {
      cMode = ZUMO_MODE_WAIT;   // 待機モードへの遷移
    }
  }
  else {}

  // モード別処理
  if ( ZUMO_MODE_DRIVE == cMode ) {   // 走行モード時の処理
    // 走行状態変更処理
    cDriveState = getDriveState( cDriveState, iSlopeDegPre );

    // 走行状態別の出力指示
    switch ( cDriveState ) {
      case DRIVE_ROAD_DOWN:       // 下り坂
        setOutputOrder( OUT_LED_ODR_ON, OUT_BUZ_ODR_BEEP, OUT_MTR_ODR_ON );
        break;
      case DRIVE_ROAD_FLAT:       // 平坦
        setOutputOrder( OUT_LED_ODR_ON, OUT_BUZ_ODR_OFF, OUT_MTR_ODR_ON );
        break;
      case DRIVE_ROAD_UP:         // 上り坂
        setOutputOrder( OUT_LED_ODR_1Hz, OUT_BUZ_ODR_OFF, OUT_MTR_ODR_ON );
        break;
      case DRIVE_ROAD_LIMIT:      // 急な上り坂
        setOutputOrder( OUT_LED_ODR_OFF, OUT_BUZ_ODR_ON, OUT_MTR_ODR_OFF );
        break;
      default:                    // 異常なステータス
        setOutputOrder( OUT_LED_ODR_1Hz, OUT_BUZ_ODR_OFF, OUT_MTR_ODR_OFF );
        break;
    }
  }
  else {                              // 待機モード時の処理
    setOutputOrder( OUT_LED_ODR_1Hz, OUT_BUZ_ODR_OFF, OUT_MTR_ODR_OFF );
  }
  iSlopeDegPre = g_iSlopeDeg;     // 前回角度の更新
}

// 走行状態変更処理
char getDriveState( char cStateNow, int iDegPre ) {
  char cState;            // 戻り値(変更後の走行状態)

  cState = cStateNow;
  if ( DRIVE_ROAD_LIMIT != cStateNow ) {
    if ( DRIVE_LIMIT_SLOPE > g_iSlopeDeg ) {
      if ( ( 0 == iDegPre ) && ( 0 != g_iSlopeDeg ) ) {
        if ( ( 0 < g_iSlopeDeg ) && ( DRIVE_ROAD_UP > cState ) ) {
          cState++;       // 下り坂 -> 平坦 -> 上り坂
        }
        else if ( ( 0 > g_iSlopeDeg ) && ( DRIVE_ROAD_DOWN < cState ) ) {
          cState--;       // 下り坂 <- 平坦 <- 上り坂
        }
        else {}
      }
      else {}
    }
    else {
      // 走行制限路面(推定角度オーバー)を検出
      cState = DRIVE_ROAD_LIMIT;
    }
  }
  else {
    // 急な坂道を検出している場合は走行状態を変更しない
  }

  return cState;
}

// =============
// 入力制御機能
// =============
void doInput( void ) {
  makeSwitchInput();      // スイッチ入力情報生成処理
  makeGyroSenserInput();  // センサ入力情報生成処理
}

// スイッチ入力情報生成処理
void makeSwitchInput( void ) {
#if SIMULATION_ENV == 0
  if ( button.getSingleDebouncedRelease() ) {
    g_cSwStatus = SWITCH_ON;    // スイッチ入力あり
  }
  else {
    g_cSwStatus = SWITCH_OFF;   // スイッチ入力なし
  }
#else
  /* シミュレーション環境ではプッシュボタンが無いため、起動後の初回のみ
     SWITCH_ON を発火し、走行モードへ遷移させる。
     ※実環境とのI/F調整は後工程で実施する想定のスタブ。 */
  static char sim_button_fired = 0;
  if ( 0 == sim_button_fired ) {
    g_cSwStatus = SWITCH_ON;
    sim_button_fired = 1;
  }
  else {
    g_cSwStatus = SWITCH_OFF;
  }
#endif
}

// センサ入力情報生成処理
void makeGyroSenserInput( void ) {
#if SIMULATION_ENV == 0
  static int iArraySMA[ GYRO_SMA_NUM ] = {0,0,0,0,0};   // SMAフィルタ計算用
  static unsigned char cSetIndex = 0;                   // 格納先インデックス
  static long lSumGyro = 0;                             // センサ出力累積値
  static int iCounter = 0;                              // 連続ゼロ回数カウント用
  int iLoop, iSenTemp, iSumSMA, iGyroSMA;

  if ( imu.gyroDataReady() ) {  // ジャイロセンサデータありの場合のみ処理する
    imu.readGyro();   // センサ生値の読込み

    // しきい値フィルタ
    iSenTemp = imu.g.y + GYRO_SEN_OFFST;
    if ( ( GYRO_RAW_THR1 <= iSenTemp ) ||
         ( GYRO_RAW_THR2 >= iSenTemp ) ) {
      iArraySMA[ cSetIndex ] = iSenTemp;
    }
    else {
      iArraySMA[ cSetIndex ] = 0;
    }

    // SMAフィルタ
    iSumSMA = 0;
    for ( iLoop = 0; iLoop < GYRO_SMA_NUM; iLoop++ ) {
      iSumSMA += iArraySMA[ iLoop ];
    }
    iGyroSMA = iSumSMA / GYRO_SMA_NUM;
    cSetIndex++;
    cSetIndex %= GYRO_SMA_NUM;

    // 坂道推定角度変換
    lSumGyro += iGyroSMA;
    if ( iGyroSMA == GYRO_RESET_THR ) {
      iCounter++;     // 連続角度ゼロのカウント
      if ( iCounter >= GYRO_RESET_NUM ) {
        lSumGyro = 0;
      }
      else {}
    }
    else {
      iCounter = 0;
    }
    g_iSlopeDeg = -(int)( ( GYRO_CNVT_DEG1 * lSumGyro / GYRO_CNVT_DEG2 ) / 1000 );
  }
  else {}
#else
  /* シミュレーション環境ではジャイロI/Fが未提供のため、坂道角度は常に 0[deg]
     （平坦）として扱う。実際の坂道検出処理は後工程で接続する。 */
  g_iSlopeDeg = 0;
  // 演習では gyo IFを用意しているので取得して実装を行う

#endif
}

// =============
// 出力制御機能
// =============
void doOutput( void ) {
  static char cLED = LOW;           // LED出力前回値
  static char cVolm = 0;            // ブザー出力ボリューム係数
  static unsigned long ulTimer;     // タイマ更新用
  unsigned long now;                // 現在タイマ値
  char cFlg1Hz;                     // 点滅周期フラグ

#if SIMULATION_ENV != 0
  /* sim 環境では millis() が無いので擬似時計を進める */
  sim_loop_tick_ms += SIM_TICK_STEP_MS;
#endif

  // 点滅周期生成
  now = millis();
  cFlg1Hz = 0;
  if ( now - ulTimer > OUT_FREQ_FLASH ) {
    ulTimer = now;
    cFlg1Hz = 1;
  }
  else {}

  // LED出力設定
  switch ( g_cLedOutput ) {
    case OUT_LED_ODR_OFF:
      cLED = LOW;
      break;
    case OUT_LED_ODR_ON:
      cLED = HIGH;
      break;
    case OUT_LED_ODR_1Hz:
      if ( 1 == cFlg1Hz ) {
        cLED = (~cLED & 0x01);
      }
      else {}
      break;
    default:
      cLED = LOW;
      break;
  }
#if SIMULATION_ENV == 0
  digitalWrite(LED_PIN, cLED);
#else
  led.set( (int)cLED );
#endif

  // ブザー出力設定
#if SIMULATION_ENV == 0
  switch ( g_cBuzOutput ) {
    case OUT_BUZ_ODR_OFF:
      buzzer.stopPlaying();
      break;
    case OUT_BUZ_ODR_ON:
      buzzer.playFrequency( 440, 500, 10 );
      break;
    case OUT_BUZ_ODR_BEEP:
      if ( 1 == cFlg1Hz ) {
        cVolm = (~cVolm & 0x01);
        buzzer.playFrequency( 440, 500, 10*cVolm );
      }
      else {}
      break;
    default:
      buzzer.stopPlaying();
      break;
  }
#else
  /* シミュレーション環境ではブザーI/Fが未提供のためスタブ */
  (void)cVolm;
  (void)g_cBuzOutput;
#endif

  // モータ出力設定
  if ( OUT_MTR_ODR_ON == g_cMtrOutput ) {
    motors.setSpeeds( OUT_MTR_SPEED, OUT_MTR_SPEED );
  }
  else {
    motors.setSpeeds( 0, 0 );
  }
}

// 出力指示設定処理
void setOutputOrder(char cLedOrder, char cBuzOrder, char cMtrOrder) {
  g_cLedOutput = cLedOrder;
  g_cBuzOutput = cBuzOrder;
  g_cMtrOutput = cMtrOrder;
}
