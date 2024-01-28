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


/****************************************************
  0. mcu data and register
 ****************************************************/


/****************************************************
  1. ap instances
 ****************************************************/


void apInit(void)
{


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
  pre_time = millis();

  LOG_PRINT("Start App Loop");
  /* Infinite loop */
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();
    }

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
/*
 * mcu register update
 */
void updateApReg()
{

}


void apISR_1ms(void *arg)
{

  updateApReg();

  updateErr();

#if 0
  if (uartAvailable(_DEF_UART1) > 0)
  {
    uint8_t rx_data;
    rx_data = uartRead(_DEF_UART1);
    uartPrintf(_DEF_UART1, "rx data : 0x%02X (%c)\n", rx_data, rx_data);
  }
#endif
}

void apISR_10ms(void *arg)
{
  updateLamp();
}

