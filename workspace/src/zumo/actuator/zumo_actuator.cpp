#include "hako_asset_runner.h"
#include "apl/zumosim_api.hpp"
#include "config/zumosim_config.hpp"
#include "zumo_actuator.hpp"
#include "string.h"

Hako_ZumoPduActuator zumo_pdu_actuator;

void zumosim_actuator_init(void)
{
    memset(&zumo_pdu_actuator, 0, sizeof(Hako_ZumoPduActuator));
}

void zumosim_actuator_sync(void)
{
/* The code is calling the function `hako_asset_runner_pdu_write` to write data to a specific channel
in the Hako Asset Runner. */
    bool ret = hako_asset_runner_pdu_write(ZUMOSIM_ROBO_NAME, ZUMOSIM_CHANNEL_ID_ACTUATOR, (const char*)&zumo_pdu_actuator, sizeof(Hako_ZumoPduActuator));
    if (ret == false) {
        //TODO
    }
}

