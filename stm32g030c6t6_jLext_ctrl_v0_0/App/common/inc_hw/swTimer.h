/**
 * @file swTimer.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-05-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef __COMMOM_INC_HW_SW_TIMER_H__
#define __COMMOM_INC_HW_SW_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_SWTIMER

#define _HW_DEF_SW_TIMER_MAX        HW_SWTIMER_MAX_CH


typedef enum
{
  ONE_TIME,
  LOOP_TIME,
} SwtimerMode_t;



typedef int16_t  swtimer_handle_t;



bool swtimerInit(void);
void swtimerSet  (swtimer_handle_t handle, uint32_t period_ms, SwtimerMode_t mode, void (*Fnct)(void *), void *arg);
void swtimerStart(swtimer_handle_t handle);
void swtimerStop (swtimer_handle_t handle);
void swtimerReset(swtimer_handle_t handle);
void swtimerISR(void);


swtimer_handle_t swtimerGetHandle(void);
uint32_t swtimerGetCounter(void);

#endif

#ifdef __cplusplus
}
#endif



#endif // __COMMOM_INC_HW_SW_TIMER_H__