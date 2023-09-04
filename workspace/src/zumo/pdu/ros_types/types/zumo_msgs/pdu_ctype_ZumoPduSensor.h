#ifndef _pdu_ctype_zumo_msgs_ZumoPduSensor_H_
#define _pdu_ctype_zumo_msgs_ZumoPduSensor_H_

#include "pdu_primitive_ctypes.h"
#include "builtin_interfaces/pdu_ctype_Time.h"
#include "geometry_msgs/pdu_ctype_Quaternion.h"
#include "geometry_msgs/pdu_ctype_Vector3.h"
#include "sensor_msgs/pdu_ctype_Imu.h"
#include "std_msgs/pdu_ctype_Header.h"
#include "zumo_msgs/pdu_ctype_ZumoPduIrSensor.h"
#include "zumo_msgs/pdu_ctype_ZumoPduLineSensor.h"

typedef struct {
        Hako_ZumoPduIrSensor irs[2];
        Hako_ZumoPduLineSensor line_sensors[1];
        Hako_Imu    imu;
} Hako_ZumoPduSensor;

#endif /* _pdu_ctype_zumo_msgs_ZumoPduSensor_H_ */
