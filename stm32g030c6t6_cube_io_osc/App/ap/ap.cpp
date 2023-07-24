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
CMD::uart_cmd cmd_comm;

/****************************************************
  1. ap instances
 ****************************************************/
enStepMotor motor[3]{};

CMD::api_cmd main_cmd;


void  apInit(void)
{

#ifdef _USE_HW_CLI

#endif

  timStart(_DEF_TIM1);

  enStepMotor::cfg_t cfg{};
  cfg.motor_id = 0;
  cfg.ptr_timer = timGetData(_DEF_TIM1);
  cfg.out_addr_step = ap_io::out00;
  cfg.out_addr_dir = ap_io::out01;
  cfg.out_addr_enable = ap_io::out02;
  cfg.in_addr_org = ap_io::in00;
  cfg.in_addr_ccw_limit = ap_io::in01;
  cfg.in_addr_cw_limit = ap_io::in02;
  cfg.ptr_mcu_io = &mcu_io;
  motor[0].Init(cfg);

  cfg.motor_id = 1;
  cfg.out_addr_step = ap_io::out03;
  cfg.out_addr_dir = ap_io::out04;
  cfg.out_addr_enable = ap_io::out05;
  cfg.in_addr_org = ap_io::in03;
  cfg.in_addr_ccw_limit = ap_io::in04;
  cfg.in_addr_cw_limit = ap_io::in05;
  cfg.ptr_mcu_io = &mcu_io;
  cfg.ptr_timer = timGetData(_DEF_TIM1);
  motor[1].Init(cfg);

  cfg.motor_id = 2;
  cfg.out_addr_step = ap_io::out06;
  cfg.out_addr_dir = ap_io::out07;
  cfg.out_addr_enable = ap_io::out10;
  cfg.in_addr_org = ap_io::in06;
  cfg.in_addr_ccw_limit = ap_io::in07;
  cfg.in_addr_cw_limit = ap_io::in10;
  cfg.ptr_mcu_io = &mcu_io;
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

  timEnableISR(_DEF_TIM1);

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


