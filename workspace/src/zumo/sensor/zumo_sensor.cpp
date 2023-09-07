#include "hako_asset_runner.h"
#include "apl/zumosim_api.hpp"
#include "config/zumosim_config.hpp"
#include "zumo_sensor.hpp"
#include "string.h"

Hako_ZumoPduSensor zumo_pdu_sensor;

void zumosim_sensor_init(void)
{
    memset(&zumo_pdu_sensor, 0, sizeof(Hako_ZumoPduSensor));
}
void zumosim_sensor_sync(void)
{
    bool ret = hako_asset_runner_pdu_read(ZUMOSIM_ROBO_NAME, ZUMOSIM_CHANNEL_ID_SENSOR, (char*)&zumo_pdu_sensor, sizeof(Hako_ZumoPduSensor));
    if (ret == false) {
        //TODO
    }
}
