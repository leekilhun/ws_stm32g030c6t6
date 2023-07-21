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
ap_io mcu_io;


/****************************************************
  1. ap instances
 ****************************************************/
enStepMotor motor[3]{};


void  apInit(void)
{

#ifdef _USE_HW_CLI

#endif


  enStepMotor::cfg_t cfg{};
  cfg.motor_id = 0;
  cfg.ptr_timer = timGetData(_DEF_TIM1);
  motor[0].Init(cfg);

  cfg.motor_id = 0;
  cfg.ptr_timer = timGetData(_DEF_TIM1);
  motor[1].Init(cfg);

  cfg.motor_id = 0;
  cfg.ptr_timer = timGetData(_DEF_TIM1);
  motor[2].Init(cfg);

  /*Assign Obj */
  mcu_io.Init();

#ifdef _USE_HW_CLI
cliAdd("app", cliApp);
#endif

}




void  apMain(void)
{
  uint32_t pre_time;
  uint32_t out_cnt = 0;
  pre_time = millis();

  while (1)
  {

    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
      //logPrintf(">>hdma_usart1_rx.Instance->CNDTR %d \n",hdma_usart1_rx.Instance->CNDTR);
      out_cnt =  out_cnt  % 16;
      mcu_io.OutputToggle(ap_io::out00 + out_cnt);
      out_cnt++;
    }



    /* 1. io register */
      mcu_io.Update_io();


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


