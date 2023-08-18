/*
 * ap_dat.hpp
 *
 *  Created on: 2023. 8. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_DAT_HPP_
#define AP__INC_AP_DAT_HPP_






constexpr auto mcu_data_io_dword_max = 3;
constexpr auto mcu_data_motor_dword_max = 4;
constexpr auto mcu_data_data_dword_max = 4;
//max 80 byte
struct mcu_data_st
{
  uint16_t reg_sys{};
  uint16_t reg_state{}; //
  uint32_t reg_option{}; //
  uint32_t reg_err{}; //
  std::array < uint32_t, mcu_data_io_dword_max> io_in{};//
  std::array < uint32_t, mcu_data_io_dword_max> io_out{};//
  uint16_t motor_cnt{};//
  std::array < uint32_t, mcu_data_motor_dword_max> motor_pulse{};//
  std::array < uint16_t, mcu_data_motor_dword_max> motor_status{};//
  std::array < uint32_t, mcu_data_data_dword_max> datas{};


  mcu_data_st() = default;

  ~mcu_data_st() = default;
  // copy constructor
  mcu_data_st(const mcu_data_st& rhs) = default;
  // copy assignment operator
  mcu_data_st& operator=(const mcu_data_st& rhs) = default;
  // move constructor
  mcu_data_st(mcu_data_st&& rhs) = default;
  // move assignment operator
  mcu_data_st& operator=(mcu_data_st&& rhs) = default;
};



#endif /* AP__INC_AP_DAT_HPP_ */
