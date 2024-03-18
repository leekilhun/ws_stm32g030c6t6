/*****************************************************************//**
 * \file   core.cpp
 * \brief  
 * 
 * 프로젝트 폴더/
 * └─ src/
 *     ├── core/
 *     │   ├── ap/
 *     │   │   ├── _inc/
 *     │   │   ├── api/
 *     │   │   ├── cn/
 *     │   │   └── en/
 *     │   │ 
 *     │   ├── common/
 *     │   │   ├── _inc_hw/
 *     │   │   └── util/
 *     │   │ 
 *     │   ├── hw/
 *     │   │   └── drive/
 *     │   │ 
 *     │   └── sys/
 *     │   
 *     ├── _data
 *     │   
 *     ├── mcc_lib
 *     │   
 *     └── main.cpp
 * 
 *
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/

 // if this is mfc , then include the precompiled header
#include "pch.h"


#include "core.hpp"


/****************************************************
 *	log
 ****************************************************/

 // initializer static memvers
uint32_t TinyC::mcc_log::trace_prc_pre_tick = 0;
std::mutex TinyC::mcc_log::log_mutex{};
TinyC::Que<TinyC::mcc_log::dat_st> TinyC::mcc_log::log_table[mcc_log::lvl_max] = { TinyC::Que<mcc_log::dat_st>(4096), TinyC::Que<mcc_log::dat_st>(4096), TinyC::Que<mcc_log::dat_st>(4096) };
