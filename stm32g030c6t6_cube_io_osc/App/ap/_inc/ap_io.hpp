/*
 * ap_io.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_IO_HPP_
#define AP__INC_AP_IO_HPP_



#ifdef _USE_HW_RTOS
#define AP_IO_LOCK_BEGIN osMutexWait(ap_io_mutex_id, osWaitForever)
#define AP_IO_LOCK_END   osMutexRelease(ap_io_mutex_id)
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
  enum in_e// in_name
  {
    in_00 = AP_DEF_START_IN_ADDR,
    in_01,
    in_mode_key_manual,
    in_modde_key_auto,
    in_04,
    in_05,
    in_06,
    in_safety_sensor, //

    in10, // reserved
    in11,
    in12,
    in13,
    in14,
    in15,
    in16,
    in17,

    in20, // ext_io 1
    in21,
    in22,
    in23,
    in24,
    in25,
    in26,
    in27,

    in30,
    in31,
    in32,
    in33,
    in34,
    in35,
    in36,
    in37,

  };



  union in_u
  {
    volatile  uint32_t data{};
    struct
    {
      volatile unsigned in_00:1;
      volatile unsigned in_01:1;
      volatile unsigned in_mode_key_manual:1;
      volatile unsigned in_mode_key_auto:1;
      volatile unsigned in04:1;
      volatile unsigned in05:1;
      volatile unsigned in06:1;
      volatile unsigned in_safety_sensor:1;

      volatile unsigned in10:1;
      volatile unsigned in11:1;
      volatile unsigned in12:1;
      volatile unsigned in13:1;
      volatile unsigned in14:1;
      volatile unsigned in15:1;
      volatile unsigned in16:1;
      volatile unsigned in17:1;

      volatile unsigned in_20:1;
      volatile unsigned in_21:1;
      volatile unsigned in_22:1;
      volatile unsigned in_23:1;
      volatile unsigned in_24:1;
      volatile unsigned in_25:1;
      volatile unsigned in_26:1;
      volatile unsigned in_27:1;

      volatile unsigned in30:1;
      volatile unsigned in31:1;
      volatile unsigned in32:1;
      volatile unsigned in33:1;
      volatile unsigned in34:1;
      volatile unsigned in35:1;
      volatile unsigned in36:1;
      volatile unsigned in37:1;
    };
  };

  enum out_e//out_name
  {
    out00 = AP_DEF_START_OUT_ADDR,
    out01,
    out02,
    out03,
    out04,
    out05,
    out06,
    out07,

    out10, // reserved
    out11,
    out12,
    out13,
    out14,
    out15,
    out16,
    out17,

    out20, // ext_io 1
    out21,
    out22,
    out23,
    out24,
    out25,
    out26,
    out27,

    out30,
    out31,
    out32,
    out33,
    out34,
    out35,
    out36,
    out37,
  };
  // 1bank 4byte
  union out_u
  {
    volatile  uint32_t data{};
    struct
    {
      volatile unsigned out00:1;
      volatile unsigned out01:1;
      volatile unsigned out02:1;
      volatile unsigned out03:1;
      volatile unsigned out04:1;
      volatile unsigned out05:1;
      volatile unsigned out06:1;
      volatile unsigned out07:1;

      volatile unsigned out10:1;
      volatile unsigned out11:1;
      volatile unsigned out12:1;
      volatile unsigned out13:1;
      volatile unsigned out14:1;
      volatile unsigned out15:1;
      volatile unsigned out16:1;
      volatile unsigned out17:1;

      volatile unsigned out20:1;
      volatile unsigned out21:1;
      volatile unsigned out22:1;
      volatile unsigned out23:1;
      volatile unsigned out24:1;
      volatile unsigned out25:1;
      volatile unsigned out26:1;
      volatile unsigned out27:1;

      volatile unsigned out30:1;
      volatile unsigned out31:1;
      volatile unsigned out32:1;
      volatile unsigned out33:1;
      volatile unsigned out34:1;
      volatile unsigned out35:1;
      volatile unsigned out36:1;
      volatile unsigned out37:1;
    };
  };

  union sysio_u
  {
    uint16_t system_io{};
    struct
    {
      volatile unsigned start_sw:1;
      volatile unsigned stop_sw:1;
      volatile unsigned reset_sw:1;
      volatile unsigned estop_Sw:1;
      volatile unsigned in04:1;
      volatile unsigned in05:1;
      volatile unsigned in06:1;
      volatile unsigned in07:1;

      volatile unsigned start_lamp:1;
      volatile unsigned stop_lamp:1;
      volatile unsigned reset_lamp:1;
      volatile unsigned out03:1;
      volatile unsigned out04:1;
      volatile unsigned out05:1;
      volatile unsigned out06:1;
      volatile unsigned out07:1;

    };
  };



  volatile in_u m_in;
  volatile out_u m_out;
  volatile sysio_u m_sysio;
  volatile uint16_t m_extIn;
  volatile uint16_t m_extOut;

  uint8_t m_errCnt;
public:
  ap_io() : m_in{}, m_out{} ,m_sysio{}, m_extIn{0}, m_extOut{0} ,m_errCnt{}{
#ifdef _USE_HW_RTOS
    osMutexDef(ap_io_mutex_id);
    ap_io_mutex_id = osMutexCreate (osMutex(ap_io_mutex_id));
#endif
  }
  virtual ~ap_io(){}

  inline int Init(){
    pca8575pw_Read(_PCA8575PW_EX_IO_CH_OUT, (uint16_t*)&m_extOut);
    LOG_PRINT("Init Success!");
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

  inline int SetGpioOut(out_e out_idx, bool onoff = true){
    if ((uint32_t)out_idx < AP_DEF_START_OUT_ADDR)
      return -1;

    AP_IO_LOCK_BEGIN;
    auto is_ext = [&](auto idx) ->bool
        {
          return ((uint32_t)out17 <= (uint32_t)idx);
        };

    if (is_ext(out_idx))
    {
      uint32_t no = 0;
      if (onoff)
      {
        no = 1<<((uint32_t)out_idx - (uint32_t)out20 );
        m_extOut = m_extOut | no;
      }
      else
      {
        no = ~(1<<((uint32_t)out_idx - (uint32_t)out20  ));
        m_extOut = m_extOut & no;
      }
    }
    else
    {
      switch (out_idx)
      {
        case out00:
          gpioPinWrite(_GPIO_IO_OUT_01, onoff);
          break;
        case out01:
          gpioPinWrite(_GPIO_IO_OUT_02, onoff);
          break;
        case out02:
          gpioPinWrite(_GPIO_IO_OUT_03, onoff);
          break;
        case out03:
          gpioPinWrite(_GPIO_IO_OUT_04, onoff);
          break;
        case out04:
          gpioPinWrite(_GPIO_IO_OUT_05, onoff);
          break;
        case out05:
          gpioPinWrite(_GPIO_IO_OUT_06, onoff);
          break;
        case out06:
          gpioPinWrite(_GPIO_IO_OUT_07, onoff);
          break;
        case out07:
          gpioPinWrite(_GPIO_IO_OUT_08, onoff);
          break;
        default:
          return -1;
      }
      // end of switch
    }

    AP_IO_LOCK_END;
    return ERROR_SUCCESS;
  }


  inline void Update_io(void){
    enum Bank { b1, b2, b3, b4, b_max };
    std::array<uint8_t, b_max> data {};


    uint8_t gpio{};
    // Out
    {
      gpio |= gpioPinRead(_GPIO_IO_OUT_01) << 0;
      gpio |= gpioPinRead(_GPIO_IO_OUT_02) << 1;
      gpio |= gpioPinRead(_GPIO_IO_OUT_03) << 2;
      gpio |= gpioPinRead(_GPIO_IO_OUT_04) << 3;
      gpio |= gpioPinRead(_GPIO_IO_OUT_05) << 4;
      gpio |= gpioPinRead(_GPIO_IO_OUT_06) << 5;
      gpio |= gpioPinRead(_GPIO_IO_OUT_07) << 6;
      gpio |= gpioPinRead(_GPIO_IO_OUT_08) << 7;
      data[b1] = gpio;

      data[b3] = static_cast<uint8_t>(m_extOut >> 0);
      data[b4] = static_cast<uint8_t>(m_extOut >> 8);

      m_out.data = (static_cast<uint32_t>(data[b1]) << 24)
                         | (static_cast<uint32_t>(data[b2]) << 16)
                         | (static_cast<uint32_t>(data[b3]) << 8)
                         | static_cast<uint32_t>(data[b4]);

      // Extension IO module
      uint16_t out_data = m_extOut;
      if (!pca8575pw_Write(_PCA8575PW_EX_IO_CH_OUT, ~(out_data)))
        ++m_errCnt;
    }

    // In
    {
      data.fill(0);
      gpio = 0;
      gpio |= gpioPinRead(_GPIO_IO_IN_01) << 0;
      gpio |= gpioPinRead(_GPIO_IO_IN_02) << 1;
      gpio |= gpioPinRead(_GPIO_IO_IN_03) << 2;
      gpio |= gpioPinRead(_GPIO_IO_IN_04) << 3;
      gpio |= gpioPinRead(_GPIO_IO_IN_05) << 4;
      gpio |= gpioPinRead(_GPIO_IO_IN_06) << 5;
      gpio |= gpioPinRead(_GPIO_IO_IN_07) << 6;
      gpio |= gpioPinRead(_GPIO_IO_IN_08) << 7;
      data[b1] = gpio;

      // Extension IO module
      if (pca8575pw_Read(_PCA8575PW_EX_IO_CH_IN, (uint16_t*)&m_extIn))
      {
        data[b3] = static_cast<uint8_t>(m_extIn >> 0);
        data[b4] = static_cast<uint8_t>(m_extIn >> 8);
      } else
        ++m_errCnt;


      m_in.data = (static_cast<uint32_t>(data[b1]) << 24)
                    | (static_cast<uint32_t>(data[b2]) << 16)
                    | (static_cast<uint32_t>(data[b3]) << 8)
                    | static_cast<uint32_t>(data[b4]);
    }

    // Check I2C communication recovery
    constexpr uint8_t limit_recovery_cnt = 100;
    if (m_errCnt >= limit_recovery_cnt)
    {
      m_errCnt = 0;
      pca8575pw_Recovery();
      Init();
    }

    AP_IO_LOCK_END;
  }



  /* IO  control function */
  inline bool IsOn(uint32_t addr) {
    Update_io();

    if (addr < AP_DEF_START_OUT_ADDR) // Input
      return (m_in.data >> (addr - AP_DEF_START_IN_ADDR)) & 1;
    else // Output
      return (m_out.data >> (addr - AP_DEF_START_OUT_ADDR)) & 1;
  }

  inline bool IsOff(uint32_t addr) {
    return !IsOn(addr);
  }

  inline int OutputOn(uint32_t addr) {
    return SetGpioOut((out_e)addr);
  }

  inline int OutputOff(uint32_t addr) {
    return SetGpioOut((out_e)addr, false);
  }

  inline int OutputToggle(uint32_t addr){
    int ret = ERROR_SUCCESS;
    if (IsOn(addr))
      ret = OutputOff(addr);
    else
      ret = OutputOn(addr);
    return ret;
  }


};


#endif /* AP__INC_AP_IO_HPP_ */
