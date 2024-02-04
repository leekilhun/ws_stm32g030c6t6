/*
 * tim.c
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 * 
 * edit : 2024.02.03 timer interrupt capture 추가
 */






#include "tim.h"
#include "cli.h"


#ifdef _USE_HW_TIM


extern TIM_HandleTypeDef htim3;

//extern TIM_HandleTypeDef htim14;
//extern TIM_HandleTypeDef htim16;


tim_tbl_t  tim_tbl[TIM_MAX_CH];


#ifdef _USE_HW_CLI
static void cliTimer(cli_args_t *args);
static void timerTestISR(void);
#endif

bool timInit(void)
{
  for (int i=0; i<TIM_MAX_CH; i++)
  {
    tim_tbl[i].pHandle = NULL;
    tim_tbl[i].func_callback = NULL;
    tim_tbl[i].func_cb = NULL;
    tim_tbl[i].func_capture_cb = NULL;
    tim_tbl[i].obj = NULL;
    tim_tbl[i].act_ch_bit = 0x00;
    tim_tbl[i].is_start = false;
    tim_tbl[i].sConfig.OCFastMode = 0;
    tim_tbl[i].sConfig.OCIdleState = 0;
    tim_tbl[i].sConfig.OCMode = 0;
    tim_tbl[i].sConfig.OCNIdleState = 0;
    tim_tbl[i].sConfig.OCNPolarity = 0;
    tim_tbl[i].sConfig.Pulse = 0;
    tim_tbl[i].channel[0] = TIM_CHANNEL_1;
    tim_tbl[i].channel[1] = TIM_CHANNEL_2;
    tim_tbl[i].channel[2] = TIM_CHANNEL_3;
    tim_tbl[i].channel[3] = TIM_CHANNEL_4;

  }

#ifdef _USE_HW_CLI
  cliAdd("timer", cliTimer);
#endif

  return true;
}


tim_tbl_t * timGetData(uint8_t ch)
{
  return &tim_tbl[ch];
}


bool timStart(uint8_t ch)
{
  //TIM_MasterConfigTypeDef sMasterConfig = {0};
  switch (ch)
  {
    case _DEF_TIM1:
      tim_tbl[ch].pHandle  = &htim3;

#if 0
      /* timer clock interval 1us, initial event time 1ms  */
      tim_tbl[ch].pHandle->Instance                = TIM3;
      tim_tbl[ch].pHandle->Init.Prescaler          = (uint32_t)(SystemCoreClock/1000000)-1;
      tim_tbl[ch].pHandle->Init.CounterMode        = TIM_COUNTERMODE_UP;
      tim_tbl[ch].pHandle->Init.Period             = (1000-1);
      tim_tbl[ch].pHandle->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
      tim_tbl[ch].pHandle->Init.AutoReloadPreload  = TIM_AUTORELOAD_PRELOAD_ENABLE;
      if (HAL_TIM_Base_Init(tim_tbl[ch].pHandle) != HAL_OK)
      {
        return false;
      }

      sMasterConfig.MasterOutputTrigger       = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode           = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(tim_tbl[ch].pHandle, &sMasterConfig) != HAL_OK)
      {
        return false;
      }
#endif
      tim_tbl[ch].is_start = true;
      break;

    // case _DEF_TIM2:
    //   tim_tbl[ch].pHandle  = &htim14;

    //   /* timer clock interval 1us, initial event time 1ms  */
    //   tim_tbl[ch].pHandle->Instance                = TIM14;
    //   tim_tbl[ch].pHandle->Init.Prescaler          = (uint32_t)(SystemCoreClock/1000000)-1;
    //   tim_tbl[ch].pHandle->Init.CounterMode        = TIM_COUNTERMODE_UP;
    //   tim_tbl[ch].pHandle->Init.Period             = (1000-1);
    //   tim_tbl[ch].pHandle->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
    //   tim_tbl[ch].pHandle->Init.AutoReloadPreload  = TIM_AUTORELOAD_PRELOAD_ENABLE;
    //   if (HAL_TIM_Base_Init(tim_tbl[ch].pHandle) != HAL_OK)
    //   {
    //     return false;
    //   }
    //   tim_tbl[ch].is_start = true;
    //   break;

    // case _DEF_TIM3:
    //   tim_tbl[ch].pHandle  = &htim16;

    //   /* timer clock interval 1us, initial event time 1ms  */
    //   tim_tbl[ch].pHandle->Instance                = TIM16;
    //   tim_tbl[ch].pHandle->Init.Prescaler          = (uint32_t)(SystemCoreClock/1000000)-1;
    //   tim_tbl[ch].pHandle->Init.CounterMode        = TIM_COUNTERMODE_UP;
    //   tim_tbl[ch].pHandle->Init.Period             = (1000-1);
    //   tim_tbl[ch].pHandle->Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
    //   tim_tbl[ch].pHandle->Init.AutoReloadPreload  = TIM_AUTORELOAD_PRELOAD_ENABLE;
    //   if (HAL_TIM_Base_Init(tim_tbl[ch].pHandle) != HAL_OK)
    //   {
    //     return false;
    //   }
    //   tim_tbl[ch].is_start = true;
    //   break;

    default:
      break;
  }
  // end of switch


  return true;
}


void timAttachCallBackFunc(uint8_t ch, void (*func)())
{
  tim_tbl[ch].func_callback = func;
}

void timAttachCB (uint8_t ch, void* obj, void (*func_cb)(void*, void*, void*))
{
  tim_tbl[ch].obj = obj;
  tim_tbl[ch].func_cb = func_cb;
}

