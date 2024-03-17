/*****************************************************************//**
 * \file   core.cpp
 * \brief  
 *   core -> sys -> hw -> ap ���� ����
 *   ap : �� ��ü�� �����ϰ� �����带 �����Ѵ�.
 *     -  en -> cn  ��ü ���� ������ ���� �� �ִ�.
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
