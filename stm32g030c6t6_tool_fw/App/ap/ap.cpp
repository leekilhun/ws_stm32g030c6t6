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
static void updateErr();
static void eventOpPanel();
static void updateLamp();

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif

/****************************************************
  0. mcu data and register
 ****************************************************/
ap_io mcu_io;
CMD::uart_cmd cmd_comm;
ap_reg mcu_reg;
mcu_data_st mcu_data;

/****************************************************
  1. ap instances
 ****************************************************/
enStepMotor motor[AP_OBJ::MOTOR_MAX]{};
enOp op_panel;
enBtn btns[AP_OBJ::BTN_MAX];
enLed leds[AP_OBJ::LED_MAX];

// control
cnAuto autoManager;
// cnJob process;
cnTasks tasks;
api_cmd tool_cmd;

void apInit(void)
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

  {
    using namespace CMD;
    uart_cmd::cfg_t cfg{};
    cfg.ch = HW_UART_CH_RS485;
    cfg.baud = 115200;
    cmd_comm.Init(cfg);
  }

#ifdef _USE_HW_CLI

#endif

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
    motor[AP_OBJ::MOTOR_A].Init(cfg);
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
    motor[AP_OBJ::MOTOR_B].Init(cfg);
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
    motor[AP_OBJ::MOTOR_C].Init(cfg);
  }

  {
    enBtn::cfg_t cfg;
    cfg.gpio_port = IO_IN_0_GPIO_Port;
    cfg.gpio_pin = IO_IN_0_Pin;
    cfg.repeat_time_detect = 60; // ISR count
    cfg.repeat_time = 200;
    cfg.repeat_time_delay = 250;
    memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
    strlcpy(&cfg.name_str[0], "0_START", sizeof(cfg.name_str));
    if (btns[AP_OBJ::BTN_START].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_START] Init Failed!");

    cfg.gpio_port = IO_IN_1_GPIO_Port;
    cfg.gpio_pin = IO_IN_1_Pin;
    cfg.repeat_time_detect = 60; // ISR count
    cfg.repeat_time = 200;
    cfg.repeat_time_delay = 250;
    memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
    strlcpy(&cfg.name_str[0], "1_RESET", sizeof(cfg.name_str));
    if (btns[AP_OBJ::BTN_RESET].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_RESET] Init Failed!");

    cfg.gpio_port = IO_IN_2_GPIO_Port;
    cfg.gpio_pin = IO_IN_2_Pin;
    cfg.repeat_time_detect = 60; // ISR count
    cfg.repeat_time = 200;
    cfg.repeat_time_delay = 250;
    memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
    strlcpy(&cfg.name_str[0], "2_STOP", sizeof(cfg.name_str));
    if (btns[AP_OBJ::BTN_STOP].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_STOP] Init Failed!");
  }

  {
    enLed::cfg_t cfg;
    cfg.gpio_port = STATUS_GPIO_Port;
    cfg.gpio_pin = STATUS_Pin;
    if (leds[AP_OBJ::LED_STATUS].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("status_led Init Failed!");

    cfg.gpio_port = IO_OUT_0_GPIO_Port;
    cfg.gpio_pin = IO_OUT_0_Pin;
    if (leds[AP_OBJ::LED_OP_G_READY].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("OP Green Ready LED Init Failed!");

    cfg.gpio_port = IO_OUT_1_GPIO_Port;
    cfg.gpio_pin = IO_OUT_1_Pin;
    if (leds[AP_OBJ::LED_OP_Y_BUSY].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("OP Yellow Busy LED Init Failed!");

    cfg.gpio_port = IO_OUT_2_GPIO_Port;
    cfg.gpio_pin = IO_OUT_2_Pin;
    if (leds[AP_OBJ::LED_OP_R_ERROR].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("OP Red Error LED Init Failed!");
  }

  /* operating panel sw initial */
  {
    enOp::cfg_t cfg = {};
    cfg.ptr_mcu_io = &mcu_io;
    cfg.ptr_mcu_reg = &mcu_reg;

    cfg.ptr_btn_reset = &btns[AP_OBJ::BTN_RESET];
    cfg.ptr_btn_start = &btns[AP_OBJ::BTN_START];
    cfg.ptr_btn_stop = &btns[AP_OBJ::BTN_STOP];
    cfg.ptr_btn_estop = nullptr;

    cfg.ptr_led_op_g = &leds[AP_OBJ::LED_OP_G_READY];
    cfg.ptr_led_op_y = &leds[AP_OBJ::LED_OP_Y_BUSY];
    cfg.ptr_led_op_r = &leds[AP_OBJ::LED_OP_R_ERROR];
    cfg.ptr_led_status = &leds[AP_OBJ::LED_STATUS];    

    op_panel.Init(cfg);
  }

  /* automanager initial */
  {
    cnAuto::cfg_t auto_cfg = {};
    auto_cfg.ptr_apReg = &mcu_reg;
    auto_cfg.ptr_op = &op_panel;
    auto_cfg.ptr_io = &mcu_io;
    autoManager.Init(auto_cfg);
  }
  
  	/* task jos initial */
	{
		cnTasks::cfg_t cfg = {};
		tasks.Init(cfg);
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
  // uint32_t out_cnt = 0;
  pre_time = millis();

  LOG_PRINT("Start App Loop");
  /* Infinite loop */
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();
    }

    /* 0. auto management*/
    autoManager.ThreadJob();

    /**/
    tasks.ThreadJob();

    /* 1. tool cmd*/
    tool_cmd.ThreadJob();

#ifdef _USE_HW_CLI
    cliMain();
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
  /**
   * @brief
   * status led
   */
  {
    static uint32_t pre_time_update_led;

    if (millis() - pre_time_update_led >= 1'000)
    {
      pre_time_update_led = millis();
      leds[AP_OBJ::LED_STATUS].Toggle();
    }
  }
  // end of status led

  {
  }
  const enOp::status_e system_init = enOp::status_e::INIT;
  const enOp::status_e auto_stop = enOp::status_e::STEP_STOP;
  const enOp::status_e auto_ready = enOp::status_e::RUN_READY;
  const enOp::status_e auto_working = enOp::status_e::RUN;
  const enOp::status_e error_stop = enOp::status_e::ERR_STOP;

  // const enOp::lamp_e g_ready = enOp::LAMP_READY;
  // const enOp::lamp_e y_busy = enOp::LAMP_BUSY;
  // const enOp::lamp_e r_error = enOp::LAMP_ERROR;

  static uint32_t on_led_ms;
  switch (autoManager.GetOPStatus())
  {
  case system_init:
  {
    constexpr int system_init_ing_ms = 500;
    if (millis() - on_led_ms >= system_init_ing_ms)
    {
      on_led_ms = millis();

      op_panel.LampToggle(enOp::LAMP_READY);
      op_panel.LampToggle(enOp::LAMP_BUSY);
      op_panel.LampToggle(enOp::LAMP_ERROR);
    }
  }
  break;

  case auto_stop:
  {
  }
  break;

  case auto_ready:
  {
  }
  break;

  case auto_working: //
  {
  }
  break;

  case error_stop:
  {
  }
  break;

  default:
    break;
  }
  // end of switch (autoManager.GetOPStatus())
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
  mcu_io.Update_io();
}

void apISR_10ms(void *arg)
{
  updateLamp();
  eventOpPanel();

  for (auto &elm : btns)
    elm.ISR();
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
