/*
 * ap_io.hpp
 *
 *  Created on: 2023. 11. 27.
 *      Author: gns2l
 *
 *  edit :2024.02.03 ext_op io
 *
 *
 */

#pragma once
#ifndef AP__INC_AP_IO_HPP_
#define AP__INC_AP_IO_HPP_

#include "ap_def.hpp"


struct ap_io
{
  using data_t = uint8_t;
  static constexpr uint8_t def_io_port_of_bank = 8;
  static constexpr uint32_t def_in_start_addr  = AP_DEF_START_IN_ADDR;
  static constexpr uint32_t def_out_start_addr = AP_DEF_START_OUT_ADDR;

  enum io_bank
  {
    _bank_main_0808,
    _bank_max,
  };
  enum in_e // in_name
  {
    in_0x00 = def_in_start_addr,
    in_0x01,
    in_0x02,
    in_0x03,
    in_0x04,
    in_0x05,
    in_0x06,
    in_0x07,
  };
  enum out_e // out_name
  {
    out_0x00 = def_out_start_addr,
    out_0x01,
    out_0x02,
    out_0x03,
    out_0x04,
    out_0x05,
    out_0x06,
    out_0x07,
  };

  struct cfg_t
  {
    uint8_t ext_in_hc165_ch{};

    cfg_t()  = default;
    ~cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t &other) = default;
    // copy assignment
    cfg_t &operator=(const cfg_t &other) = default;
    // move constructor
    cfg_t(cfg_t &&other) = default;
    // move assignment
    cfg_t &operator=(cfg_t &&other) = default;
  } m_cfg{};

public:
  std::array<data_t, _bank_max> m_in{};
  std::array<data_t, _bank_max> m_out{};

public:
  ap_io()  = default;
  ~ap_io() = default;

public:
  inline errno_t Init(cfg_t cfg)
  {
    m_cfg = cfg;
    LOG_PRINT("[OK] Init Success io cnt [%dx%d]!", def_io_port_of_bank, _bank_max);
    return ERROR_SUCCESS;
  }

  inline errno_t SetOutputReg(uint8_t reg, uint8_t bank = 0)
  {
    if (bank >= _bank_max)
      return -1;

    uint8_t x_reg = reg ^ m_out[bank];
    // LOG_PRINT("reg[0x%04X], x_reg[0x%04X], bank[%d]",reg, x_reg, bank);
    for (uint8_t i = 0; i < def_io_port_of_bank; i++)
    {
      if ((x_reg >> (i)) & 1)
      {
        switch (bank)
        {
        case _bank_main_0808:
          SetGpioOut((i), ((reg >> (i)) & 1));
          break;

        default:
          return -1;
        }
      }
    }

    return ERROR_SUCCESS;
  }

  inline errno_t SetGpioOut(uint16_t out_bit, bool onoff = true)
  {
    GPIO_PinState state = onoff ? GPIO_OUT_STATE_ON : GPIO_OUT_STATE_OFF;
    switch ((out_e)(out_bit + def_out_start_addr))
    {
    case out_0x00:
      HAL_GPIO_WritePin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin, state);
      break;
    case out_0x01:
      HAL_GPIO_WritePin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin, state);
      break;
    case out_0x02:
      HAL_GPIO_WritePin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin, state);
      break;
    case out_0x03:
      HAL_GPIO_WritePin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin, state);
      break;
    case out_0x04:
      HAL_GPIO_WritePin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin, state);
      break;
    case out_0x05:
      HAL_GPIO_WritePin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin, state);
      break;
    case out_0x06:
      HAL_GPIO_WritePin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin, state);
      break;
    case out_0x07:
      HAL_GPIO_WritePin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin, state);
      break;
    default: return -1; break;
    }
    // end of switch
    return ERROR_SUCCESS;
  }

  inline void Update_io(void)
  {
    {
      data_t gpio{};
      // Out
      enum Bank
      {
        _b1,
        _b_max
      };
      std::array<data_t, _b_max> datas{};
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin) << 0;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin) << 1;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin) << 2;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin) << 3;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin) << 4;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin) << 5;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin) << 6;
      gpio       |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin) << 7;
      datas[_b1]  = gpio;

      m_out[_b1] = datas[_b1];

      // In
      datas.fill(0);
      gpio =  ~hc165_ReadInputs(m_cfg.ext_in_hc165_ch); //state 0 is on, 1 is off
      datas[_b1]  = gpio;
      m_in[_b1] = datas[_b1];
    }
  }

  /* IO  control function */
  inline bool IsOn(uint32_t addr)
  {
    Update_io();

    enum Bank
    {
      _b1,
      _b_max
    };
    // Input
    if (addr < AP_DEF_START_OUT_ADDR) 
    {
      uint32_t data = (uint32_t)m_in[_b1];
      return (data >> (addr - AP_DEF_START_IN_ADDR)) & 1;
    }

    // Output
    uint32_t data = (uint32_t)(m_out[_b1]);
    return (data >> (addr - AP_DEF_START_OUT_ADDR)) & 1;
  }

  inline bool IsOff(uint32_t addr)
  {
    return !IsOn(addr);
  }

  inline errno_t OutputOn(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr));
  }

  inline errno_t OutputOff(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr), false);
  }

  inline errno_t OutputToggle(uint32_t addr)
  {
    if (IsOn(addr))
      return OutputOff(addr);
    else
      return OutputOn(addr);
  }
};
// end of struct ap_io

