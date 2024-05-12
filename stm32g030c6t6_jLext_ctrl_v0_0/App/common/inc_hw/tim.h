/**
 * @file tim.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-05-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifndef COMMON_INC_HW_TIM_H_
#define COMMON_INC_HW_TIM_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_TIM



  typedef struct
  {
    TIM_HandleTypeDef       *ptr_htim;
    TIM_OC_InitTypeDef      sConfig;
    uint8_t                 act_ch_bit;
    uint32_t                channel[4];
    void (*func_callback)(void);
    void (*func_cb)(void*, void*, void*);
    void* obj;
    bool                    is_start;
  } tim_tbl_t;


  bool timInit(void);

  bool timStart(uint8_t ch);

  tim_tbl_t * timGetData(uint8_t ch);

  void timAttachCallBackFunc(uint8_t ch, void (*func)());
  void timAttachCB (uint8_t ch, void* obj , void (*func_cb)(void*, void*, void*));
  void timEnableISR(uint8_t ch);
  void timDisableISR(uint8_t ch);
  void timResetCnt(uint8_t ch);

  bool timChangePrescale(uint8_t ch,uint32_t psc);
  bool timChangeAutoReloadRegister(uint8_t ch,uint32_t arr);


#endif /*_USE_HW_TIM*/

#ifdef __cplusplus
}
#endif

#endif /* COMMON_INC_HW_TIM_H_ */
