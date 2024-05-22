/*
 *  TOPPERS Hakoniwa ZumoSim
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *      Hakoniwa(a virtual simulation environment) for Zumo
 *
 *  Copyright (C) 2023-2024 by Center for Embedded Computing Systems
 *              Graduate School of Informatis, Nagoya Univ., JAPAN
 *
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 */

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
    zumosim_register_setup(apl_main_task_setup);
    zumosim_register_reset(apl_main_task_setup);
    hako_asset_runner_register_callback(&zumosim_callbacks);
    if (hako_asset_runner_init(argv[1], argv[2], atoi(argv[3]) * 1000) == false) {
        std::cerr << "ERROR: " << "hako_asset_runner_init() error" << std::endl;
        return 1;
    }
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