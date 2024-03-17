/**
 * @file enLoadCell.hpp
 * @author lee kil hun (pba7189@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once
#ifndef __EN_LOAD_CELL_HPP__
#define __EN_LOAD_CELL_HPP__

#include "ap_def.hpp"

#ifdef _USE_APP_ENLOADCELL

struct enLoadCell
{
  /*
  
  
  */
  struct mv_avg_filter_t
  {    
    static constexpr int32_t MAX_DATA = 5;
    std::array<int32_t, MAX_DATA> data{};
    uint8_t index{}; // 다음 데이터를 삽입할 인덱스
    uint8_t count{}; // 입력된 데이터의 개수
    mv_avg_filter_t() = default;
  	// copy constructor
  	mv_avg_filter_t(const mv_avg_filter_t& other) = default;
  	// copy assignment
  	mv_avg_filter_t& operator=(const mv_avg_filter_t& other) = default;
  	// move constructor
  	mv_avg_filter_t(mv_avg_filter_t&& other) = default;
  	// move assignment
  	mv_avg_filter_t& operator=(mv_avg_filter_t&& other) = default;
    ~mv_avg_filter_t() = default;
    mv_avg_filter_t& operator<<(int32_t new_data)
    {
        this->data[index] = new_data;
        index = (index + 1) % MAX_DATA;
        if (count < MAX_DATA) {
            ++count; // 데이터 개수 증가, MAX_DATA를 초과하지 않도록 함
        }
        return *this;
    }
  }m_mv_avg_filter{};

  struct cfg_t
  {
    uint8_t loadcell_ch{};
    bool (*hx711_IsInit)(uint8_t ch){};
    bool (*hx711_IsAvailable)(uint8_t ch){};
    bool (*hx711_SetGain)(uint8_t ch, hx_711_gain_t gain){};
    void (*hx711_SetCoefficient)(uint8_t ch, float coefficient){};
    int32_t (*hx711_GetData)(uint8_t ch){};
    float (*hx11_ReadWeight)(uint8_t ch, uint8_t sample_feq){};
    void (*hx711_PowerOn)(uint8_t ch){};
    void (*hx711_PowerDown)(uint8_t ch){};
    char name_str[32]{};

    cfg_t() = default;
  	// copy constructor
  	cfg_t(const cfg_t& other) = default;
  	// copy assignment
  	cfg_t& operator=(const cfg_t& other) = default;
  	// move constructor
  	cfg_t(cfg_t&& other) = default;
  	// move assignment
  	cfg_t& operator=(cfg_t&& other) = default;
  } m_cfg{};

    prc_step_t m_step{};
    int32_t m_data{};
    UTL::_que<int32_t> m_que{};

  public:
  enLoadCell() = default;
  enLoadCell(const cfg_t& cfg) : m_cfg(cfg) {}
  // copy constructor
  enLoadCell(const enLoadCell& other) = default;
  // copy assignment
  enLoadCell& operator=(const enLoadCell& other) = default;
  // move constructor
  enLoadCell(enLoadCell&& other) = default;
  // move assignment
  enLoadCell& operator=(enLoadCell&& other) = default;
  ~enLoadCell()                             = default;

  inline error_t Init(cfg_t& cfg)
  {
    m_cfg = cfg;
    return ERROR_SUCCESS;
  }

  // 이동 평균 필터 함수
  inline int32_t moving_average_filter(int32_t new_data)
  {
    int32_t sum = 0;
    m_mv_avg_filter << new_data; // 데이터 삽입

    // 입력된 데이터의 개수에 따라 평균을 계산
    for (uint8_t i = 0; i < m_mv_avg_filter.count; ++i)
    {
        sum += m_mv_avg_filter.data[i];
    }

    // 입력된 데이터의 개수로 나누어 평균 계산
    return sum / (m_mv_avg_filter.count ? m_mv_avg_filter.count : 1);  
  }


  inline void GetDataStep()
  {
    enum : uint8_t
    {
      STEP_INIT,
      STEP_TODO,
      STEP_GET_DATA,
      STEP_GET_DATA_START,
      STEP_GET_DATA_WAIT,
      STEP_GET_DATA_END,
      STEP_TIMEOUT,
    };

    switch (m_step.GetStep())
    {
    case STEP_INIT:
    {
      m_que.Flush();
      m_step.SetStep(STEP_TODO);
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case STEP_TODO:
    {      
      m_cfg.hx711_PowerOn(m_cfg.loadcell_ch);
      m_step.SetStep(STEP_GET_DATA);
    }
    break;

     case STEP_TIMEOUT:
    {
      m_que.Flush();
      m_cfg.hx711_PowerDown(m_cfg.loadcell_ch);
      m_step.SetStep(STEP_INIT);
    }

    /*######################################################
       get data
      ######################################################*/
    case STEP_GET_DATA:
    {
      m_step.SetStep(STEP_GET_DATA_START);
      m_step.retry_cnt = 0;
    }
    break;
    case STEP_GET_DATA_START:
    {
      m_step.SetStep(STEP_GET_DATA_WAIT);
      m_data = m_cfg.hx711_GetData(m_cfg.loadcell_ch);
      if (abs(m_data) > 0)
        m_que.Put(m_data);//int32_t(moving_average_filter(m_data))
    }
    break;
    case STEP_GET_DATA_WAIT:
    {
      if (m_step.LessThan(1))
        break;
      if (m_cfg.hx711_IsAvailable(m_cfg.loadcell_ch))
      {
        m_step.SetStep(STEP_GET_DATA_START);
      }
      else
      {
        if(m_step.MoreThan(150))
        {
          if (m_step.retry_cnt++ > 3)
          {
            m_step.SetStep(STEP_GET_DATA_END);
          }
          else
          {
            m_step.SetStep(STEP_GET_DATA_START);
          }    
        }
      }
    }
    break;
    case STEP_GET_DATA_END:
    {
      //m_que.Push(m_cfg.hx711_GetData(m_cfg.loadcell_ch));
      m_cfg.hx711_PowerDown(m_cfg.loadcell_ch);
      m_step.SetStep(STEP_INIT);
    }
    break;
   
    break;
    default:
      break;
    }
    // end of switch

  }

};
// end of enLoadCell

#endif // _USE_APP_ENLOADCELL

#endif // __EN_LOAD_CELL_HPP__