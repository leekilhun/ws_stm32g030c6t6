/*
 * ap_io.hpp
 *
 *  Created on: 2023. 8. 7.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_IO_HPP_
#define AP__INC_AP_IO_HPP_

#ifdef _USE_HW_RTOS
#define AP_IO_LOCK_BEGIN osMutexWait(ap_io_mutex_id, osWaitForever)
#define AP_IO_LOCK_END osMutexRelease(ap_io_mutex_id)
#else
#define AP_IO_LOCK_BEGIN
#define AP_IO_LOCK_END
#endif

struct ap_io
{
#ifdef _USE_HW_RTOS
  osMutexId ap_io_mutex_id;
#endif

public:
  enum in_e // in_name
  {
    in00_ready_sw = AP_DEF_START_IN_ADDR,
    in01_busy_sw,
    in02_error_sw,
    in03,
    in04_a_vac_on,
    in05_b_vac_on,
    in06,
    in07_a_motor_org, //

    in10,
    in11,
    in12_b_motor_org,
    in13,
    in14_c_r_motor_org,
    in15_c_l_motor_org,
    in16,
    in17,
  };

  union in_u
  {
    volatile uint16_t data{};
    struct
    {
      volatile unsigned in00_ready_sw : 1;
      volatile unsigned in01_busy_sw : 1;
      volatile unsigned in02_error_sw : 1;
      volatile unsigned in03 : 1;
      volatile unsigned in04_a_vac_on : 1;
      volatile unsigned in05_b_vac_on : 1;
      volatile unsigned in06 : 1;
      volatile unsigned in07_a_motor_org : 1;

      volatile unsigned in10 : 1;
      volatile unsigned in11 : 1;
      volatile unsigned in12_b_motor_org : 1;
      volatile unsigned in13 : 1;
      volatile unsigned in14_c_r_motor_org : 1;
      volatile unsigned in15_c_l_motor_org : 1;
      volatile unsigned in16 : 1;
      volatile unsigned in17 : 1;
    };
  };

  enum out_e // out_name
  {
    out00_ready_lamp = AP_DEF_START_OUT_ADDR,
    out01_busy_lamp,
    out02_error_lamp,
    out03_a_vac_suction,
    out04_a_vac_blow,
    out05_b_vac_suction,
    out06_b_vac_blow,
    out07_a_motor_enable,

    out10_a_motor_direction, // reserved
    out11_a_motor_pulse,
    out12_b_motor_enable,
    out13_b_motor_direction,
    out14_b_motor_pulse,
    out15_c_motor_enalble,
    out16_c_motor_direction,
    out17_c_motor_pulse,
  };

  union out_u
  {
    volatile uint16_t data{};
    struct
    {
      volatile unsigned out00_ready_lamp : 1;
      volatile unsigned out01_busy_lamp : 1;
      volatile unsigned out02_error_lamp : 1;
      volatile unsigned out03_a_vac_suction : 1;
      volatile unsigned out04_a_vac_blow : 1;
      volatile unsigned out05_b_vac_suction : 1;
      volatile unsigned out06_b_vac_blow : 1;
      volatile unsigned out07_a_motor_enable : 1;

      volatile unsigned out10_a_motor_direction : 1;
      volatile unsigned out11_a_motor_pulse : 1;
      volatile unsigned out12_b_motor_enable : 1;
      volatile unsigned out13_b_motor_direction : 1;
      volatile unsigned out14_b_motor_pulse : 1;
      volatile unsigned out15_c_motor_enalble : 1;
      volatile unsigned out16_c_motor_direction : 1;
      volatile unsigned out17_c_motor_pulse : 1;
    };
  };

  union sysio_u
  {
    uint16_t system_io{};
    struct
    {
      volatile unsigned start_sw : 1;
      volatile unsigned reset_sw : 1;
      volatile unsigned stop_sw : 1;
      volatile unsigned estop_Sw : 1;
      volatile unsigned in04 : 1;
      volatile unsigned in05 : 1;
      volatile unsigned in06 : 1;
      volatile unsigned in07 : 1;

      volatile unsigned ready_lamp : 1;
      volatile unsigned busy_lamp : 1;
      volatile unsigned error_lamp : 1;
      volatile unsigned out03 : 1;
      volatile unsigned out04 : 1;
      volatile unsigned out05 : 1;
      volatile unsigned out06 : 1;
      volatile unsigned out07 : 1;
    };
  };

  volatile in_u m_in{};
  volatile out_u m_out{};
  volatile sysio_u m_sysio{};

  uint8_t m_errCnt{};

public:
  ap_io() : m_in{}, m_out{}, m_sysio{}, m_errCnt{}
  {
#ifdef _USE_HW_RTOS
    osMutexDef(ap_io_mutex_id);
    ap_io_mutex_id = osMutexCreate(osMutex(ap_io_mutex_id));
#endif
  }
  ~ap_io() {}

  inline int Init()
  {
    LOG_PRINT("[OK] Init Success!");
    return ERROR_SUCCESS;
  }

  /*inline void assignObj(iio* p_io){
    pIo = p_io;
 }*/

   //Update information that has changed compared to previous data
  inline void SetOutputReg(uint32_t reg, uint8_t bank = 0)
  {
    uint32_t x_reg = reg ^ m_out.data;

    for(uint8_t i = 0 ; i < 32; i++)
    {
      if((x_reg >>(i))&1)
      {
        SetGpioOut((out_e)(i+AP_DEF_START_OUT_ADDR),((reg>>(i))&1));
      }
    }
  }

  inline int SetGpioOut(out_e out_idx, bool onoff = true)
  {
    if ((uint32_t)out_idx < AP_DEF_START_OUT_ADDR)
      return -1;
    AP_IO_LOCK_BEGIN;

    GPIO_PinState state = onoff ? GPIO_OUT_STATE_ON : GPIO_OUT_STATE_OFF;
    switch (out_idx)
    {
    case out00_ready_lamp:
      HAL_GPIO_WritePin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin, state);
      break;
    case out01_busy_lamp:
      HAL_GPIO_WritePin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin, state);
      break;
    case out02_error_lamp:
      HAL_GPIO_WritePin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin, state);
      break;
    case out03_a_vac_suction:
      HAL_GPIO_WritePin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin, state);
      break;
    case out04_a_vac_blow:
      HAL_GPIO_WritePin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin, state);
      break;
    case out05_b_vac_suction:
      HAL_GPIO_WritePin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin, state);
      break;
    case out06_b_vac_blow:
      HAL_GPIO_WritePin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin, state);
      break;
    case out07_a_motor_enable:
      HAL_GPIO_WritePin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin, state);
      break;
    case out10_a_motor_direction:
      HAL_GPIO_WritePin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin, state);
      break;
    case out11_a_motor_pulse:
      HAL_GPIO_WritePin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin, state);
      break;
    case out12_b_motor_enable:
      HAL_GPIO_WritePin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin, state);
      break;
    case out13_b_motor_direction:
      HAL_GPIO_WritePin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin, state);
      break;
    case out14_b_motor_pulse:
      HAL_GPIO_WritePin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin, state);
      break;
    case out15_c_motor_enalble:
      HAL_GPIO_WritePin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin, state);
      break;
    case out16_c_motor_direction:
      HAL_GPIO_WritePin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin, state);
      break;
    case out17_c_motor_pulse:
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
    enum Bank
    {
      b1,
      b2,
      b_max
    };
    std::array<uint8_t, b_max> datas{};
    auto in_state = [](auto state) -> uint8_t
    {
      return (state == GPIO_IN_STATE_ON) ? 1 : 0;
    };

    volatile uint8_t gpio{};
    // Out
    {
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

      m_out.data = *(reinterpret_cast<uint16_t *>(datas.data()));
    }

    // In
    {
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
      
      m_in.data = *(reinterpret_cast<uint16_t *>(datas.data()));
    }

    AP_IO_LOCK_END;
  }

  /* IO  control function */
  inline bool IsOn(uint32_t addr)
  {
    Update_io();

    if (addr < AP_DEF_START_OUT_ADDR) // Input
      return (m_in.data >> (addr - AP_DEF_START_IN_ADDR)) & 1;
    else // Output
      return (m_out.data >> (addr - AP_DEF_START_OUT_ADDR)) & 1;
  }

  inline bool IsOff(uint32_t addr)
  {
    return !IsOn(addr);
  }

  inline int OutputOn(uint32_t addr)
  {
    return SetGpioOut((out_e)addr);
  }

  inline int OutputOff(uint32_t addr)
  {
    return SetGpioOut((out_e)addr, false);
  }

  inline int OutputToggle(uint32_t addr)
  {
    int ret = ERROR_SUCCESS;
    if (IsOn(addr))
      ret = OutputOff(addr);
    else
      ret = OutputOn(addr);
    return ret;
  }
};

#endif /* AP__INC_AP_IO_HPP_ */