#if 0
struct ap_io
{

#ifndef _USE_EXHW_IICOMM_DEV
  
  static constexpr uint8_t def_io_port_of_bank = 16;
  static constexpr uint8_t def_err_cnt_max = 100;
  static constexpr uint32_t def_in_start_addr = AP_DEF_START_IN_ADDR;
  static constexpr uint32_t def_out_start_addr = AP_DEF_START_OUT_ADDR;
  
  
  /**
   * @brief io controller 
   *  main   b'd input 16, output 16
   */
  enum io_bank
  {
    bank_main_1616,
    bank_max,
  };

  enum in_e // in_name
  {
    in_0x00 = def_in_start_addr,
    in_0x01,
    in_0x02,
    in_0x03,
    in_0x04,
    in_0x05,
    in_0x06,
    in_0x07,

    in_0x10, //
    in_0x11,
    in_0x12,
    in_0x13,
    in_0x14,
    in_0x15,
    in_0x16,
    in_0x17,
  };
  enum out_e // out_name
  {
    out_0x00 = def_out_start_addr,
    out_0x01,
    out_0x02,
    out_0x03,
    out_0x04,
    out_0x05,
    out_0x06,
    out_0x07,

    out_0x10, //
    out_0x11,
    out_0x12,
    out_0x13,
    out_0x14,
    out_0x15,
    out_0x16,
    out_0x17,

  };
#ifdef _USE_HW_RTOS
  osMutexId ap_io_mutex_id;
#endif

  struct cfg_t
  {
    uint8_t i2c_ch{};
    uint16_t io_dev_addr{};
    bool (*i2cBegin)(uint8_t, i2c_freq_t){};
    bool (*i2cRecovery)(uint8_t){};
    bool (*i2cReadBytes)(uint8_t, uint16_t, uint16_t, uint8_t *, uint32_t, uint32_t){};
    bool (*i2cWriteBytes)(uint8_t, uint16_t, uint16_t, uint8_t *, uint32_t, uint32_t){};

    cfg_t() = default;
    ~cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t &other) = default;
    // copy assignment
    cfg_t &operator=(const cfg_t &other) = default;
    // move constructor
    cfg_t(cfg_t &&other) = default;
    // move assignment
    cfg_t &operator=(cfg_t &&other) = default;
  } m_cfg{};

public:
  std::array<volatile uint16_t, bank_max> m_in{};
  std::array<volatile uint16_t, bank_max> m_out{};
  uint16_t m_errCnt{};
  bool m_isExtComm{};
