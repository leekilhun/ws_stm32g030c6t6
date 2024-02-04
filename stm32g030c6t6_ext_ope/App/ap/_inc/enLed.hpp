/*
 * enLed.hpp
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 * 
 *  edit :24.02.03 ext_op
 */
#pragma once
#ifndef AP__INC_ENLED_HPP_
#define AP__INC_ENLED_HPP_


#include "ap_def.hpp"

struct enLed
{
  struct cfg_t
  {
    GPIO_TypeDef * gpio_port{};
    uint16_t       gpio_pin{};
    char name_str[32]{};

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

    char *set_name(const char *name)
    {
      memset(&name_str[0], 0, sizeof(name_str));
      strlcpy(&name_str[0], name, sizeof(name_str));
      return &name_str[0];
    }

  } m_cfg{};

public:
  enLed() = default;
  ~enLed()= default;
  inline error_t Init(cfg_t& cfg){
    LOG_PRINT("[OK] Init Success! LED [%s], pin [%d], port[%d]",cfg.name_str, cfg.gpio_pin, cfg.gpio_port);
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
