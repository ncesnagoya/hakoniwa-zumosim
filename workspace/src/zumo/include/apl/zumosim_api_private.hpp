#ifndef _ZUMOSIM_PRIVATE_API_HPP_
#define _ZUMOSIM_PRIVATE_API_HPP_

#ifdef __cplusplus
extern "C" {
#endif

extern void zumosim_led_ctrl(bool on);
#define ZUMOSIM_REFLECT_SENSOR_NUM  6
extern bool zumosim_reflect_values(unsigned int values[ZUMOSIM_REFLECT_SENSOR_NUM]);

#define ZUMOSIM_MIN_DELAY_MSEC  20

typedef enum {
    ZumoSimMotorDir_Left = 0,
    ZumoSimMotorDir_Right
} ZumoSimMotorDirType;
extern void zumosim_motor_ctrl(ZumoSimMotorDirType dir, int power);
extern float zumosim_mag_value();

#ifdef __cplusplus
}
#endif

#define ZUMOSIM_NOUSE_PARAM(param) do { if (param) {} } while(0)


#endif /* _ZUMOSIM_PRIVATE_API_HPP_ */