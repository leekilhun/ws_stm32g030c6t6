/**
 * @file swTimer.c
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-05-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "swtimer.h"


#ifdef _USE_HW_SWTIMER 


  /****************************************************
   *  macro and define
   ****************************************************/
#define THIS_TIMER  htim17
#define THIS_TIMER_IRQ  TIM17_IRQn


 /****************************************************
   *  typedef
   ****************************************************/
typedef struct
{
  bool          timer_en;   // Enable timer signal
  SwtimerMode_t timer_mode; // Timer mode
  uint32_t      timer_cnt;  // Current timer value
  uint32_t      timer_init; // Timer initial count value
  void (*tmr_func)(void *); // Function to be executed when timer expires
  void *tmr_func_arg;       // Arguments to be passed to the function
} swtimer_t;


  /****************************************************
   *  data
   ****************************************************/  
extern TIM_HandleTypeDef htim17;

static bool is_init = false;
static volatile uint32_t sw_timer_counter      = 0;
static volatile uint16_t sw_timer_handle_index = 0;
static swtimer_t  swtimer_tbl[_HW_DEF_SW_TIMER_MAX];           // timer table

static bool swtimerInitTimer(void);
static void swtimerTimerCallback(TIM_HandleTypeDef *htim);


  /****************************************************
   *  function
   ****************************************************/

bool swtimerInit(void)
{
  if (is_init)
    return true;


  // timer table init
  for(uint8_t i=0; i<_HW_DEF_SW_TIMER_MAX; i++)
  {
    swtimer_tbl[i].timer_en   = false;
    swtimer_tbl[i].timer_cnt  = 0;
    swtimer_tbl[i].timer_init = 0;
    swtimer_tbl[i].tmr_func   = NULL;
  }

  is_init = true;

  swtimerInitTimer();

  return true;
}

bool swtimerInitTimer(void)
{
  THIS_TIMER.Init.Prescaler         = (uint32_t)(SystemCoreClock / 1000000) - 1;
  THIS_TIMER.Init.CounterMode       = TIM_COUNTERMODE_UP;
  THIS_TIMER.Init.Period            = (1000 - 1);
  THIS_TIMER.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  THIS_TIMER.Init.RepetitionCounter = 0;
  THIS_TIMER.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&THIS_TIMER) != HAL_OK)
  {
    return false;
  }

  HAL_TIM_RegisterCallback(&THIS_TIMER, HAL_TIM_PERIOD_ELAPSED_CB_ID, swtimerTimerCallback);

  HAL_NVIC_SetPriority(THIS_TIMER_IRQ, 3, 0);
  HAL_NVIC_EnableIRQ(THIS_TIMER_IRQ);  

  HAL_TIM_Base_Start_IT(&THIS_TIMER);

  return true;
}

void swtimerISR(void)
{
  uint8_t i;

  sw_timer_counter++;


  for (i=0; i<_HW_DEF_SW_TIMER_MAX && i<sw_timer_handle_index; i++)   
  {
    if ( swtimer_tbl[i].timer_en == true)                       
    {
      swtimer_tbl[i].timer_cnt--;                               

      if (swtimer_tbl[i].timer_cnt == 0)                        
      {
        if(swtimer_tbl[i].timer_mode == ONE_TIME)               
        {
          swtimer_tbl[i].timer_en = false;                       
        }

        swtimer_tbl[i].timer_cnt = swtimer_tbl[i].timer_init;   

        (*swtimer_tbl[i].tmr_func)(swtimer_tbl[i].tmr_func_arg);  
      }
    }
  }
}

void swtimerSet(swtimer_handle_t handle, uint32_t period_ms, SwtimerMode_t mode, void (*Fnct)(void *), void *arg)
{
  if(handle < 0) return;

  swtimer_tbl[handle].timer_mode = mode;    
  swtimer_tbl[handle].tmr_func   = Fnct;       
  swtimer_tbl[handle].tmr_func_arg = arg;        
  swtimer_tbl[handle].timer_cnt  = period_ms;
  swtimer_tbl[handle].timer_init = period_ms;
}

void swtimerStart(swtimer_handle_t handle)
{
  if(handle < 0) return;

  swtimer_tbl[handle].timer_cnt = swtimer_tbl[handle].timer_init;
  swtimer_tbl[handle].timer_en  = true;
}

void swtimerStop (swtimer_handle_t handle)
{
  if(handle < 0) return;

  swtimer_tbl[handle].timer_en = false;
}

void swtimerReset(swtimer_handle_t handle)
{
  if(handle < 0) return;

  swtimer_tbl[handle].timer_en   = false;
  swtimer_tbl[handle].timer_cnt  = swtimer_tbl[handle].timer_init;
}

swtimer_handle_t swtimerGetHandle(void)
{
  swtimer_handle_t tmr_index = sw_timer_handle_index;

  if (tmr_index < _HW_DEF_SW_TIMER_MAX)
    sw_timer_handle_index++;
  else
    tmr_index = -1;

  return tmr_index;
}

uint32_t swtimerGetCounter(void)
{
  return sw_timer_counter;
}


void swtimerTimerCallback(TIM_HandleTypeDef *htim)
{
  swtimerISR();
}

#endif
