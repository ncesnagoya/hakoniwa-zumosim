#include "zumosim.hpp"

hako_asset_runner_callback_t zumosim_callbacks = {
    NULL,   // setup
    NULL,   // write_initial_pdu_data
    NULL,   // do_task
    NULL    // reset
};
