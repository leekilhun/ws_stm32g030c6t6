/*****************************************************************//**
 * \file   utils.hpp
 * \brief  전체적으로 필요한 유틸리티 함수들을 모아놓은 헤더파일
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef _CORE_COMMON_UTIL_UTILS_HPP_
#define _CORE_COMMON_UTIL_UTILS_HPP_

#include "../def.hpp"

namespace ap_core
{
  struct machine_step_t
  {
    volatile uint8_t         curr_step{};
    volatile uint8_t         pre_step{};
    volatile uint8_t         wait_step{};
    volatile uint32_t        prev_ms{};
    volatile uint32_t        elap_ms{};
    volatile uint8_t         retry_cnt{};
    TinyC::tiny_fifo<uint8_t, 32> steps{};

    inline void SetStep(uint8_t step)
    {
      elap_ms = tim::millis() - prev_ms;
      prev_ms = tim::millis();
      pre_step = curr_step;
      curr_step = step;
      steps.in(step);
    }

    inline uint8_t GetStep() const
    {
      return curr_step;
    }

    inline uint32_t ElapTime() const
    {
      return elap_ms;
    }

    inline bool LessThan(uint32_t msec)
    {
      elap_ms = tim::millis() - prev_ms;
      if (elap_ms < msec)
        return true;
      else
        return false;
    }

    inline bool MoreThan(uint32_t msec)
    {
      return !LessThan(msec);
    }

    inline uint8_t operator-(uint8_t dec_index)
    {
      if (dec_index > steps.available())
      {
        LOG_PRINT("dec_index is out of range");
        return 0;
      }
      else
      {
        return steps.pop(dec_index);
      }
    }
  };
  // end of struct machine_step_t



  namespace types
  {
    template<typename... Args>
    concept same_args_c = std::conjunction_v<std::is_same<Args, Args>...>;

  } // namespace types

}
// end of namespace ap_core

#endif // _CORE_COMMON_UTIL_UTILS_HPP_
