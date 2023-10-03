#ifndef _IZUMO_SHILED_HPP_
#define _IZUMO_SHILED_HPP_

class IZumoLED
{
  public:
	virtual ~IZumoLED() = default;
	virtual void on() = 0;
	virtual void off() = 0;
	virtual void set(int i) = 0;
};

class IZumoBuzzer
{
public:
    virtual ~IZumoBuzzer() = default;
	virtual void playOn(void) = 0;
	virtual void playStart(void) = 0;
	virtual void playNum(int cnt) = 0;
};

class IZumoCompass
{
public:
    virtual ~IZumoCompass() = default;
    virtual void begin(int speed) = 0;
    virtual void setCalibration(int max_x, int max_y, int min_x, int min_y) = 0;
    virtual void doCalibration() = 0;
    virtual float averageHeading() = 0;
};

class IZumoGyro
{
  public:
	virtual ~IZumoGyro() = default;
    virtual int getTurnAngleDegree() = 0;
	virtual void turnSensorReset() = 0;
	virtual void turnSensorUpdate() = 0;
	virtual void turnSensorSetup() = 0;
};

class IZumoMotors
{
  public:
	virtual ~IZumoMotors() = default;
	virtual void setSpeeds(int leftSpeed, int rightSpeed) = 0;
};

class IZumoReflectanceSensorArray
{
  public:
	virtual ~IZumoReflectanceSensorArray() = default;
	virtual void update() = 0;
	virtual unsigned int value(int i) = 0;
};

#endif /* _IZUMO_SHILED_HPP_ */