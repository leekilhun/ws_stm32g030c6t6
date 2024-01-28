/*
 * ap_dat.hpp
 *
 *  Created on: 2023. 8. 10.
 *      Author: gns2l
 */

#pragma once
#ifndef AP__INC_AP_DAT_HPP_
#define AP__INC_AP_DAT_HPP_


#include "ap_def.hpp"


#ifdef _USE_APP_IO_BD
struct mcu_io_align_12_t
{
private:
    static constexpr auto io_dword_max = 1;
    static constexpr auto data_dword_max = 1;

public:
    enum dat_addr : uint8_t
    {
        addr_io_in_1 = 0,
        addr_io_out_0 = 4,
        addr_datas_0 = 8,
        addr_datas_1 = 10,
    };
    std::array<uint32_t, io_dword_max> io_in{};   //
    std::array<uint32_t, io_dword_max> io_out{};  //
    std::array<uint16_t, data_dword_max> datas{}; //

    mcu_io_align_12_t() = default;
    ~mcu_io_align_12_t() = default;
    // copy constructor
    mcu_io_align_12_t(const mcu_io_align_12_t &rhs) = default;
    // copy assignment operator
    mcu_io_align_12_t &operator=(const mcu_io_align_12_t &rhs) = default;
    // move constructor
    mcu_io_align_12_t(mcu_io_align_12_t &&rhs) = default;
    // move assignment operator
    mcu_io_align_12_t &operator=(mcu_io_align_12_t &&rhs) = default;
};


#else
struct AxisSystemXRZ
{
  uint16_t pos_x{}; // max position data 650mm
  uint16_t pos_r{}; // max position data 650mm
  uint16_t pos_z{}; // max position data 650mm

  AxisSystemXRZ() = default;
  ~AxisSystemXRZ() = default;
  // copy constructor
  AxisSystemXRZ(const AxisSystemXRZ& rhs) = default;
  // copy assignment operator
  AxisSystemXRZ& operator=(const AxisSystemXRZ& rhs) = default;
  // move constructor
  AxisSystemXRZ(AxisSystemXRZ&& rhs) = default;
  // move assignment operator
  AxisSystemXRZ& operator=(AxisSystemXRZ&& rhs) = default;

  void Init()
  {
    pos_x = 0;
    pos_r = 0;
    pos_z = 0;
  }
};

struct pos_data_st
{
  using type = AxisSystemXRZ;
  enum idx_e : uint8_t
  {
    mdi_null,
    mdi_0, //matrix data index
    mdi_1,
    mdi_2,
    mdi_3,

    mdi_253,
    mdi_max // max 255
  };

  /*position unit = 10um*/
  /*velocity unit =rpm */
  /*acc dec unit =m/ss */
  // max position data 650mm
  type data{};
  uint16_t rpm{};
  uint16_t acc{};
  uint16_t dec{};

  pos_data_st() = default;
  ~pos_data_st() = default;
  // copy constructor
  pos_data_st(const pos_data_st& rhs) = default;
  // copy assignment operator
  pos_data_st& operator=(const pos_data_st& rhs) = default;
  // move constructor
  pos_data_st(pos_data_st&& rhs) = default;
  // move assignment operator
  pos_data_st& operator=(pos_data_st&& rhs) = default;

  void Init()
  {
    data.Init();
    rpm = 0;
    acc = 0;
    dec = 0;
  }
};


struct mcu_tool_data_align_48_t
{
private:
    static constexpr auto io_dword_max = 1;
    static constexpr auto motor_dword_max = 3;
    static constexpr auto data_dword_max = 2;

public:
    enum dat_addr : uint8_t
    {
        addr_system = 0,
        addr_state = 2,
        addr_option = 4,
        addr_err = 8,
        addr_io_in_0 = 12,
        addr_io_out_0 = 16,
        addr_motor_cnt = 20,
        addr_motor_reserved = 22,
        addr_motor_pulse_0 = 24,
        addr_motor_pulse_1 = 28,
        addr_motor_pulse_2 = 32,
        addr_motor_state_0 = 36,
        addr_motor_state_1 = 38,
        addr_motor_state_2 = 40,
        addr_motor_reserved2 = 42,
        addr_datas_0 = 44,
        addr_datas_1 = 46,
    };
    uint16_t reg_sys{};                                   //
    uint16_t reg_state{};                                 //
    uint32_t reg_option{};                                //
    uint32_t reg_err{};                                   // 
    std::array<uint32_t, io_dword_max> io_in{};           // 
    std::array<uint32_t, io_dword_max> io_out{};          // 
    uint16_t motor_cnt{};                                 //
    uint16_t motor_reserved{};                            // 24
    std::array<uint32_t, motor_dword_max> motor_pulse{};  // 36
    std::array<uint16_t, motor_dword_max> motor_status{}; // 
    uint16_t motor_reserved2{};                           // 44
    std::array<uint16_t, data_dword_max> datas{};         // 48

    mcu_tool_data_align_48_t() = default;
    ~mcu_tool_data_align_48_t() = default;
    // copy constructor
    mcu_tool_data_align_48_t(const mcu_tool_data_align_48_t &rhs) = default;
    // copy assignment operator
    mcu_tool_data_align_48_t &operator=(const mcu_tool_data_align_48_t &rhs) = default;
    // move constructor
    mcu_tool_data_align_48_t(mcu_tool_data_align_48_t &&rhs) = default;
    // move assignment operator
    mcu_tool_data_align_48_t &operator=(mcu_tool_data_align_48_t &&rhs) = default;
};



#define AP_LOG_DAT_BUFF_SIZE               60
#define AP_LOG_DAT_HEADER                  0x11 // for nextion_lcd display
struct log_dat
{
  struct head_t
  {
    uint8_t  header=AP_LOG_DAT_HEADER;
    uint8_t  error_no{};
    uint8_t  obj_id{};
    uint8_t  step_no{};

    head_t() = default;
    // copy constructor
    head_t(const head_t& rhs) = default;
    // copy assignment operator
    head_t& operator=(const head_t& rhs) = default;
    // move constructor
    head_t(head_t&& rhs) = default;
    // move assignment operator
    head_t& operator=(head_t&& rhs) = default;

  };

  struct dat_t
  {
    head_t head{};
    std::array<char, AP_LOG_DAT_BUFF_SIZE> log{};

    dat_t() = default;
    // copy constructor
    dat_t(const dat_t& rhs) = default;
    // copy assignment operator
    dat_t& operator=(const dat_t& rhs) = default;
    // move constructor
    dat_t(dat_t&& rhs) = default;
    // move assignment operator
    dat_t& operator=(dat_t&& rhs) = default;

     inline void init(){
      head.header = AP_LOG_DAT_HEADER;
      head.error_no = 0;
      head.obj_id = 0;
      head.step_no = 0;
      log.fill(0);
    }
  } log_buff;

  inline char* GetBuffer(){
    return &log_buff.log[0];
  }

  inline uint8_t GetBufferLen(){
    return (uint8_t)log_buff.log.size();
  }
  inline void WriteData(uint8_t addr, dat_t &ref_data) {
  }

  inline dat_t* GetData(uint8_t addr){   
    return &log_buff;
  }


};

#endif

#endif /* AP__INC_AP_DAT_HPP_ */
