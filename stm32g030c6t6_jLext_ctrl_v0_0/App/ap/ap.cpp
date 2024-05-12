/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"

/**
 * @brief
 * It must be written in non-block code as a function called in 1ms time.
 * @param arg
 */
static void apISR_1ms(void *arg);
static void apISR_10ms(void *arg);

static void updateApReg();
static void updateLamp();

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif

/****************************************************
  0. mcu data and register
 ****************************************************/

/****************************************************
  1. ap instances
 ****************************************************/

void apInit(void)
{

#ifdef _USE_HW_CLI
  cliAdd("app", cliApp);
#endif
}

void apMain(void)
{
  /**
   * @brief
   * register the function to the 1ms interrupt sw-timer.
   */
  {
    swtimer_handle_t timer_ch;
    timer_ch = swtimerGetHandle();
    if (timer_ch >= 0)
    {
      swtimerSet(timer_ch, 1, LOOP_TIME, apISR_1ms, NULL);
      swtimerStart(timer_ch);
    }
    else
    {
      LOG_PRINT("[NG] 1ms swtimerGetHandle()");
    }

    timer_ch = swtimerGetHandle();
    if (timer_ch >= 0)
    {
      swtimerSet(timer_ch, 10, LOOP_TIME, apISR_10ms, NULL);
      swtimerStart(timer_ch);
    }
    else
    {
      LOG_PRINT("[NG] 10ms swtimerGetHandle()");
    }
  }
  // end of swtimer

  uint32_t pre_time;
  // uint32_t out_cnt = 0;
  pre_time = millis();



  //HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,GPIO_PIN_RESET);
  LOG_PRINT("Start App Loop");
  /* Infinite loop */
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();

      //HAL_GPIO_TogglePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin);

      //LOG_PRINT("ADC Value : %d", adcReadVoltage(_DEF_ADC1));
    }

    updateApReg();


  }
}

/*
 * operating lamp and buzzer
 */
void updateLamp()
{
  /**
   * @brief
   * status led
   */
  {
    static uint32_t pre_time_update_led;

    if (millis() - pre_time_update_led >= 1'000)
    {
      pre_time_update_led = millis();
      HAL_GPIO_TogglePin(STATUS_GPIO_Port, STATUS_Pin);
    }
  }
  // end of status led
}

void updateErr()
{
}


void updateApReg()
{


}


void apISR_1ms(void *arg)
{
  updateLamp();

}

void apISR_10ms(void *arg)
{

}

#ifdef _USE_HW_CLI
void cliApp(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "info") == true)
    {

      ret = true;
    }
  }
  else if (args->argc == 2)
  {

    // int result = 0;
  }

  if (ret == false)
  {
    cliPrintf("app info \n");
  }
}
#endif
