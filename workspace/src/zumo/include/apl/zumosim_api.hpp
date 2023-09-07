#ifndef _ZUMOSIM_API_HPP_
#define _ZUMOSIM_API_HPP_

#ifdef __cplusplus
extern "C" {
#endif

extern void zumosim_led_ctrl(bool on);
#define ZUMOSIM_REFLECT_SENSOR_NUM  6
extern bool zumosim_reflect_values(unsigned int values[ZUMOSIM_REFLECT_SENSOR_NUM]);

#ifdef __cplusplus
}
#endif

#endif /* _ZUMOSIM_API_HPP_ */