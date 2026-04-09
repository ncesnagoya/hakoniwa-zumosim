//
// FILE: common_setting.h
// Ver. 1.0
// DATE: 2023/11/29
//
//  enPiT-Reskill Emb
//     PBL 課題2 Zumo坂道走行プログラム
//     Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN
//

// ユーザLEDポート番号
#define LED_PIN 13

// プッシュボタンスイッチ入力関連定義
#define SWITCH_OFF  0
#define SWITCH_ON   1

// ジャイロセンサ入力関連定義
#define GYRO_SEN_OFFST  680     // 平坦でのジャイロセンサ入力をフラットにするためのオフセット
#define GYRO_RAW_THR1   100     // しきい値フィルタ(上限)
#define GYRO_RAW_THR2   -100    // しきい値フィルタ(下限)
#define GYRO_SMA_NUM    5       // 単純移動平均フィルタサイズ
#define GYRO_RESET_THR  0       // センサ出力累積値リセットしきい値
#define GYRO_RESET_NUM  10      // センサ出力累積値リセットしきい値回数
#define GYRO_CNVT_DEG1  875     // 角度推定計算用係数１(8.75[mdps] *100)
#define GYRO_CNVT_DEG2  20800    // 角度推定計算用係数２(208[Hz] *100)

// Zumoの動作モード
#define ZUMO_MODE_WAIT  0       // 待機モード
#define ZUMO_MODE_DRIVE 1       // 走行モード

// 走行制御機能関連定義
#define DRIVE_ROAD_DOWN   0     // 下り坂
#define DRIVE_ROAD_FLAT   1     // 平坦
#define DRIVE_ROAD_UP     2     // 上り坂
#define DRIVE_ROAD_LIMIT  3     // 急な上り坂
#define DRIVE_LIMIT_SLOPE 25    // 上り坂上限角度[deg]

// 出力制御機能関連定義
#define	OUT_FREQ_FLASH    500   // 1Hz用タイマ値(Duty50%)[msec]

#define OUT_LED_ODR_OFF   0     // LED出力指示：    消灯
#define OUT_LED_ODR_ON    1     // LED出力指示：    点灯
#define OUT_LED_ODR_1Hz   2     // LED出力指示：    点滅(1Hz)

#define OUT_BUZ_ODR_OFF   0     // ブザー出力指示： 消音
#define OUT_BUZ_ODR_ON    1     // ブザー出力指示： 鳴動(連続)
#define OUT_BUZ_ODR_BEEP  2     // ブザー出力指示： 鳴動(ビープ)

#define OUT_MTR_ODR_OFF   0     // モータ出力指示： 停止
#define OUT_MTR_ODR_ON    1     // モータ出力指示： 駆動
#define OUT_MTR_SPEED     80    // 走行速度

// グローバル関数プロトタイプ宣言
void initializeZumo( void );    // 初期設定機能
void doInput( void );           // 入力制御機能
void doDrive( void );           // 走行制御機能
void doOutput( void );          // 出力制御機能