public:
  ap_io() : m_in{}, m_out{}, m_errCnt{} ,m_isExtComm{}/*, m_sysio{}, */
  {
#ifdef _USE_HW_RTOS
    osMutexDef(ap_io_mutex_id);
    ap_io_mutex_id = osMutexCreate(osMutex(ap_io_mutex_id));
#endif
  }
  ~ap_io() {}

  inline errno_t Init(cfg_t cfg)
  {
    m_cfg = cfg;

    if (m_cfg.i2cBegin(m_cfg.ex_io_i2c_ch, I2C_FREQ_400KHz) == false)
      LOG_PRINT("[NG] i2cBegin fail ch[%d]", m_cfg.ex_io_i2c_ch);
    else
    {
      for (uint8_t i = 0x00; i <= 0x7F; i++)
      {
        if (i2cIsDeviceReady( m_cfg.ex_io_i2c_ch, i) == true)
        {
          LOG_PRINT("I2C CH%d Addr 0x%X : OK",  m_cfg.ex_io_i2c_ch + 1, i);
          m_isExtComm = true;
        }
      }
    }

    LOG_PRINT("[OK] Init Success io cnt [%dx%d]!", def_io_port_of_bank, bank_max);
    return ERROR_SUCCESS;
  }

  // Update information that has changed compared to previous data
  inline errno_t SetOutputReg(uint16_t reg, uint8_t bank = 0)
  {
    if (bank >= bank_max)
      return -1;

    uint16_t x_reg = reg ^ m_out[bank];
    //LOG_PRINT("reg[0x%04X], x_reg[0x%04X], bank[%d]",reg, x_reg, bank);
    for (uint8_t i = 0; i < def_io_port_of_bank; i++)
    {
      if ((x_reg >> (i)) & 1)
      {
        switch (bank)
        {
        case bank_main_1616:
          SetGpioOut((i), ((reg >> (i)) & 1));
          break;

        case bank_ext_1616:
          SetExtOut((i), ((reg >> (i)) & 1));
          break;

        default:
          return -1;
        }
      }
    }

    return ERROR_SUCCESS;
  }

  inline errno_t SetExtOut(uint16_t out_idx, bool onoff = true)
  {
    AP_IO_LOCK_BEGIN;

    if (m_isExtComm == false)
      return -1;

    //LOG_PRINT("out_idx[0x%04X], onoff[%d]", out_idx, onoff);
    if (onoff)
      m_out[bank_ext_1616] = m_out[bank_ext_1616] | (1 << out_idx);
    else
    {
      m_out[bank_ext_1616] = m_out[bank_ext_1616] & ~(1 << out_idx);
    }

    enum Bank
    {
      b1,
      b2,
      b_max
    };
    std::array<uint8_t, b_max> datas{LoBYTE(m_out[bank_ext_1616]),HiBYTE(m_out[bank_ext_1616])};
    //LOG_PRINT("[--] i2cWriteBytes addr[0x%02X], reg[0x%04X], data[0x%04X], m_out[0x%04X]", m_cfg.io_dev_addr, IICOMM_CMD_WR_OUT,MaKEWORD(datas[b1], datas[b2]),m_out[bank_ext_1616]);
    if (m_cfg.i2cWriteBytes(m_cfg.ex_io_i2c_ch, m_cfg.io_dev_addr, IICOMM_CMD_WR_OUT, datas.data(), datas.size(), 100) == false)
      LOG_PRINT("[NG] m_cfg.i2cWriteData fail! ");
    else
    {
      uint8_t order_set_out_reg = 1;
      if (m_cfg.i2cWriteBytes(m_cfg.ex_io_i2c_ch, m_cfg.io_dev_addr, (uint16_t)IICOMM_CMD_WR_ORDER, &order_set_out_reg, 1, 100) != true)
      {
        LOG_PRINT("[NG] IICOMM_CMD_WR_ORDER i2cWriteBytes [0x%02X] Fail! ", order_set_out_reg);
        return -1;
      }
      //LOG_PRINT("[OK] i2cWriteBytes addr[0x%02X], data[0x%04X]", m_cfg.io_dev_addr, MaKEWORD(datas[b1], datas[b2]));
      //m_out[bank_ext_1616] = MaKEWORD(datas[b1], datas[b2]);
    }

    AP_IO_LOCK_END;
    return ERROR_SUCCESS;
  }

  inline errno_t SetGpioOut(uint16_t out_bit, bool onoff = true)
  {
    AP_IO_LOCK_BEGIN;
    //LOG_PRINT(" out_bit[0x%04X]", out_bit);
    GPIO_PinState state = onoff ? GPIO_OUT_STATE_ON : GPIO_OUT_STATE_OFF;
    switch ((out_e)(out_bit + def_out_start_addr))
    {
    case out_0x00:
      HAL_GPIO_WritePin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin, state);
      break;
    case out_0x01:
      HAL_GPIO_WritePin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin, state);
      break;
    case out_0x02:
      HAL_GPIO_WritePin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin, state);
      break;
    case out_0x03:
      HAL_GPIO_WritePin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin, state);
      break;
    case out_0x04:
      HAL_GPIO_WritePin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin, state);
      break;
    case out_0x05:
      HAL_GPIO_WritePin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin, state);
      break;
    case out_0x06:
      HAL_GPIO_WritePin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin, state);
      break;
    case out_0x07:
      HAL_GPIO_WritePin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin, state);
      break;
    case out_0x10:
      HAL_GPIO_WritePin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin, state);
      break;
    case out_0x11:
      HAL_GPIO_WritePin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin, state);
      break;
    case out_0x12:
      HAL_GPIO_WritePin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin, state);
      break;
    case out_0x13:
      HAL_GPIO_WritePin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin, state);
      break;
    case out_0x14:
      HAL_GPIO_WritePin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin, state);
      break;
    case out_0x15:
      HAL_GPIO_WritePin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin, state);
      break;
    case out_0x16:
      HAL_GPIO_WritePin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin, state);
      break;
    case out_0x17:
      HAL_GPIO_WritePin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin, state);
      break;
    default:
      return -1;
    }
    // end of switch

    AP_IO_LOCK_END;
    return ERROR_SUCCESS;
  }

  inline void Update_io(void)
  {

    auto in_state = [](auto state) -> uint8_t
    {
      return (state == GPIO_IN_STATE_ON) ? 1 : 0;
    };

    {
      volatile uint8_t gpio{};
      // Out
      enum Bank
      {
        b1,
        b2,
        b_max
      };
      std::array<uint8_t, b_max> datas{};
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin) << 7;
      datas[b1] = gpio;

      gpio = 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin) << 7;
      datas[b2] = gpio;

      m_out[bank_main_1616] = *(reinterpret_cast<uint16_t *>(datas.data()));

      // In
      datas.fill(0);
      gpio = 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_0_GPIO_Port, IO_IN_0_Pin)) << 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_1_GPIO_Port, IO_IN_1_Pin)) << 1;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_2_GPIO_Port, IO_IN_2_Pin)) << 2;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_3_GPIO_Port, IO_IN_3_Pin)) << 3;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_4_GPIO_Port, IO_IN_4_Pin)) << 4;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_5_GPIO_Port, IO_IN_5_Pin)) << 5;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_6_GPIO_Port, IO_IN_6_Pin)) << 6;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_7_GPIO_Port, IO_IN_7_Pin)) << 7;
      datas[b1] = gpio;

      gpio = 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_8_GPIO_Port, IO_IN_8_Pin)) << 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_9_GPIO_Port, IO_IN_9_Pin)) << 1;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_10_GPIO_Port, IO_IN_10_Pin)) << 2;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_11_GPIO_Port, IO_IN_11_Pin)) << 3;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_12_GPIO_Port, IO_IN_12_Pin)) << 4;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_13_GPIO_Port, IO_IN_13_Pin)) << 5;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_14_GPIO_Port, IO_IN_14_Pin)) << 6;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_15_GPIO_Port, IO_IN_15_Pin)) << 7;
      datas[b2] = gpio;

      m_in[bank_main_1616] = *(reinterpret_cast<uint16_t *>(datas.data()));
    }

    // extern io with i2c
    {
      if (m_isExtComm == false)
        return;

      mcu_io_align_12_t receive_data{};

      if (m_cfg.i2cReadBytes(m_cfg.ex_io_i2c_ch, m_cfg.io_dev_addr, IICOMM_CMD_READ_MCD_DATA, (uint8_t *)&receive_data, sizeof(receive_data), 100) == false)
      { 
        m_errCnt++;
        //LOG_PRINT("[NG] m_cfg.i2cReadData fail! ");
        if (m_errCnt > def_err_cnt_max)
        {
          m_errCnt = 0;
          if (m_cfg.i2cRecovery(m_cfg.ex_io_i2c_ch))
            LOG_PRINT("[OK] m_cfg.i2cRecovery ! ");
          else
            LOG_PRINT("[NG] m_cfg.i2cRecovery fail! ");
        }
      }
      else
      {
        m_in[bank_ext_1616] = LoWORD(receive_data.io_in[0]);
        m_out[bank_ext_1616] = LoWORD(receive_data.io_out[0]);
      }
    }

    AP_IO_LOCK_END;
  }

  /* IO  control function */
  inline bool IsOn(uint32_t addr)
  {
    Update_io();

    if (addr < AP_DEF_START_OUT_ADDR) // Input
    {
      uint32_t data = MaKEDWORD(m_in[bank_main_1616], m_in[bank_ext_1616]);
      return (data >> (addr - AP_DEF_START_IN_ADDR)) & 1;
    }
    // Output
    uint32_t data = MaKEDWORD(m_out[bank_main_1616], m_out[bank_ext_1616]);
    return (data >> (addr - AP_DEF_START_OUT_ADDR)) & 1;
  }

  inline bool IsOff(uint32_t addr)
  {
    return !IsOn(addr);
  }

  inline errno_t OutputOn(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr));
  }

  inline errno_t OutputOff(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr), false);
  }

  inline errno_t OutputToggle(uint32_t addr)
  {
    if (IsOn(addr))
      return OutputOff(addr);
    else
      return OutputOn(addr);
  }
