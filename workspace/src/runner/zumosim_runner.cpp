#include <iostream>
#include "hako_asset_runner.h"

static hako_asset_runner_callback_t my_callbacks = {
    NULL,   // setup
    NULL,   // write_initial_pdu_data
    NULL,   // do_task
    NULL    // reset
};

int main(int argc, const char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] <<  " <asset_name> <config path> <delta_time_msec>" << std::endl;
        return 1;
    }
    std::cout << "asset_name = " << argv[1] << std::endl;
    std::cout << "config path = " << argv[2] << std::endl;
    std::cout << "delta_time_msec = " << atoi(argv[3]) << std::endl;
    if (hako_asset_runner_init(argv[1], argv[2], atoi(argv[3]) * 1000) == false) {
        std::cerr << "ERROR: " << "hako_asset_runner_init() error" << std::endl;
        return 1;
    }
    hako_asset_runner_register_callback(&my_callbacks);
    std::cout << "INFO: start simulation" << std::endl;
    while (true) {
        if (hako_asset_runner_step(1) == false) {
            break;
        }
    }
    std::cout << "INFO: end simulation" << std::endl;
    hako_asset_runner_fin();    
    return 0;
}