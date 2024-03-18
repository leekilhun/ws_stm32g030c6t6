/*****************************************************************//**
 * \file   hw.hpp
 * \brief
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef HW_HW_HPP
#define HW_HW_HPP


#include "hw_def.hpp"

#include "common/_inc_hw/uart.hpp"
namespace ap_core
{

#ifdef _USE_HW_UART
	using uart_t = serial_comm<uint8_t>;
#endif

	bool hw_init(void);

}
// end of namespace ap_core
#endif // HW_HW_HPP