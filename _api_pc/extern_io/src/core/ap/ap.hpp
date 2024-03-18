/*****************************************************************//**
 * \file   ap.hpp
 * \brief
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/


#ifndef AP_AP_HPP_
#define AP_AP_HPP_



#include "ap_def.hpp"

#include "def_obj.hpp"

#include "_inc/uart_nextion.hpp"
#include "_inc/uart_hx711.hpp"

namespace ap_core
{

  errno_t  init(void);
  //errno_t deinit(void);
  void  main(void);
  void  exit(void);

#ifdef _USE_AP_UART_NEXTION 
  using uart_lcd_t = uart_nextion<uint8_t>;
  std::shared_ptr<uart_lcd_t> get_lcd(void);
#endif

#ifdef _USE_AP_UART_HX711
  using uart_hx711_t = uart_hx711<uint8_t>;
	std::shared_ptr<uart_hx711_t> get_hx711(INST_IDX::_LORDCELL obj_idx);
  #endif
}
// end of namespace ap




#endif /* AP_AP_HPP_ */