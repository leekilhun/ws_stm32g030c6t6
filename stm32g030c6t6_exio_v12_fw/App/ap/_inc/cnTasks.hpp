/*
 * cnTasks.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */
#pragma once
#ifndef AP__INC_CNTASKS_HPP_
#define AP__INC_CNTASKS_HPP_

#include "ap_def.hpp"

#ifdef _USE_APP_CNTASKS

struct cnTasks
{

  using pos_data_t = pos_data_st;
  using pos_idx_t = pos_data_st::idx_e;

  struct cfg_t
  {

    cfg_t() = default;
    ~cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t &rhs) = default;
    // copy assignment operator
    cfg_t &operator=(const cfg_t &rhs) = default;
    // move constructor
    cfg_t(cfg_t &&rhs) = default;
    // move assignment operator
    cfg_t &operator=(cfg_t &&rhs) = default;
  };

  cfg_t m_cfg;
  prc_step_t m_step;
  uint8_t m_idx;

  cnTasks() = default;
  ~cnTasks() = default;

  // copy constructor
  cnTasks(const cnTasks &rhs) = default;
  // copy assignment operator
  cnTasks &operator=(const cnTasks &rhs) = default;
  // move constructor
  cnTasks(cnTasks &&rhs) = default;
  // move assignment operator
  cnTasks &operator=(cnTasks &&rhs) = default;

  inline int Init(cnTasks::cfg_t &cfg)
  {
    LOG_PRINT("[OK] Init Success!");
    m_cfg = cfg;
    return ERROR_SUCCESS;
  }

  inline void ThreadJob()
  {
    doRunStep(m_idx);
  }

  inline void doRunStep(uint8_t idx = 0)
  {

    constexpr uint32_t step_wait_delay = 50;
    //constexpr uint8_t retry_max = 3;
    //constexpr uint32_t move_timeout = 1'000 * 5;

    enum : uint8_t
    {
      STEP_INIT,
      STEP_TODO,
      STEP_TIMEOUT,
      STEP_WAIT_DONE,
      STEP_WAIT_RETURN,
      STEP_WAIT_PREV_STEP_RETURN,
      STEP_DO_INITAIL,
      STEP_DO_INITAIL_START,
      STEP_DO_INITAIL_WAIT,
      STEP_DO_INITAIL_END,
      STEP_DO_STANDBY,
      STEP_DO_STANDBY_START,
      STEP_DO_STANDBY_WAIT,
      STEP_DO_STANDBY_END,
    };

    switch (m_step.GetStep())
    {
    case STEP_INIT:
    {
      LOG_PRINT("STEP_INIT");
      m_idx = 0;
      m_step.SetStep(STEP_TODO);
    }
    break;
    /*######################################################
       to do
      ######################################################*/
    case STEP_TODO:
    {
    }
    break;

    case STEP_TIMEOUT:
    {
      m_step.retry_cnt = 0;
      m_step.wait_resp = false;
      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_WAIT_DONE:
    {
    }
    break;
    case STEP_WAIT_PREV_STEP_RETURN:
    {
      if (m_step.LessThan(step_wait_delay * 10))
        break;
      LOG_PRINT("STEP_WAIT_PREV_STEP_RETURN prev_step[%d], retry[%d]", m_step.pre_step, m_step.retry_cnt);
      /*  wait 0.5 sec  and return */

      m_step.SetStep((m_step.pre_step - 1));
    }
    break;

    case STEP_WAIT_RETURN:
    {
    }
    break;
    ////////////////////////////////////////////////////
    /*######################################################
       do app system initialize
      ######################################################*/
    case STEP_DO_INITAIL:
    {
      m_step.retry_cnt = 0;
      m_step.wait_step = 0;
      m_step.wait_resp = false;
    }
    break;
    case STEP_DO_INITAIL_START:
    {
    }
    break;
    case STEP_DO_INITAIL_WAIT:
    {
      if (m_step.LessThan(step_wait_delay))
        break;
    }
    break;
    case STEP_DO_INITAIL_END:
    {
    }
    break;
    /*######################################################
       do system standby state and wait start
      ######################################################*/
    case STEP_DO_STANDBY:
    {
    }
    break;
    case STEP_DO_STANDBY_START:
    {
    }
    break;
    case STEP_DO_STANDBY_WAIT:
    {
    }
    break;
    case STEP_DO_STANDBY_END:
    {
    }
    break;
    ////////////////////////////////////////////////////
    default:
      break;
    }
  }
};
// end of cnTasks

#endif /* _USE_APP_CNTASKS */

#endif /* AP__INC_CNTASKS_HPP_ */