#else
  // _USE_EXHW_IICOMM_DEV

  enum in_e // in_name
  {
    in_0x00 = AP_DEF_START_IN_ADDR,
    in_0x01,
    in_0x02,
    in_0x03,
    in_0x04,
    in_0x05,
    in_0x06,
    in_0x07,

    in_0x10, //
    in_0x11,
    in_0x12,
    in_0x13,
    in_0x14,
    in_0x15,
    in_0x16,
    in_0x17,
  };
  enum out_e // out_name
  {
    out_0x00 = AP_DEF_START_OUT_ADDR,
    out_0x01,
    out_0x02,
    out_0x03,
    out_0x04,
    out_0x05,
    out_0x06,
    out_0x07,

    out_0x10, //
    out_0x11,
    out_0x12,
    out_0x13,
    out_0x14,
    out_0x15,
    out_0x16,
    out_0x17,
  };
  static constexpr uint8_t def_io_port_of_bank = 16;
  static constexpr uint32_t def_in_start_addr = AP_DEF_START_IN_ADDR;
  static constexpr uint32_t def_out_start_addr = AP_DEF_START_OUT_ADDR;
#ifdef _USE_HW_RTOS
  osMutexId ap_io_mutex_id;
#endif

  struct cfg_t
  {
    uint8_t ex_io_i2c_ch{};
    uint16_t io_dev_addr{};

    cfg_t() = default;
    ~cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t &other) = default;
    // copy assignment
    cfg_t &operator=(const cfg_t &other) = default;
    // move constructor
    cfg_t(cfg_t &&other) = default;
    // move assignment
    cfg_t &operator=(cfg_t &&other) = default;
  } m_cfg{};

