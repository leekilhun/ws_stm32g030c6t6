/**
 * @file tim.c
 * @author lee kil hun (gns2.lee@)
 * @brief
 * @version 0.1
 * @date 2024-05-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "tim.h"

#ifdef _USE_HW_TIM

#include "cli.h"

/****************************************************
 *  macro and define
 ****************************************************/

#define TIM_MAX_CH HW_TIM_MAX_CH
#define DEF_INSTANCE_TIM1 TIM6
#define DEF_INSTANCE_TIM2 TIM7
#define DEF_INSTANCE_TIM3 TIM14

/****************************************************
 *  typedef
 ****************************************************/

/****************************************************
 *  data
 ****************************************************/
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim14;

tim_tbl_t tim_tbl[TIM_MAX_CH];

#if CLI_USE(HW_TIM)
static void cliTimer(cli_args_t *args);
static void timerTestISR(void);
#endif

/****************************************************
 *  function
 ****************************************************/

bool timInit(void)
{
  /* _DEF_TIM1*/
  uint8_t idx                       = _DEF_TIM1;
  tim_tbl[idx].ptr_htim             = &htim6;
  tim_tbl[idx].func_callback        = NULL;
  tim_tbl[idx].func_cb              = NULL;
  tim_tbl[idx].obj                  = NULL;
  tim_tbl[idx].act_ch_bit           = 0x00;
  tim_tbl[idx].is_start             = false;
  tim_tbl[idx].sConfig.OCFastMode   = 0;
  tim_tbl[idx].sConfig.OCIdleState  = 0;
  tim_tbl[idx].sConfig.OCMode       = 0;
  tim_tbl[idx].sConfig.OCNIdleState = 0;
  tim_tbl[idx].sConfig.OCNPolarity  = 0;
  tim_tbl[idx].sConfig.Pulse        = 0;
  tim_tbl[idx].channel[0]           = TIM_CHANNEL_1;
  tim_tbl[idx].channel[1]           = TIM_CHANNEL_2;
  tim_tbl[idx].channel[2]           = TIM_CHANNEL_3;
  tim_tbl[idx].channel[3]           = TIM_CHANNEL_4;

  /* _DEF_TIM2*/
  idx                       = _DEF_TIM2;
  tim_tbl[idx].ptr_htim             = &htim7;
  tim_tbl[idx].func_callback        = NULL;
  tim_tbl[idx].func_cb              = NULL;
  tim_tbl[idx].obj                  = NULL;
  tim_tbl[idx].act_ch_bit           = 0x00;
  tim_tbl[idx].is_start             = false;
  tim_tbl[idx].sConfig.OCFastMode   = 0;
  tim_tbl[idx].sConfig.OCIdleState  = 0;
  tim_tbl[idx].sConfig.OCMode       = 0;
  tim_tbl[idx].sConfig.OCNIdleState = 0;
  tim_tbl[idx].sConfig.OCNPolarity  = 0;
  tim_tbl[idx].sConfig.Pulse        = 0;
  tim_tbl[idx].channel[0]           = TIM_CHANNEL_1;
  tim_tbl[idx].channel[1]           = TIM_CHANNEL_2;
  tim_tbl[idx].channel[2]           = TIM_CHANNEL_3;
  tim_tbl[idx].channel[3]           = TIM_CHANNEL_4;

  /* _DEF_TIM3*/
  idx                       = _DEF_TIM3;
  tim_tbl[idx].ptr_htim             = &htim14;
  tim_tbl[idx].func_callback        = NULL;
  tim_tbl[idx].func_cb              = NULL;
  tim_tbl[idx].obj                  = NULL;
  tim_tbl[idx].act_ch_bit           = 0x00;
  tim_tbl[idx].is_start             = false;
  tim_tbl[idx].sConfig.OCFastMode   = 0;
  tim_tbl[idx].sConfig.OCIdleState  = 0;
  tim_tbl[idx].sConfig.OCMode       = 0;
  tim_tbl[idx].sConfig.OCNIdleState = 0;
  tim_tbl[idx].sConfig.OCNPolarity  = 0;
  tim_tbl[idx].sConfig.Pulse        = 0;
  tim_tbl[idx].channel[0]           = TIM_CHANNEL_1;
  tim_tbl[idx].channel[1]           = TIM_CHANNEL_2;
  tim_tbl[idx].channel[2]           = TIM_CHANNEL_3;
  tim_tbl[idx].channel[3]           = TIM_CHANNEL_4;

#if CLI_USE(HW_TIM)
  cliAdd("timer", cliTimer);
#endif

  return true;
}

tim_tbl_t *timGetData(uint8_t ch)
{
  return &tim_tbl[ch];
}

bool timStart(uint8_t ch)
{
  if (ch >= TIM_MAX_CH)
    return false;
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_HandleTypeDef      *p_handle      = tim_tbl[ch].ptr_htim;

  /* timer clock interval 1us, initial event time 1ms  */
  p_handle->Init.Prescaler         = (uint32_t)(SystemCoreClock / 1000000) - 1;
  p_handle->Init.CounterMode       = TIM_COUNTERMODE_UP;
  p_handle->Init.Period            = (1000 - 1);
  p_handle->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  p_handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(p_handle) != HAL_OK)
    return false;

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(p_handle, &sMasterConfig) != HAL_OK)
    return false;

  tim_tbl[ch].is_start = true;

  return true;
}

