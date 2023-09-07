#ifndef _ZUMO_SENSOR_HPP_
#define _ZUMO_SENSOR_HPP_

#include "zumo_msgs/pdu_ctype_ZumoPduSensor.h"

extern Hako_ZumoPduSensor zumo_pdu_sensor;
extern void zumosim_sensor_init(void);
extern void zumosim_sensor_sync(void);

#endif /* _ZUMO_SENSOR_HPP_ */