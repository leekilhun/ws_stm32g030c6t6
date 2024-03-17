/*
 * led.c
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */



#include "led.h"

#ifdef _USE_HW_LED

bool ledInit(void)
{
  return true;
}

void ledOn(uint8_t ch)
{
  switch(ch)
  {
    case _DEF_LED1:
      HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_SET);
      break;
  }
}

void ledOff(uint8_t ch)
{
  switch(ch)
  {
    case _DEF_LED1:
      HAL_GPIO_WritePin(STATUS_GPIO_Port, STATUS_Pin, GPIO_PIN_RESET);
      break;
  }
}

void ledToggle(uint8_t ch)
{
  switch(ch)
  {
    case _DEF_LED1:
      HAL_GPIO_TogglePin(STATUS_GPIO_Port, STATUS_Pin);
      break;
  }
}

#endif
