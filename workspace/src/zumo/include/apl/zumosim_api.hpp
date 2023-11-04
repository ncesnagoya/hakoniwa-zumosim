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