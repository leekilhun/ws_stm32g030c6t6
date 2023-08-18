/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"

static void updateApReg();
static void updateErr();
static void eventOpPanel();
static void updateLamp();

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif

/****************************************************
  0. mcu data and register
 ****************************************************/
ap_reg mcu_reg;
ap_io mcu_io;
CMD::uart_cmd cmd_comm;
mcu_data_st mcu_data;

/****************************************************
  1. ap instances
 ****************************************************/
enStepMotor motor[3]{};
enOp op_panel{};

api_cmd tool_cmd;
void apInit(void)
{

  {
    using namespace CMD;
    uart_cmd::cfg_t cfg{};
    cfg.ch = HW_UART_CH_RS485;
    cfg.baud = 115200;
    cmd_comm.Init(cfg);
  }

#ifdef _USE_HW_CLI

#endif

  /* operating panel sw initial */
  {
    enOp::cfg_t cfg = {};
    cfg.ptr_mcu_io = &mcu_io;

    op_panel.Init(cfg);
  }
  {
    enStepMotor::cfg_t cfg{};
    cfg.motor_id = 0;
    cfg.gpio_pin_enable = IO_OUT_7_Pin;
    cfg.gpio_port_enable = IO_OUT_7_GPIO_Port;
    cfg.gpio_pin_dir = IO_OUT_8_Pin;
    cfg.gpio_port_dir = IO_OUT_8_GPIO_Port;
    cfg.gpio_pin_step = IO_OUT_9_Pin;
    cfg.gpio_port_step = IO_OUT_9_GPIO_Port;
    cfg.ptr_timer = timGetData(_DEF_TIM1);
    motor[0].Init(cfg);
  }
  {
    enStepMotor::cfg_t cfg{};
    cfg.motor_id = 0;
    cfg.gpio_pin_enable = IO_OUT_10_Pin;
    cfg.gpio_port_enable = IO_OUT_10_GPIO_Port;
    cfg.gpio_pin_dir = IO_OUT_11_Pin;
    cfg.gpio_port_dir = IO_OUT_11_GPIO_Port;
    cfg.gpio_pin_step = IO_OUT_12_Pin;
    cfg.gpio_port_step = IO_OUT_12_GPIO_Port;
    cfg.ptr_timer = timGetData(_DEF_TIM1);
    motor[1].Init(cfg);
  }
  {
    enStepMotor::cfg_t cfg{};
    cfg.motor_id = 0;
    cfg.gpio_pin_enable = IO_OUT_13_Pin;
    cfg.gpio_port_enable = IO_OUT_13_GPIO_Port;
    cfg.gpio_pin_dir = IO_OUT_14_Pin;
    cfg.gpio_port_dir = IO_OUT_14_GPIO_Port;
    cfg.gpio_pin_step = IO_OUT_15_Pin;
    cfg.gpio_port_step = IO_OUT_15_GPIO_Port;
    cfg.ptr_timer = timGetData(_DEF_TIM1);
    motor[2].Init(cfg);
  }
  {
    api_cmd::cfg_t cfg{};
    cfg.ptr_comm = &cmd_comm;
    cfg.ptr_io = &mcu_io;
    cfg.ptr_mcu_data = &mcu_data;
    cfg.ptr_mcu_reg = &mcu_reg;

    tool_cmd.Init(cfg);
  }

  /*Assign Obj */
  mcu_io.Init();

#ifdef _USE_HW_CLI
  cliAdd("app", cliApp);
#endif
}

void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();

  while (1)
  {

    if (millis() - pre_time >= 1000)
    {
      pre_time = millis();
      ledToggle(_DEF_LED1);
    }

    /* 0. tool cmd*/
    tool_cmd.ThreadJob();

    updateApReg();

    eventOpPanel();

    updateLamp();

    updateErr();

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

/*
 * operating button event
 */
void eventOpPanel()
{
}

/*
 * operating lamp and buzzer
 */
void updateLamp()
{
}

void updateErr()
{
}

/*
 * mcu register update
 */
void updateApReg()
{
  /* 1. io register */
  mcu_io.Update_io();

  /* 2. motor data */
  enum : uint8_t
  {
    io_1d,
    io_2d,
    io_3d,
    io_max
  };
  enum : uint8_t
  {
    motor_1d,
    motor_2d,
    motor_3d,
    motor_4d,
    motor_max
  };
  enum : uint8_t
  {
    data_1d,
    data_2d,
    data_3d,
    data_4d,
    data_max
  };
  mcu_data.reg_sys = mcu_io.m_sysio.system_io;
  mcu_data.reg_state = mcu_reg.state_reg.ap_state;
  mcu_data.reg_option = mcu_reg.option_reg.ap_option;
  mcu_data.reg_err = mcu_reg.error_reg.ap_error;
  mcu_data.io_in[io_1d] = mcu_io.m_in.data;
  mcu_data.io_in[io_2d] = 0;
  mcu_data.io_in[io_3d] = 0;
  mcu_data.io_out[io_1d] = mcu_io.m_out.data;
  mcu_data.io_out[io_2d] = 0;
  mcu_data.io_out[io_3d] = 0;
  mcu_data.motor_cnt = 3;
  mcu_data.motor_pulse[motor_1d] = 1'000; // 500'000;
  mcu_data.motor_status[motor_1d] = 9;
  mcu_data.motor_pulse[motor_2d] = 2'000'000;
  mcu_data.motor_status[motor_2d] = 8;
  mcu_data.motor_pulse[motor_3d] = 0;
  mcu_data.motor_status[motor_3d] = 0;
  mcu_data.motor_pulse[motor_4d] = 0;
  mcu_data.motor_status[motor_4d] = 0;
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
