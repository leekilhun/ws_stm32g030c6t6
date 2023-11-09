/*
 * bsp.c
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */


#include "bsp.h"
#include "uart.h"
#include "utils.h"


bool bspInit(void)
{
  return true;
}



void bspDeInit(void)
{
  HAL_RCC_DeInit();

  // Disable Interrupts
  NVIC->ICER[0] = 0xFFFFFFFF;
  __DSB();
  __ISB();

  SysTick->CTRL = 0;
}


void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}

void delay_us(uint32_t us)
{
  volatile uint32_t i;

  for (i=0; i<us*1000; i++)
  {

  }
}



uint32_t millis(void)
{
  return HAL_GetTick();
}


#ifndef _USE_HW_LOG
void logPrintf(const char *fmt, ...)
{
#ifdef DEBUG
  va_list args;
  int len;
  char buf[256];

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  uartWrite(HW_LOG_CH, (uint8_t *)buf, len);

  va_end(args);

#endif

}
#endif



void logView(const char* file, const char* func, const int line, const char* fmt, ...)
{

#ifdef DEBUG
  const int ARG_TBL_CNT_MAX = 10;
  const uint8_t FILE_STR_MAX = 255;

  char tmp_str[FILE_STR_MAX];
  memset(tmp_str,0, FILE_STR_MAX);
  strcpy(tmp_str, file);
  char* arg_tbl[ARG_TBL_CNT_MAX];
  memset(arg_tbl,0, ARG_TBL_CNT_MAX);
  uint8_t arg_cnt = utilParseArgs(tmp_str, arg_tbl, "/\\", ARG_TBL_CNT_MAX);

  va_list args;
  char buf[256];
  va_start(args, fmt);
  vsnprintf(buf, 256, fmt, args);
  uartPrintf(HW_LOG_CH, "[%s][%s(%d)][%s] \n", arg_tbl[arg_cnt - 1], func, line, (const char*)buf);
  va_end(args);

#endif


}



