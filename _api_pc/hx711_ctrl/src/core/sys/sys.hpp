/*****************************************************************//**
 * \file   sys.hpp
 * \brief
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef SYS_HPP
#define SYS_HPP

#include "sys_def.hpp"
#include "sys_data.hpp"

namespace ap_core
{

  //instance index 
	namespace INST_IDX
	{
    enum _IO : uint32_t
    {
      io_max
    };
    enum _CYLINDER : uint32_t
    {
      cyl_max
    };

    enum _VACUUM : uint32_t
    {
      vac_max
    };

    enum _SERIAL : uint32_t
    {
      uart_com13,
      uart_com5,
      uart_max
    };

    enum _LCD : uint32_t
    {
      nextion,
      lcd_max
    };

    enum _LORDCELL : uint32_t
    {
      hx711_0,
      loadcell_max
    };

	}
	// end of obj



	bool sys_init(void);

	errno_t regist_object(uint32_t idx, IObj* ptr_obj);
	//
	template <typename T>
	T get_object_data(uint32_t idx);



}
// end of namespace ap_core





#endif // SYS_HPP