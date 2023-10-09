/*
 * enBtn.hpp
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */
#pragma once
#ifndef AP__INC_ENBTN_HPP_
#define AP__INC_ENBTN_HPP_

#include "ap_def.h"

#ifdef _USE_APP_ENBTN

struct enBtn
{
  struct cfg_t
  {
    GPIO_TypeDef *gpio_port{};
    uint16_t gpio_pin{};
    uint32_t repeat_time_detect{}; // Minimum number of retention counters for noise avoidance
    uint32_t repeat_time_delay{};  // long key count
    uint32_t repeat_time{};        // middle key count
    char name_str[32]{};

    cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t &rhs) = default;
    // copy assignment operator
    cfg_t &operator=(const cfg_t &rhs) = default;
    // move constructor
    cfg_t(cfg_t &&rhs) = default;
    // move assignment operator
    cfg_t &operator=(cfg_t &&rhs) = default;

    ~cfg_t() = default;

  } m_cfg{};

  enum class event_e : uint8_t
  {
    none,
    pressed,
    released,
    repeat,
  } m_event{};
  /*
    struct event_t
    {
      bool is_init{};
      uint8_t level{};
      uint8_t index{};

      bool pressed_event{};
      bool released_event{};
      bool repeat_event{};
    }m_event{};
  */

  bool m_pressed{};                // Change button status to pressed status
  bool m_pressed_event{};          // An event in which the button is pressed has occurred
  uint16_t m_pressed_cnt{};        // It is increased when the button is pressed
  uint32_t m_pressed_start_time{}; // Time when the button is switched to pressed state
  uint32_t m_pressed_end_time{};   // Time when the button is pressed

  bool m_released{};                // Change button status to released status
  bool m_released_event{};          // An event in which the button is released has occurred
  uint32_t m_released_start_time{}; // Time when the button is switched to released state
  uint32_t m_released_end_time{};   // Time when the button is released

  bool m_repeat_update{};  // Start cumulative counter with button pressed
  uint32_t m_repeat_cnt{}; // Accumulated counter information of button pressed status

public:
  enBtn() = default;
  ~enBtn() = default;
  inline error_t Init(cfg_t &cfg)
  {
    m_cfg = cfg;
    LOG_PRINT("[OK] Init Success! BTN_%s pin [%d], port[%d]",cfg.name_str, cfg.gpio_pin, cfg.gpio_port);
    return ERROR_SUCCESS;
  }

  inline bool IsPressed()
  {
    return (HAL_GPIO_ReadPin(m_cfg.gpio_port, m_cfg.gpio_pin) == GPIO_IN_STATE_ON);
  }

  inline bool IsReleased()
  {
    return !IsPressed();
  }

  inline void ISR()
  {
    uint32_t repeat_time;

    if (IsPressed() == true)
    {
      //LOG_PRINT("pressed [%d]",m_pressed );    
      if (m_pressed == false)
      {
        m_pressed_event = true;
        m_pressed_start_time = millis();
        m_event = event_e::pressed;
      }
      //(m_pressed == false)

      m_pressed = true;
      m_pressed_cnt++;

      if (m_repeat_cnt == 0)
      {
        repeat_time = m_cfg.repeat_time_detect;
      }
      else if (m_repeat_cnt == 1)
      {
        repeat_time = m_cfg.repeat_time_delay;
      }
      else
      {
        repeat_time = m_cfg.repeat_time;
      }
      // if (m_repeat_cnt == 0)

      if (m_pressed_cnt >= repeat_time)
      {
        m_pressed_cnt = 0;
        m_repeat_cnt++;
        m_repeat_update = true;
        m_event = event_e::repeat;
      }

      m_pressed_end_time = millis();
      m_released = false;
    }
    else
    {
      if (m_pressed == true)
      {
        m_released_event = true;
        m_released_start_time = millis();
        m_event = event_e::released;
      }
      // if (m_pressed == true)

      m_pressed = false;
      m_released = true;
      m_repeat_cnt = 0;
      m_repeat_update = false;

      m_released_end_time = millis();
    }
  }

  inline uint32_t GetPressedTime() const
  {
    return (m_pressed_end_time - m_pressed_start_time);
  }

  inline uint32_t GetReleasedTime() const
  {
    return (m_released_end_time - m_released_start_time);
  }

  inline bool GetPressedEvent()
  {
    if (m_event == event_e::pressed)
    {
      m_event = event_e::none;
      return true;
    }
    return false;
  }

  inline bool GetReleasedEvent()
  {
    if (m_event == event_e::released)
    {
      m_event = event_e::none;
      return true;
    }
    return false;
  }

  inline bool GetRepeatEvent()
  {
    if (m_event == event_e::repeat)
    {
      m_event = event_e::none;
      return true;
    }
    return false;
  }

  inline const char *GetName()
  {
    return m_cfg.name_str;
  }
};

#endif /* _USE_APP_ENBTN */

#endif /* AP__INC_ENBTN_HPP_ */
