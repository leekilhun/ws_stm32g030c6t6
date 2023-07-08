/*
 * hw.c
 *
 *  Created on: Jun 9, 2023
 *      Author: gns2l
 */


#include "hw.h"


bool hwInit(void)
{
  bool ret = true;

#ifdef _USE_HW_CLI
  ret &= cliInit();
#endif

#ifdef _USE_HW_LED
  ret &= ledInit();
#endif

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif

#ifdef _USE_HW_LOG
  ret &= logInit();

  logOpen(HW_UART_CH_DEBUG, 115200);
  logPrintf("[ fw Begin... ]\r\n");
  logPrintf("Core Clock    \t\t: %d Mhz\r\n\n\n", SystemCoreClock/1000000);
#else // use small size log func
  uartOpen(HW_UART_CH_DEBUG, 115200);
  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Core Clock    \t\t: %d Mhz\r\n", SystemCoreClock/1000000);
#endif


#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif



  return ret;
}

