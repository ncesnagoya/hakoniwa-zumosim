#include "main_task_body.hpp"
#include "apl/zumosim_api.hpp"

static bool led_status = false;
#define TASK_CYCLE 300
void apl_main_task_body(void)
{
    static int count = 0;
    count++;
    if ((count % TASK_CYCLE) != 0) {
        return;
    }
    
    if (led_status) {
        zumosim_led_ctrl(false);
    }
    else {
        zumosim_led_ctrl(true);
    }

    return;
}