public:
  volatile uint16_t m_in{};
  volatile uint16_t m_out{};

public:
  ap_io() : m_in{}, m_out{} /*, m_sysio{}, m_errCnt{}*/
  {
#ifdef _USE_HW_RTOS
    osMutexDef(ap_io_mutex_id);
    ap_io_mutex_id = osMutexCreate(osMutex(ap_io_mutex_id));
#endif
  }
  ~ap_io() {}

  inline errno_t Init(cfg_t cfg)
  {
    m_cfg = cfg;

   
    return ERROR_SUCCESS;
  }

  // Update information that has changed compared to previous data
  inline errno_t SetOutputReg(uint16_t reg)
  {
    uint16_t x_reg = reg ^ m_out;
    // LOG_PRINT("x_reg[0x%04X]", x_reg);
    for (uint8_t i = 0; i < def_io_port_of_bank; i++)
      if ((x_reg >> (i)) & 1)
        SetGpioOut((i), ((reg >> (i)) & 1));

    return ERROR_SUCCESS;
  }  

  inline errno_t SetGpioOut(uint16_t out_bit, bool onoff = true)
  {
    AP_IO_LOCK_BEGIN;
    LOG_PRINT(" out_bit[0x%04X]", out_bit);
    GPIO_PinState state = onoff ? GPIO_OUT_STATE_ON : GPIO_OUT_STATE_OFF;
    switch ((out_e)(out_bit + def_out_start_addr))
    {
    case out_0x00:
      HAL_GPIO_WritePin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin, state);
      break;
    case out_0x01:
      HAL_GPIO_WritePin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin, state);
      break;
    case out_0x02:
      HAL_GPIO_WritePin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin, state);
      break;
    case out_0x03:
      HAL_GPIO_WritePin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin, state);
      break;
    case out_0x04:
      HAL_GPIO_WritePin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin, state);
      break;
    case out_0x05:
      HAL_GPIO_WritePin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin, state);
      break;
    case out_0x06:
      HAL_GPIO_WritePin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin, state);
      break;
    case out_0x07:
      HAL_GPIO_WritePin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin, state);
      break;
    case out_0x10:
      HAL_GPIO_WritePin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin, state);
      break;
    case out_0x11:
      HAL_GPIO_WritePin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin, state);
      break;
    case out_0x12:
      HAL_GPIO_WritePin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin, state);
      break;
    case out_0x13:
      HAL_GPIO_WritePin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin, state);
      break;
    case out_0x14:
      HAL_GPIO_WritePin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin, state);
      break;
    case out_0x15:
      HAL_GPIO_WritePin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin, state);
      break;
    case out_0x16:
      HAL_GPIO_WritePin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin, state);
      break;
    case out_0x17:
      HAL_GPIO_WritePin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin, state);
      break;
    default:
      return -1;
    }
    // end of switch

    AP_IO_LOCK_END;
    return ERROR_SUCCESS;
  }

  inline void Update_io(void)
  {

    auto in_state = [](auto state) -> uint8_t
    {
      return (state == GPIO_IN_STATE_ON) ? 1 : 0;
    };

    {
      volatile uint8_t gpio{};
      // Out
      enum Bank
      {
        b1,
        b2,
        b_max
      };
      std::array<uint8_t, b_max> datas{};
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin) << 7;
      datas[b1] = gpio;

      gpio = 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin) << 7;
      datas[b2] = gpio;

      m_out = *(reinterpret_cast<uint16_t *>(datas.data()));

      // In
      datas.fill(0);
      gpio = 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_0_GPIO_Port, IO_IN_0_Pin)) << 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_1_GPIO_Port, IO_IN_1_Pin)) << 1;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_2_GPIO_Port, IO_IN_2_Pin)) << 2;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_3_GPIO_Port, IO_IN_3_Pin)) << 3;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_4_GPIO_Port, IO_IN_4_Pin)) << 4;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_5_GPIO_Port, IO_IN_5_Pin)) << 5;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_6_GPIO_Port, IO_IN_6_Pin)) << 6;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_7_GPIO_Port, IO_IN_7_Pin)) << 7;
      datas[b1] = gpio;

      gpio = 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_8_GPIO_Port, IO_IN_8_Pin)) << 0;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_9_GPIO_Port, IO_IN_9_Pin)) << 1;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_10_GPIO_Port, IO_IN_10_Pin)) << 2;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_11_GPIO_Port, IO_IN_11_Pin)) << 3;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_12_GPIO_Port, IO_IN_12_Pin)) << 4;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_13_GPIO_Port, IO_IN_13_Pin)) << 5;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_14_GPIO_Port, IO_IN_14_Pin)) << 6;
      gpio |= in_state(HAL_GPIO_ReadPin(IO_IN_15_GPIO_Port, IO_IN_15_Pin)) << 7;
      datas[b2] = gpio;

      m_in = *(reinterpret_cast<uint16_t *>(datas.data()));
    }
    AP_IO_LOCK_END;
  }

  /* IO  control function */
  inline bool IsOn(uint32_t addr)
  {
    Update_io();

    if (addr < AP_DEF_START_OUT_ADDR) // Input
    {
      return (m_in >> (addr - AP_DEF_START_IN_ADDR)) & 1;
    }
    // Output
    return (m_out >> (addr - AP_DEF_START_OUT_ADDR)) & 1;
  }

  inline bool IsOff(uint32_t addr)
  {
    return !IsOn(addr);
  }

  inline errno_t OutputOn(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr));
  }

  inline errno_t OutputOff(uint32_t addr)
  {
    if (addr < def_out_start_addr)
      return -1;
    return SetGpioOut((addr - def_out_start_addr), false);
  }

  inline errno_t OutputToggle(uint32_t addr)
  {
    if (IsOn(addr))
      return OutputOff(addr);
    else
      return OutputOn(addr);
  }

#endif
};


#endif

#endif /* AP__INC_AP_IO_HPP_ */
