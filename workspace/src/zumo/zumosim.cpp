/*
 *  TOPPERS Hakoniwa ZumoSim
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Hakoniwa(a virtual simulation environment) for Zumo
 *
 *  Copyright (C) 2023-2024 by Center for Embedded Computing Systems
 *              Graduate School of Informatis, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 */

#include "runner/zumosim.hpp"
#include "apl/zumosim_api.hpp"
#include "zumo_sensor.hpp"
#include "zumo_actuator.hpp"
#include <iostream>

static void (*zumosim_main_task_body) (void);
static void (*zumosim_main_task_setup) (void);
static void (*zumosim_main_task_reset) (void);

static unsigned int zumosim_asset_delta_time_msec = 20;

void zumosim_set_delta_time_msec(unsigned int msec)
{
    zumosim_asset_delta_time_msec = msec;
}

void zumosim_register_reset(void (*reset) (void))
{
    zumosim_main_task_reset = reset;
}
void zumosim_register_setup(void (*setup) (void))
{
    zumosim_main_task_setup = setup;
}
void zumosim_register_task(void (*task) (void))
{
    zumosim_main_task_body = task;
}


static void zumosim_setup(void)
{
    std::cout << "INFO: ZUMOSIM SETUP" << std::endl;
    zumosim_sensor_init();
    zumosim_actuator_init();
    if (zumosim_main_task_setup != NULL) {
        zumosim_main_task_setup();
    }
}
#if 0
static void zumosim_do_task(void)
{
    //std::cout << "INFO: ZUMOSIM DO_TASK" << std::endl;
    if (zumosim_main_task_body != NULL) {
        //read pdu
        zumosim_sensor_sync();
        //do task
        zumosim_main_task_body();
        //write pdu
        zumosim_actuator_sync();
    }
}
#endif
static void zumosim_reset(void)
{
    std::cout << "INFO: ZUMOSIM RESET" << std::endl;
    zumosim_sensor_init();
    zumosim_actuator_init();
    if (zumosim_main_task_reset != NULL) {
        zumosim_main_task_reset();
    }
}

static bool zumosim_delay(unsigned int delay_msec)
{
    unsigned int delay_step = (delay_msec + zumosim_asset_delta_time_msec - 1) / zumosim_asset_delta_time_msec;
    //std::cout << "delay_step = " << delay_step << std::endl;
    //std::cout << "delta_time_msec = " << zumosim_asset_delta_time_msec << std::endl;
    zumosim_actuator_sync();
    bool ret = hako_asset_runner_step(delay_step);
    zumosim_sensor_sync();
    return ret;
}

bool zumo_delay(unsigned int delay_msec)
{
    return zumosim_delay(delay_msec);
}
static ZumoSimLED zumosim_led;
static ZumoSimCompass zumosim_compass;
static ZumoSimMotors zumosim_motors;
static ZumoSimReflectanceSensorArray zumosim_reflectance_sensor_array;
static ZumoSimSerial zumosim_serial;
IZumoLED& zumo_get_led()
{
    return zumosim_led;
}

IZumoCompass& zumo_get_compass()
{
    return zumosim_compass;
}
IZumoMotors& zumo_get_motors()
{
    return zumosim_motors;
}
IZumoReflectanceSensorArray& zumo_get_reflectance_sensor_array()
{
    return zumosim_reflectance_sensor_array;
}
IZumoSerial& zumo_get_serial()
{
    return zumosim_serial;
}
hako_asset_runner_callback_t zumosim_callbacks = {
    zumosim_setup,   // setup
    NULL,   // write_initial_pdu_data
    NULL,   // do_task
    zumosim_reset    // reset
};

