#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"

static bool led_status = false;
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

    return;
}