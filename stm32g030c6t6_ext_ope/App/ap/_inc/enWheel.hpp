/*
 * enWheel.hpp
 *
 *  Created on: Feb 4, 2024
 *      Author: gns2l
 * 
 *  test 24.02.03 
 *  edit : test 1 -  https://www.youtube.com/watch?v=yeVCuAdRnZU&t=252s&ab_channel=SteppeSchool 
 *  edit : test 2 -  https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=eziya76&logNo=221466864342
 */

#pragma once
#ifndef AP__INC_ENWHEEL_HPP_
#define AP__INC_ENWHEEL_HPP_

#include "ap_def.hpp"

#ifdef _USE_APP_ENWHEEL

struct enWheel
{
  struct cfg_t
  {
    uint8_t    timer_id{};
    tim_tbl_t *ptr_timer{};
    uint16_t   pulse_per_revolution{};
    uint32_t   clock_frequence{};
    uint16_t   speed_cals_interval{};
    uint16_t   speed_unit_conversion{};
    char       name_str[32]{};

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

    char *set_name(const char *name)
    {
      memset(&name_str[0], 0, sizeof(name_str));
      strlcpy(&name_str[0], name, sizeof(name_str));
      return &name_str[0];
    }
  } m_cfg{};

  struct timer_value_t
  {
    volatile uint32_t capture_count{};      //  입력 캡처 카운트
    volatile uint32_t capture_value{};      //  입력 캡처 값
    volatile uint32_t prev_capture_value{}; //  이전 입력 캡처 값
    volatile uint32_t pulse_count{};        //  펄스 발생 수
    volatile uint32_t direction{};          //  방향
    volatile uint32_t speed{};              //  속도
    volatile uint32_t wheel_cnt{};
  } m_value{};

  struct encoder_instance
  {
    int16_t  velocity{};
    int64_t  position{};
    uint32_t last_counter_value{};
  } m_encoder{};

int16_t m_encoder_velocity{};
int32_t m_encoder_position{};
uint16_t m_timer_counter{};
  public:

  enWheel()  = default;
  ~enWheel() = default;

  inline error_t Init(const cfg_t &cfg)
  {
    // timAttachCB(cfg.timer_id, this, ISR_axis);
    // timAttachCaptureCB(cfg.timer_id, timercapture_func);
    m_cfg = cfg;

    //HAL_TIM_Encoder_Start(m_cfg.ptr_timer->pHandle, TIM_CHANNEL_1 | TIM_CHANNEL_2);
    //timEnableISR(m_cfg.timer_id);

    reset_encoder();

    HAL_TIM_Encoder_Start(m_cfg.ptr_timer->pHandle, TIM_CHANNEL_ALL);
    LOG_PRINT("Init Success! %s", m_cfg.name_str);

    return ERROR_SUCCESS;
  }

  inline void reset_encoder()
  {
    m_encoder.last_counter_value = 0;
    m_encoder.position           = 0;
    m_encoder.velocity           = 0;
  }

