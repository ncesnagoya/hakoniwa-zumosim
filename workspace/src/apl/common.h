#ifndef _COMMON_H_
#define _COMMON_H_
/************************************************************************************/
/*		FILE: common.h																*/
/*		Ver. 1.3																	*/
/*		ATE: 2023/11/30																*/
/*																					*/
/*		enPiT-Reskill Emb															*/
/*		PBL 課題1 Zumoコマンド走行プログラム									           */
/*		Copyright (C) Graduate School of Information Science, Nagoya Univ., JAPAN	*/
/************************************************************************************/

/************************/
/*		マクロ定義		*/
/************************/
/* 評価環境 */
#define SIMULATION_ENV				(1)					/* 0: 実機環境                / 0以外: シミュレーション環境   */
#define SIM_DEBUG_CONSOLE           (0)					/* 0: デバックコンソール無効  / 0以外: デバックコンソール有効  */

/* 状態遷移 */
#define STATUS_INIT					(0)					/* 初期化状態			*/
#define STATUS_COMMAND_INPUT		(1)					/* コマンド入力モード	*/
#define STATUS_ZUMO_DRIVE			(2)					/* Zumo走行モード		*/
#define STATUS_ZUMO_STOP			(3)					/* 停止モード			*/

#endif