void timAttachCaptureCB(uint8_t ch, void (*func_capture_cb)(void *, void *, void *))
{
  tim_tbl[ch].func_capture_cb = func_capture_cb;
}

void timEnableISR(uint8_t ch)
{

  if (tim_tbl[ch].is_start == false)
    return;

  if (HAL_TIM_Base_Start_IT(tim_tbl[ch].pHandle) != HAL_OK)
  {
    /* Error */
  }
}

void timDisableISR(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  //LOG_PRINT("ch[%d]",ch);
  if (HAL_TIM_Base_Stop_IT(tim_tbl[ch].pHandle) != HAL_OK)
  {
    /* Error */
    LOG_PRINT("[NG] ch[%d]  ",ch);
  }
  //LOG_PRINT("[OK] ch[%d]",ch);
}


void timResetCnt(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  tim_tbl[ch].pHandle->Instance->CR1 |= 1;
  tim_tbl[ch].pHandle->Instance->CNT = 0;
}


bool timChangePrescale(uint8_t ch,uint32_t psc)
{
  if (tim_tbl[ch].is_start == false)
    return false;

  if (ch < TIM_MAX_CH)
  {
    timDisableISR(ch);
    tim_tbl[ch].pHandle->Instance->PSC = psc;
    timEnableISR(ch);
    return true;
  }
  return false;
}


bool timChangeAutoReloadRegister(uint8_t ch,uint32_t arr)
{
  if (tim_tbl[ch].is_start == false)
    return false;

  if (ch < TIM_MAX_CH)
  {
    timDisableISR(ch);
    tim_tbl[ch].pHandle->Instance->ARR = arr;
    timEnableISR(ch);
    return true;
  }
  return false;

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

#ifdef _USE_HW_RTOS
  HAL_IncTick();
#endif

  if(htim->Instance==TIM3)
  {
    if (tim_tbl[_DEF_TIM1].func_callback != NULL )
    {
      (*tim_tbl[_DEF_TIM1].func_callback)();
    }
    if (tim_tbl[_DEF_TIM1].func_cb != NULL )
    {
      (*tim_tbl[_DEF_TIM1].func_cb)(tim_tbl[_DEF_TIM1].obj, NULL, NULL);
    }
  }
  // else if(htim->Instance==TIM14)
  // {
  //   if (tim_tbl[_DEF_TIM2].func_callback != NULL )
  //   {
  //     (*tim_tbl[_DEF_TIM2].func_callback)();
  //   }
  //   if (tim_tbl[_DEF_TIM2].func_cb != NULL )
  //   {
  //     (*tim_tbl[_DEF_TIM2].func_cb)(tim_tbl[_DEF_TIM2].obj, NULL, NULL);
  //   }
  // }
  // else if(htim->Instance==TIM16)
  // {
  //   if (tim_tbl[_DEF_TIM3].func_callback != NULL )
  //   {
  //     (*tim_tbl[_DEF_TIM3].func_callback)();
  //   }
  //   if (tim_tbl[_DEF_TIM3].func_cb != NULL )
  //   {
  //     (*tim_tbl[_DEF_TIM3].func_cb)(tim_tbl[_DEF_TIM3].obj, NULL, NULL);
  //   }
  // }

}

/* 입력 캡처 인터럽트 핸들러 함수 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance==TIM3)
  {
    if (tim_tbl[_DEF_TIM1].func_capture_cb != NULL )
    {
      (*tim_tbl[_DEF_TIM1].func_capture_cb)(tim_tbl[_DEF_TIM1].obj, NULL, NULL);
    }
  }
}




#ifdef _USE_HW_CLI


void timerTestGpio(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}
void timerTestISR(void)
{
  LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_0);
}



void cliTimer(cli_args_t *args)
{
  bool ret = true;
  uint8_t  ch;
  uint32_t regist_val;

  if (args->argc == 3)
  {
    ch  = (uint8_t)args->getData(1);
    regist_val = (uint16_t)args->getData(2);

    ch = constrain(ch, 0, TIM_MAX_CH);

    if(args->isStr(0, "set_PSC"))
    {
      timChangePrescale(ch, regist_val);
      cliPrintf("set PSC ch%d %d\n", ch, regist_val);
    }
    else if(args->isStr(0, "set_ARR"))
    {
      timChangeAutoReloadRegister(ch, regist_val);
      cliPrintf("set ARR ch%d %d\n", ch, regist_val);
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 2)
  {
    ch = (uint8_t)args->getData(1);

    if(args->isStr(0, "start"))
    {
      timStart(ch);
      timAttachCallBackFunc(ch,timerTestISR);
      cliPrintf("timer ch%d start\n", ch);
    }
    else if(args->isStr(0, "enableISR"))
    {
      timerTestGpio();
      timEnableISR(ch);
      cliPrintf("timer ch%d ISR Enable\n", ch);
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 1)
  {
    if(args->isStr(0, "disableISR"))
    {
      timDisableISR(_DEF_TIM1);
      timDisableISR(_DEF_TIM2);
      timDisableISR(_DEF_TIM3);
      timDisableISR(_DEF_TIM4);
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "timer start 0~%d \n", TIM_MAX_CH-1);
    cliPrintf( "timer enableISR 0~%d \n", TIM_MAX_CH-1);
    cliPrintf( "timer set_PSC 0~%d data[1 ~ 65534] \n", TIM_MAX_CH-1);
    cliPrintf( "timer set_ARR 0~%d data[1 ~ 65534] \n", TIM_MAX_CH-1);
    cliPrintf( "timer disableISR\n");
  }

}
#endif

#endif /*_USE_HW_TIM*/
