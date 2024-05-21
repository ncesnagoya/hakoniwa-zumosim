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

#ifndef _ZUMOSIM_API_HPP_
#define _ZUMOSIM_API_HPP_

#include "IZumoShield.hpp"
#include "zumosim_api_private.hpp"
#include <iostream>

class ZumoSimSerial : public IZumoSerial
{
  public:
	virtual ~ZumoSimSerial() = default;
	virtual void print(const char* msg)
    {
        std::cout << msg;
    }
	virtual void println(const char* msg)
    {
        std::cout << msg << std::endl;
    }
	virtual void println()
    {
        std::cout << std::endl;
    }
};

class ZumoSimMotors : public IZumoMotors
{
  public:
	virtual ~ZumoSimMotors() {}
	void setSpeeds(int leftSpeed, int rightSpeed)
    {
        zumosim_motor_ctrl(ZumoSimMotorDir_Left, leftSpeed);
        zumosim_motor_ctrl(ZumoSimMotorDir_Right, rightSpeed);
    }
};
class ZumoSimLED : public IZumoLED
{
  public:
	virtual ~ZumoSimLED() {}
	void on() 
    {
        zumosim_led_ctrl(true);
    }
	void off()
    {
        zumosim_led_ctrl(false);
    }
	void set(int i)
    {
        if (i == 1) {
            this->on();
        }
        else {
            this->off();
        }
    }
};
class ZumoSimBuzzer : public IZumoBuzzer
{
  public:
	virtual ~ZumoSimBuzzer() {}
	void playOn(void) {}
	void playStart(void)  {}
	void playNum(int cnt) 
    {
        ZUMOSIM_NOUSE_PARAM(cnt);
    }
};
class ZumoSimCompass : public IZumoCompass
{
  public:
	virtual ~ZumoSimCompass() {}
    virtual void begin(int speed) 
    {
        ZUMOSIM_NOUSE_PARAM(speed);
    }
    virtual void setCalibration(int max_x, int max_y, int min_x, int min_y) 
    {
        ZUMOSIM_NOUSE_PARAM(max_x);
        ZUMOSIM_NOUSE_PARAM(max_y);
        ZUMOSIM_NOUSE_PARAM(min_x);
        ZUMOSIM_NOUSE_PARAM(min_y);
    }
    virtual void doCalibration() {}
    virtual float averageHeading()
    {
        return zumosim_mag_value();
    }
};
class ZumoSimGyro : public IZumoGyro
{
  public:
	virtual ~ZumoSimGyro() {}
    int getTurnAngleDegree()
    {
        return 0;
    }
	void turnSensorReset() {}
	void turnSensorUpdate() {}
	void turnSensorSetup() {}
};


class ZumoSimReflectanceSensorArray : public IZumoReflectanceSensorArray
{
  public:
	virtual ~ZumoSimReflectanceSensorArray() {}
	void update()
    {
        zumo_delay(ZUMOSIM_MIN_DELAY_MSEC);
    }
	unsigned int value(int i)
    {
        if (i <= 0) {
            return -1;
        }
        else if (i > ZUMOSIM_REFLECT_SENSOR_NUM) {
            return -1;
        }
        unsigned int values[ZUMOSIM_REFLECT_SENSOR_NUM];
        (void)zumosim_reflect_values(values);
        return (1000 - values[i-1]);
    }
};

#endif /* _ZUMOSIM_API_HPP_ */