/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"


#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif


/****************************************************
  0. mcu data and register
 ****************************************************/



/****************************************************
  1. ap instances
 ****************************************************/

void  apInit(void)
{

#ifdef _USE_HW_CLI

#endif




#ifdef _USE_HW_CLI
cliAdd("app", cliApp);
#endif

}




void  apMain(void)
{
  uint32_t pre_time;

  pre_time = millis();

  while (1)
  {

    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
      //logPrintf(">>hdma_usart1_rx.Instance->CNDTR %d \n",hdma_usart1_rx.Instance->CNDTR);
    }



#ifdef _USE_HW_CLI
    cliMain();
#endif

#if 0

    if (uartAvailable(_DEF_UART1) > 0)
    {
      uint8_t rx_data;

      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "rx data : 0x%02X (%c)\n", rx_data, rx_data);
    }
#endif

  }

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
    
    //int result = 0;

  }



  if (ret == false)
  {
    cliPrintf( "app info \n");
  }


}
#endif


