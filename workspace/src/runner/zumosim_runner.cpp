#include <iostream>
#include "zumosim.hpp"
#include "main_task_body.hpp"

int main(int argc, const char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] <<  " <asset_name> <config path> <delta_time_msec>" << std::endl;
        return 1;
    }
    std::cout << "asset_name = " << argv[1] << std::endl;
    std::cout << "config path = " << argv[2] << std::endl;
    std::cout << "delta_time_msec = " << atoi(argv[3]) << std::endl;
    zumosim_set_delta_time_msec(atoi(argv[3]));
    hako_asset_runner_register_callback(&zumosim_callbacks);
    if (hako_asset_runner_init(argv[1], argv[2], atoi(argv[3]) * 1000) == false) {
        std::cerr << "ERROR: " << "hako_asset_runner_init() error" << std::endl;
        return 1;
    }
    zumosim_register_setup(apl_main_task_setup);
    zumosim_register_reset(apl_main_task_setup);
    //zumosim_register_task(apl_main_task_body);
    while (true) {
        std::cout << "INFO: start simulation" << std::endl;
        while (true) {
            apl_main_task_body();
            if (hako_asset_runner_step(1) == false) {
                std::cout << "INFO: stopped simulation" << std::endl;
                break;
            }
            //std::cout << "STEP" << std::endl;
        }
        //リセット発生した場合は最初からやり直す。
    }
    std::cout << "INFO: end simulation" << std::endl;
    hako_asset_runner_fin();
    return 0;
}