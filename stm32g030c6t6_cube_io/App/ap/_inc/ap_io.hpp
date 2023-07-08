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
    in00 = AP_DEF_START_IN_ADDR,
    in01,
    in02,
    in03,
    in04,
    in05,
    in06,
    in07, //

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
      volatile unsigned in_02:1;
      volatile unsigned in_03:1;
      volatile unsigned in_04:1;
      volatile unsigned in_05:1;
      volatile unsigned in_06:1;
      volatile unsigned in_07:1;

      volatile unsigned in_10:1;
      volatile unsigned in_11:1;
      volatile unsigned in_12:1;
      volatile unsigned in_13:1;
      volatile unsigned in_14:1;
      volatile unsigned in_15:1;
      volatile unsigned in_16:1;
      volatile unsigned in_17:1;

      volatile unsigned in_20:1;
      volatile unsigned in_21:1;
      volatile unsigned in_22:1;
      volatile unsigned in_23:1;
      volatile unsigned in_24:1;
      volatile unsigned in_25:1;
      volatile unsigned in_26:1;
      volatile unsigned in_27:1;

      volatile unsigned in_30:1;
      volatile unsigned in_31:1;
      volatile unsigned in_32:1;
      volatile unsigned in_33:1;
      volatile unsigned in_34:1;
      volatile unsigned in_35:1;
      volatile unsigned in_36:1;
      volatile unsigned in_37:1;
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
      volatile unsigned out_00:1;
      volatile unsigned out_01:1;
      volatile unsigned out_02:1;
      volatile unsigned out_03:1;
      volatile unsigned out_04:1;
      volatile unsigned out_05:1;
      volatile unsigned out_06:1;
      volatile unsigned out_07:1;

      volatile unsigned out_10:1;
      volatile unsigned out_11:1;
      volatile unsigned out_12:1;
      volatile unsigned out_13:1;
      volatile unsigned out_14:1;
      volatile unsigned out_15:1;
      volatile unsigned out_16:1;
      volatile unsigned out_17:1;

      volatile unsigned out_20:1;
      volatile unsigned out_21:1;
      volatile unsigned out_22:1;
      volatile unsigned out_23:1;
      volatile unsigned out_24:1;
      volatile unsigned out_25:1;
      volatile unsigned out_26:1;
      volatile unsigned out_27:1;

      volatile unsigned out_30:1;
      volatile unsigned out_31:1;
      volatile unsigned out_32:1;
      volatile unsigned out_33:1;
      volatile unsigned out_34:1;
      volatile unsigned out_35:1;
      volatile unsigned out_36:1;
      volatile unsigned out_37:1;
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
    LOG_PRINT("Init Success!");
    return ERROR_SUCCESS;
  }

  /*inline void assignObj(iio* p_io){
    pIo = p_io;
 }*/


  inline int SetGpioOut(out_e out_idx, bool onoff = true){
    if ((uint32_t)out_idx < AP_DEF_START_OUT_ADDR)
      return -1;
    GPIO_PinState state = onoff?GPIO_PIN_SET:GPIO_PIN_RESET;
    AP_IO_LOCK_BEGIN;

      switch (out_idx)
      {
        case out00:
          HAL_GPIO_WritePin(IO_OUT_0_GPIO_Port, IO_OUT_0_Pin,state);
          break;
        case out01:
          HAL_GPIO_WritePin(IO_OUT_1_GPIO_Port, IO_OUT_1_Pin,state);
          break;
        case out02:
          HAL_GPIO_WritePin(IO_OUT_2_GPIO_Port, IO_OUT_2_Pin,state);
          break;
        case out03:
          HAL_GPIO_WritePin(IO_OUT_3_GPIO_Port, IO_OUT_3_Pin,state);
          break;
        case out04:
          HAL_GPIO_WritePin(IO_OUT_4_GPIO_Port, IO_OUT_4_Pin,state);
          break;
        case out05:
          HAL_GPIO_WritePin(IO_OUT_5_GPIO_Port, IO_OUT_5_Pin,state);
          break;
        case out06:
          HAL_GPIO_WritePin(IO_OUT_6_GPIO_Port, IO_OUT_6_Pin,state);
          break;
        case out07:
          HAL_GPIO_WritePin(IO_OUT_7_GPIO_Port, IO_OUT_7_Pin,state);
          break;
        case out10:
          HAL_GPIO_WritePin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin,state);
          break;
        case out11:
          HAL_GPIO_WritePin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin,state);
          break;
        case out12:
          HAL_GPIO_WritePin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin,state);
          break;
        case out13:
          HAL_GPIO_WritePin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin,state);
          break;
        case out14:
          HAL_GPIO_WritePin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin,state);
          break;
        case out15:
          HAL_GPIO_WritePin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin,state);
          break;
        case out16:
          HAL_GPIO_WritePin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin,state);
          break;
        case out17:
          HAL_GPIO_WritePin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin,state);
          break;
        default:
          return -1;
      }
      // end of switch


    AP_IO_LOCK_END;
    return ERROR_SUCCESS;
  }


  inline void Update_io(void){
    enum Bank { b1, b2, b3, b4, b_max };
    std::array<uint8_t, b_max> data {};


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
      data[b1] = gpio;

      gpio = 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_8_GPIO_Port, IO_OUT_8_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_9_GPIO_Port, IO_OUT_9_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_10_GPIO_Port, IO_OUT_10_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_11_GPIO_Port, IO_OUT_11_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_12_GPIO_Port, IO_OUT_12_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_13_GPIO_Port, IO_OUT_13_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_14_GPIO_Port, IO_OUT_14_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_OUT_15_GPIO_Port, IO_OUT_15_Pin) << 7;
      data[b2] = gpio;

      data[b3] = 0;
      data[b4] = 0;

      m_out.data = (static_cast<uint32_t>(data[b4]) << 24)
                              | (static_cast<uint32_t>(data[b3]) << 16)
                              | (static_cast<uint32_t>(data[b2]) << 8)
                              | static_cast<uint32_t>(data[b1]);

    }

    // In
    {
      data.fill(0);
      gpio = 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_0_GPIO_Port, IO_IN_0_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_1_GPIO_Port, IO_IN_1_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_2_GPIO_Port, IO_IN_2_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_3_GPIO_Port, IO_IN_3_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_4_GPIO_Port, IO_IN_4_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_5_GPIO_Port, IO_IN_5_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_6_GPIO_Port, IO_IN_6_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_7_GPIO_Port, IO_IN_7_Pin) << 7;
      data[b1] = ~gpio; //state reverse

      gpio = 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_8_GPIO_Port, IO_IN_8_Pin) << 0;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_9_GPIO_Port, IO_IN_9_Pin) << 1;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_10_GPIO_Port, IO_IN_10_Pin) << 2;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_11_GPIO_Port, IO_IN_11_Pin) << 3;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_12_GPIO_Port, IO_IN_12_Pin) << 4;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_13_GPIO_Port, IO_IN_13_Pin) << 5;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_14_GPIO_Port, IO_IN_14_Pin) << 6;
      gpio |= (uint8_t)HAL_GPIO_ReadPin(IO_IN_15_GPIO_Port, IO_IN_15_Pin) << 7;
      data[b2] = ~gpio;//state reverse

      data[b3] = 0;
      data[b4] = 0;


      m_in.data = (static_cast<uint32_t>(data[b4]) << 24)
                          | (static_cast<uint32_t>(data[b3]) << 16)
                          | (static_cast<uint32_t>(data[b2]) << 8)
                          | static_cast<uint32_t>(data[b1]);

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
