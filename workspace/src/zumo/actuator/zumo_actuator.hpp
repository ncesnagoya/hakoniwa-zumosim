#ifndef _ZUMO_ACTUATOR_HPP_
#define _ZUMO_ACTUATOR_HPP_

#include "zumo_msgs/pdu_ctype_ZumoPduActuator.h"

extern Hako_ZumoPduActuator zumo_pdu_actuator;
extern void zumosim_actuator_init(void);
extern void zumosim_actuator_sync(void);

#endif /* _ZUMO_ACTUATOR_HPP_ */