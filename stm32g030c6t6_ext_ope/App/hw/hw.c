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

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif

#ifdef _USE_HW_TIM
  ret &= timInit();

  timStart(_TIM_3);
  LOG_PRINT("start tim3");
#endif

#ifdef _USE_HW_ADC
  ret &= adcInit();
#endif

#ifdef _USE_HW_WDG
  ret &= wdgInit();
#endif

#ifdef _USE_HW_LOG
  ret &= logInit();

  logOpen(HW_LOG_CH, 115200);
  logPrintf("[ fw Begin... ]\r\n");
  logPrintf("Core Clock    \t\t: %d Mhz\r\n\n\n", SystemCoreClock/1000000);
#else // use small size log func
  uartOpen(HW_LOG_CH, 115200);
  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("[ fw Version] \t\t: %s\r\n", _DEF_FW_VERSION);
  logPrintf("[ fw Name]    \t\t: %s\r\n", _DEF_FW_NAME);
  logPrintf("Core Clock    \t\t: %d Mhz\r\n", SystemCoreClock/1000000);
#endif

#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif

#ifdef _USE_HW_IIC_S
  ret &= iicsInit();
#endif

#ifdef _USE_HW_SWTIMER
  ret &=swtimerInit();
#endif



  return ret;
}

