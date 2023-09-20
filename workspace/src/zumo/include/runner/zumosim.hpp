#ifndef _ZUMOSIMU_HPP_
#define _ZUMOSIMU_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include "hako_asset_runner.h"

extern hako_asset_runner_callback_t zumosim_callbacks;
extern void zumosim_register_task(void (*task) (void));
extern void zumosim_register_setup(void (*setup) (void));
extern void zumosim_register_reset(void (*setup) (void));
extern void zumosim_set_delta_time_msec(unsigned int msec);

#ifdef __cplusplus
}
#endif

#endif /* _ZUMOSIMU_HPP_ */