void timAttachCallBackFunc(uint8_t ch, void (*func)())
{
  tim_tbl[ch].func_callback = func;
}

void timAttachCB(uint8_t ch, void *obj, void (*func_cb)(void *, void *, void *))
{
  tim_tbl[ch].obj     = obj;
  tim_tbl[ch].func_cb = func_cb;
}

void timEnableISR(uint8_t ch)
{

  if (tim_tbl[ch].is_start == false)
    return;

  // cliPrintf("timEnableISR ch%d\n", ch);

  if (HAL_TIM_Base_Start_IT(tim_tbl[ch].ptr_htim) != HAL_OK)
  {
    /* Error */
  }
}

void timDisableISR(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  if (HAL_TIM_Base_Stop_IT(tim_tbl[ch].ptr_htim) != HAL_OK)
  {
    /* Error */
  }
}

void timResetCnt(uint8_t ch)
{
  if (tim_tbl[ch].is_start == false)
    return;

  tim_tbl[ch].ptr_htim->Instance->CR1 |= 1;
  tim_tbl[ch].ptr_htim->Instance->CNT  = 0;
}

bool timChangePrescale(uint8_t ch, uint32_t psc)
{
  if (tim_tbl[ch].is_start == false)
    return false;

  if (ch < TIM_MAX_CH)
  {
    timDisableISR(ch);
    tim_tbl[ch].ptr_htim->Instance->PSC = psc;
    timEnableISR(ch);
    return true;
  }
  return false;
}

bool timChangeAutoReloadRegister(uint8_t ch, uint32_t arr)
{
  if (tim_tbl[ch].is_start == false)
    return false;

  if (ch < TIM_MAX_CH)
  {
    timDisableISR(ch);
    tim_tbl[ch].ptr_htim->Instance->ARR = arr;
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

  if (htim->Instance == DEF_INSTANCE_TIM1)
  {
    if (tim_tbl[_DEF_TIM1].func_callback != NULL)
    {
      (*tim_tbl[_DEF_TIM1].func_callback)();
    }
    if (tim_tbl[_DEF_TIM1].func_cb != NULL)
    {
      (*tim_tbl[_DEF_TIM1].func_cb)(tim_tbl[_DEF_TIM1].obj, NULL, NULL);
    }
  }
  else if (htim->Instance == DEF_INSTANCE_TIM2)
  {
    if (tim_tbl[_DEF_TIM2].func_callback != NULL)
    {
      (*tim_tbl[_DEF_TIM2].func_callback)();
    }
    if (tim_tbl[_DEF_TIM2].func_cb != NULL)
    {
      (*tim_tbl[_DEF_TIM2].func_cb)(tim_tbl[_DEF_TIM2].obj, NULL, NULL);
    }
  }
  else if (htim->Instance == DEF_INSTANCE_TIM3)
  {
    if (tim_tbl[_DEF_TIM3].func_callback != NULL)
    {
      (*tim_tbl[_DEF_TIM3].func_callback)();
    }
    if (tim_tbl[_DEF_TIM3].func_cb != NULL)
    {
      (*tim_tbl[_DEF_TIM3].func_cb)(tim_tbl[_DEF_TIM3].obj, NULL, NULL);
    }
  }
}

#if CLI_USE(HW_TIM)

void timerTestGpio(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitStruct.Pin   = GPIO_PIN_0;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
void timerTestISR(void)
{
  LL_GPIO_TogglePin(GPIOD, LL_GPIO_PIN_0);
}

void cliTimer(cli_args_t *args)
{
  bool     ret = true;
  uint8_t  ch;
  uint32_t regist_val;

  if (args->argc == 3)
  {
    ch         = (uint8_t)args->getData(1);
    regist_val = (uint16_t)args->getData(2);

    ch = constrain(ch, 0, TIM_MAX_CH);

    if (args->isStr(0, "set_PSC"))
    {
      timChangePrescale(ch, regist_val);
      cliPrintf("set PSC ch%d %d\n", ch, regist_val);
    }
    else if (args->isStr(0, "set_ARR"))
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

    if (args->isStr(0, "start"))
    {
      timStart(ch);
      timAttachCallBackFunc(ch, timerTestISR);
      cliPrintf("timer ch%d start\n", ch);
    }
    else if (args->isStr(0, "enableISR"))
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
    if (args->isStr(0, "disableISR"))
    {
      timDisableISR(_DEF_TIM1);
      timDisableISR(_DEF_TIM2);
      timDisableISR(_DEF_TIM3);
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf("timer start 0~%d \n", TIM_MAX_CH - 1);
    cliPrintf("timer enableISR 0~%d \n", TIM_MAX_CH - 1);
    cliPrintf("timer set_PSC 0~%d data[1 ~ 65534] \n", TIM_MAX_CH - 1);
    cliPrintf("timer set_ARR 0~%d data[1 ~ 65534] \n", TIM_MAX_CH - 1);
    cliPrintf("timer disableISR\n");
  }
}
#endif

#endif // _USE_HW_TIM