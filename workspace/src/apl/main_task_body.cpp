#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"
#include <iostream>

static bool led_status = false;

void apl_main_task_setup(void)
{
    led_status = false;
}

#define TASK_CYCLE 100
void apl_main_task_body(void)
{
    static int count = 0;
    count++;
    if ((count % TASK_CYCLE) != 0) {
        return;
    }
    
    if (led_status) {
        led_status = false;
        zumosim_led_ctrl(led_status);
    }
    else {
        led_status = true;
        zumosim_led_ctrl(led_status);
    }
    unsigned int values[ZUMOSIM_REFLECT_SENSOR_NUM];
    if (zumosim_reflect_values(values)) {
        int i;
        for (i = 0; i < ZUMOSIM_REFLECT_SENSOR_NUM; i++) {
            std::cout << "[" << i << "] = " << values[i] << std::endl;
        }
    }
    return;
}