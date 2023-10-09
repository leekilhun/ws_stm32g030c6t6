/*
 * enLed.hpp
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */
#pragma once
#ifndef AP__INC_ENLED_HPP_
#define AP__INC_ENLED_HPP_


#include "ap_def.h"

struct enLed
{
  struct cfg_t
  {
    GPIO_TypeDef * gpio_port{};
    uint16_t       gpio_pin{};

    cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t& rhs) = default;
    // copy assignment operator
    cfg_t& operator=(const cfg_t& rhs) = default;
    // move constructor
    cfg_t(cfg_t&& rhs) = default;
    // move assignment operator
    cfg_t& operator=(cfg_t&& rhs) = default;

    ~cfg_t() = default;

  } m_cfg{};

public:
  enLed() = default;
  ~enLed()= default;
  inline error_t Init(cfg_t& cfg){
    LOG_PRINT("[OK] Init Success! pin [%d], port[%d]", cfg.gpio_pin, cfg.gpio_port);
    m_cfg = cfg;
    Off();
    return ERROR_SUCCESS;
  }

  inline void  On(){
    HAL_GPIO_WritePin(m_cfg.gpio_port, m_cfg.gpio_pin, GPIO_OUT_STATE_ON );
  }

  inline void Off(){
    HAL_GPIO_WritePin(m_cfg.gpio_port, m_cfg.gpio_pin, GPIO_OUT_STATE_OFF );
  }

  inline void Toggle(){
    HAL_GPIO_TogglePin(m_cfg.gpio_port, m_cfg.gpio_pin);
  }

};


#endif /* AP__INC_ENLED_HPP_ */
