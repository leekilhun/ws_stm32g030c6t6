/*
 * tim.h
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_TIM_H_
#define COMMON_INC_HW_TIM_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_TIM

#define TIM_MAX_CH         HW_TIM_MAX_CH



  typedef struct
  {
    TIM_HandleTypeDef       *pHandle;
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
