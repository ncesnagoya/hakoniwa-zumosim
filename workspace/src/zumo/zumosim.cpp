#include "runner/zumosim.hpp"
#include "zumo_sensor.hpp"
#include "zumo_actuator.hpp"
#include <iostream>

static void (*zumosim_main_task_body) (void);

void zumosim_register_task(void (*task) (void))
{
    zumosim_main_task_body = task;
}


static void zumosim_setup(void)
{
    std::cout << "INFO: ZUMOSIM SETUP" << std::endl;
    zumosim_sensor_init();
    zumosim_actuator_init();
}
static void zumosim_do_task(void)
{
    std::cout << "INFO: ZUMOSIM DO_TASK" << std::endl;
    if (zumosim_main_task_body != NULL) {
        //read pdu
        zumosim_sensor_sync();
        //do task
        zumosim_main_task_body();
        //write pdu
        zumosim_actuator_sync();
    }
}
static void zumosim_reset(void)
{
    std::cout << "INFO: ZUMOSIM RESET" << std::endl;
}


hako_asset_runner_callback_t zumosim_callbacks = {
    zumosim_setup,   // setup
    NULL,   // write_initial_pdu_data
    zumosim_do_task,   // do_task
    zumosim_reset    // reset
};

