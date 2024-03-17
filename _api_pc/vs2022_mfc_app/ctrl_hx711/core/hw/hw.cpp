/*****************************************************************//**
 * \file   hw.cpp
 * \brief
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

 // if this is mfc , then include the precompiled header
#include "pch.h"

#include "hw.hpp"


#ifdef _USE_HW_CLI
/****************************************************
 *	cli
 ****************************************************/

 // initializer static memvers
std::thread TinyC::Cli::task_trd{};
std::atomic<bool> TinyC::Cli::trd_life{};
HANDLE TinyC::Cli::hConsole = nullptr;
std::string TinyC::Cli::c_input{};
std::map<std::string, std::function<int(int, char**)>> TinyC::Cli::callbacks{};

/////////////////////////////////////////////////////
#endif


bool ap_core::hw_init(void)
{
  std::cout << "hw_init" << std::endl;
  return false;
}