  inline void update_encoder(encoder_instance& value)
  {
    uint32_t       temp_counter = __HAL_TIM_GET_COUNTER(m_cfg.ptr_timer->pHandle);
    static uint8_t first_time = 0;
    if (!first_time)
    {
      value.velocity = 0;
      first_time     = 1;
    }
    else
    {
      if (temp_counter == value.last_counter_value)
      {
        value.velocity = 0;
      }
      else if (temp_counter > value.last_counter_value)
      {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(m_cfg.ptr_timer->pHandle))
          value.velocity = -value.last_counter_value - (__HAL_TIM_GET_AUTORELOAD(m_cfg.ptr_timer->pHandle) - temp_counter);
        else
          value.velocity = temp_counter - value.last_counter_value;
      }
      else
      {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(m_cfg.ptr_timer->pHandle))
          value.velocity = temp_counter - value.last_counter_value;
        else
          value.velocity = temp_counter + (__HAL_TIM_GET_AUTORELOAD(m_cfg.ptr_timer->pHandle) - value.last_counter_value);
      }
    }
    value.position           += value.velocity;
    value.last_counter_value  = temp_counter;
    // int32_t counter_value = m_cfg.ptr_timer->pHandle->Instance->CNT;
    // int32_t delta = counter_value - m_encoder.last_counter_value;
    // m_encoder.position += delta;
    // m_encoder.velocity = delta;
    // m_encoder.last_counter_value = counter_value;
  }

  inline void wheelCntView()
  {
    LOG_PRINT("Wheel CH1,2 - timer_cnt %d,pos %d, vel %d ", m_timer_counter, m_encoder_position, m_encoder_velocity);
  }

  inline void periodElapsedCallback()
  {
    m_timer_counter = __HAL_TIM_GET_COUNTER(m_cfg.ptr_timer->pHandle);
    update_encoder(m_encoder);
    m_encoder_position = m_encoder.position;
    m_encoder_velocity = m_encoder.velocity;
  }

  #if 0

  inline static void ISR_axis(void *obj, void *w_param, void *l_param)
  {
    enWheel           *p   = static_cast<enWheel *>(obj);
    TIM_HandleTypeDef *tim = p->m_cfg.ptr_timer->pHandle;
    p->m_timer_counter = __HAL_TIM_GET_COUNTER(tim);
    p->wheelCntView();

  }

  inline static void timercapture_func(void *obj, void *w_param, void *l_param)
  {
    enWheel *p               = static_cast<enWheel *>(obj);
    // p->m_value.capture_value = p->m_cfg.ptr_timer->pHandle->Instance->CCR1;
    // p->m_value.capture_count++;
    // p->update_encoder();
    TIM_HandleTypeDef *tim = p->m_cfg.ptr_timer->pHandle;

    if (tim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)                               //  채널 1에서 인터럽트가 발생한 경우
    {
      p->m_value.capture_count = HAL_TIM_ReadCapturedValue(tim, TIM_CHANNEL_1); //  채널 1의 캡처 값 읽기
      if (p->m_value.capture_count > p->m_value.prev_capture_value)             //  방향이 정방향인 경우
      {
        p->m_value.direction = 1;
        p->m_value.wheel_cnt++;
      }
      else                                                                      // 방향이 역방향인 경우
      {
        p->m_value.direction = -1;
        if (p->m_value.wheel_cnt > 1)
          p->m_value.wheel_cnt--;
      }

      p->m_value.pulse_count++;                                 // 펄스 발생 수 증가
      p->m_value.prev_capture_value = p->m_value.capture_value; // 이전 입력 캡처 값 저장

      // print out
      LOG_PRINT("Channel 1 Captured Value: %d, Direction: %d\n",  p->m_value.capture_value, p->m_value.speed);
    }
    else if (tim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)                                                                                    // 채널 2에서 인터럽트가 발생한 경우
    {
      p->m_value.capture_value = HAL_TIM_ReadCapturedValue(tim, TIM_CHANNEL_2);                                                           // 채널 2의 캡처 값 읽기

      uint32_t time_diff = p->m_value.capture_value - p->m_value.prev_capture_value;                                                      // 펄스 간 시간 차이 계산
      p->m_value.speed   = (uint32_t)((float)p->m_cfg.pulse_per_revolution / (float)time_diff * (float)p->m_cfg.clock_frequence / 60.0f); // 속도 계산 (rpm)

      p->m_value.prev_capture_value = p->m_value.capture_value;                                                                           // 이전 입력 캡처 값 저장

      // print out
      LOG_PRINT("Channel 2 Captured Value: %d, Direction: %d\n",  p->m_value.capture_value, p->m_value.speed);
    }
  }

  #endif
};

#endif

#endif /* AP__INC_ENWHEEL_HPP_ */
