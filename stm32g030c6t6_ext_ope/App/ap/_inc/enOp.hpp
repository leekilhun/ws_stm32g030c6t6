/*
 * enOp.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 * 
 *  edit : 24.02.03 ext_op
 */

#pragma once
#ifndef AP__INC_ENOP_HPP_
#define AP__INC_ENOP_HPP_

#include "ap_def.hpp"

#ifdef _USE_APP_ENOP

struct enOp
{
  enum mode_e :uint8_t
  {
    READY = 0x10,
    AUTORUN,
    STOP,
    DRY_RUN,
  };

  enum status_e :uint8_t
  {
    INIT = 0x20,
    ERR_STOP,
    STEP_STOP,
    RUN_READY,
    RUN,
  };

  enum panel_e :uint8_t
  {
    SW_START = 0x30,
    SW_STOP,
    SW_RESET,
    SW_ESTOP,
  };

  enum lamp_e :uint8_t
  {
    LAMP_START = 0x40,
    LAMP_STOP,
    LAMP_RESET,
    LAMP_ALL
  };

  struct cfg_t
  {
    ap_reg *ptr_mcu_reg{};
    ap_io  *ptr_mcu_io{};

    enLed *ptr_led_status{};
    enLed *ptr_led_op_g{};
    enLed *ptr_led_op_y{};
    enLed *ptr_led_op_r{};

    enBtn *ptr_btn_start{};
    enBtn *ptr_btn_stop{};
    enBtn *ptr_btn_reset{};
    enBtn *ptr_btn_estop{};

    GPIO_TypeDef *tower_buzzer_gpio_port{};
    uint16_t      tower_buzzer_gpio_pin{};

    GPIO_TypeDef *sensor_safety_1_gpio_port{};
    uint16_t      sensor_safety_1_gpio_pin{};

    GPIO_TypeDef *sensor_safety_2_gpio_port{};
    uint16_t      sensor_safety_2_gpio_pin{};

    cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t& rhs) = default;
    // copy assignment operator
    cfg_t& operator=(const cfg_t& rhs) = default;
    // move constructor
    cfg_t(cfg_t&& rhs) = default;
    // move assignment operator
    cfg_t& operator=(cfg_t&& rhs) = default;
  };

  volatile enOp::status_e m_status;
  volatile enOp::mode_e m_mode;
  enOp::cfg_t m_cfg;

public:
  enOp() : m_status(enOp::status_e::INIT), m_mode (enOp::mode_e::STOP){ }
  ~enOp(){}
  inline int Init(enOp::cfg_t& cfg){
    LOG_PRINT("[OK] Init Success!");
    m_cfg = cfg;
    return 0;
  }

  inline enOp::status_e GetStatus() const{
    return m_status;
  }

  inline void SetStatus(enOp::status_e state){
    // 
    if (m_status == enOp::INIT )
    {
      switch (state)
      {
      case enOp::ERR_STOP:
        break;
      case enOp::STEP_STOP:
        LampOnOff(enOp::LAMP_START, false);
        LampOnOff(enOp::LAMP_STOP, false);
        LampOnOff(enOp::LAMP_RESET, false);
        break;
      case enOp::RUN_READY:
        break;
      case enOp::RUN:
        break;

      default:
        break;
      }
    }
    else
    {
      switch (state)
      {
      case enOp::ERR_STOP:
        LampOnOff(enOp::LAMP_STOP);
        break;
      case enOp::STEP_STOP:   
      case enOp::RUN_READY:
      case enOp::RUN:      
        break;

      default:
        break;
      }

    } 
    m_status = state;
  }

  inline enOp::mode_e GetMode() const{
    return m_mode;
  }

  inline void SetMode(enOp::mode_e md){
  	m_mode = md;
  }  


  inline void LampOnOff(lamp_e lamp, bool state = true)
  {
    switch (lamp)
    {
    case LAMP_START:
      state ? m_cfg.ptr_led_op_g->On() : m_cfg.ptr_led_op_g->Off();
      break;
    case LAMP_STOP:
      state ? m_cfg.ptr_led_op_r->On() : m_cfg.ptr_led_op_r->Off();
      break;
    case LAMP_RESET:
      state ? m_cfg.ptr_led_op_y->On() : m_cfg.ptr_led_op_y->Off();
      break;
    default:
      break;
    };
  }

  inline void LampToggle(lamp_e lamp)
  {
    switch (lamp)
    {
    case LAMP_START:
      m_cfg.ptr_led_op_g->Toggle();
      break;
    case LAMP_STOP:
      m_cfg.ptr_led_op_r->Toggle();
      break;
    case LAMP_RESET:
      m_cfg.ptr_led_op_y->Toggle();
      break;
    case LAMP_ALL:
      m_cfg.ptr_led_op_r->Toggle();
      m_cfg.ptr_led_op_g->Toggle();
      m_cfg.ptr_led_op_y->Toggle();
      break;
    default:
      break;
    };
  }
};


#endif /* _USE_APP_ENOP */


#endif /* AP__INC_ENOP_HPP_ */
