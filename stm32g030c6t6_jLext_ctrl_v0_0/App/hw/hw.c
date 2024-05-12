/**
 * @file hw.cpp
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "hw.h"


bool hwInit(void)
{
  bool ret = true;

  
HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);


#ifdef _USE_HW_UART
  ret &= uartInit();
#endif

#ifdef _USE_HW_TIM
  ret &= timInit();
#endif

#ifdef _USE_HW_LOG
  ret &= logInit();
  logOpen(HW_LOG_CH, 115200);
#else
  uartOpen(HW_LOG_CH, 115200);
#endif

  logPrintf("[ Firmware Begin... ]\r\n");
  logPrintf("Firmware..Name \t\t: %s\r\n", _DEF_FW_NAME);
  logPrintf("Firmware..Ver  \t\t: %s\r\n", _DEF_FW_VERSION);
  logPrintf("Firmware..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq() / 1000000);
  logPrintf("\n");


#ifdef _USE_HW_SWTIMER
  ret &=swtimerInit();
#endif

#ifdef _USE_HW_I2C
  ret &= i2cInit();
#endif

  return ret;
}
