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
static void apISR_50ms(void *arg);

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
// CMD::uart_cmd cmd_comm;
#ifdef _USE_EXHW_IICOMM_DEV
iic_cmd cmd_comm;
#else
CMD::uart_cmd cmd_comm;
#endif
ap_reg                   mcu_reg;
mcu_tool_data_align_48_t mcu_data;
mcu_tool_data_align_48_t for_compare_data;

/****************************************************
  1. ap instances
 ****************************************************/
enStepMotor motor[AP_OBJ::MOTOR_MAX]{};
enOp        op_panel;
enBtn       btns[AP_OBJ::BTN_MAX];
enLed       leds[AP_OBJ::LED_MAX];

cnMotors motors;

// control
// cnAuto autoManager;
// cnJob process;
// cnTasks tasks;

api_cmd tool_cmd;

void apInit(void)
{

  // #ifdef _USE_HW_IIC_S
  //   if (iicsBegin(_DEF_IIC_S1, IICS_FREQ_400KHz))
  //     LOG_PRINT("iicsBegin  Success!");
  //   else
  //     LOG_PRINT("iicsBegin  Fail!");
  // #endif

#ifdef _USE_EXHW_IICOMM_DEV
  {
    iic_cmd::cfg_t cfg{};
    cfg.ReadReg  = iicomm_devReadReg;
    cfg.WriteReg = iicomm_devWriteReg;
    cmd_comm.Init(cfg);
  }
#else
  {
    using namespace CMD;
    uart_cmd::cfg_t cfg{};
    cfg.ch   = HW_UART_PC;
    cfg.baud = 115200;
    if (cmd_comm.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("cmd_comm Init  Fail!");
  }
#endif

#ifdef _USE_HW_CLI

#endif

  {
      {enStepMotor::cfg_t cfg{};
  cfg.obj_idx           = AP_OBJ::MOTOR_A;
  cfg.sm_type           = stepmotorType_e::cw_to_zero;
  cfg.io_input_idx_zero = ap_io::in_e::in07_a_motor_org;
  cfg.ptr_io            = &mcu_io;
  cfg.timer_id          = _DEF_TIM1;
  cfg.gpio_pin_enable   = IO_OUT_7_Pin;
  cfg.gpio_port_enable  = IO_OUT_7_GPIO_Port;
  cfg.gpio_pin_dir      = IO_OUT_8_Pin;
  cfg.gpio_port_dir     = IO_OUT_8_GPIO_Port;
  cfg.gpio_pin_step     = IO_OUT_9_Pin;
  cfg.gpio_port_step    = IO_OUT_9_GPIO_Port;
  cfg.ptr_timer         = timGetData(_DEF_TIM1);
  if (motor[AP_OBJ::MOTOR_A].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("motor[AP_OBJ::MOTOR_A] Init Failed!");
}
{
  enStepMotor::cfg_t cfg{};
  cfg.obj_idx           = AP_OBJ::MOTOR_B;
  cfg.sm_type           = stepmotorType_e::ccw_to_zero;
  cfg.io_input_idx_zero = ap_io::in_e::in12_b_motor_org;
  cfg.ptr_io            = &mcu_io;
  cfg.timer_id          = _DEF_TIM2;
  cfg.gpio_pin_enable   = IO_OUT_10_Pin;
  cfg.gpio_port_enable  = IO_OUT_10_GPIO_Port;
  cfg.gpio_pin_dir      = IO_OUT_11_Pin;
  cfg.gpio_port_dir     = IO_OUT_11_GPIO_Port;
  cfg.gpio_pin_step     = IO_OUT_12_Pin;
  cfg.gpio_port_step    = IO_OUT_12_GPIO_Port;
  cfg.ptr_timer         = timGetData(_DEF_TIM2);
  if (motor[AP_OBJ::MOTOR_B].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("motor[AP_OBJ::MOTOR_B] Init Failed!");
}
{
  enStepMotor::cfg_t cfg{};
  cfg.obj_idx              = AP_OBJ::MOTOR_C;
  cfg.sm_type              = stepmotorType_e::floating_zero;
  cfg.io_input_idx_zero    = ap_io::in_e::in15_c_r_motor_org;
  cfg.io_input_idx_zero_ex = ap_io::in_e::in16_c_l_motor_org;
  cfg.ptr_io               = &mcu_io;
  cfg.timer_id             = _DEF_TIM3;
  cfg.gpio_pin_enable      = IO_OUT_13_Pin;
  cfg.gpio_port_enable     = IO_OUT_13_GPIO_Port;
  cfg.gpio_pin_dir         = IO_OUT_14_Pin;
  cfg.gpio_port_dir        = IO_OUT_14_GPIO_Port;
  cfg.gpio_pin_step        = IO_OUT_15_Pin;
  cfg.gpio_port_step       = IO_OUT_15_GPIO_Port;
  cfg.ptr_timer            = timGetData(_DEF_TIM3);
  if (motor[AP_OBJ::MOTOR_C].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("motor[AP_OBJ::MOTOR_C] Init Failed!");
}
}

{
  enBtn::cfg_t cfg;
  cfg.gpio_port          = IO_IN_0_GPIO_Port;
  cfg.gpio_pin           = IO_IN_0_Pin;
  cfg.repeat_time_detect = 6; // ISR count
  cfg.repeat_time        = 20;
  cfg.repeat_time_delay  = 25;
  memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
  strlcpy(&cfg.name_str[0], "0_START", sizeof(cfg.name_str));
  if (btns[AP_OBJ::BTN_START].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("btns[AP_OBJ::BTN_START] Init Failed!");

  cfg.gpio_port          = IO_IN_1_GPIO_Port;
  cfg.gpio_pin           = IO_IN_1_Pin;
  cfg.repeat_time_detect = 6; // ISR count
  cfg.repeat_time        = 20;
  cfg.repeat_time_delay  = 25;
  memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
  strlcpy(&cfg.name_str[0], "1_RESET", sizeof(cfg.name_str));
  if (btns[AP_OBJ::BTN_RESET].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("btns[AP_OBJ::BTN_RESET] Init Failed!");

  cfg.gpio_port          = IO_IN_2_GPIO_Port;
  cfg.gpio_pin           = IO_IN_2_Pin;
  cfg.repeat_time_detect = 6; // ISR count
  cfg.repeat_time        = 20;
  cfg.repeat_time_delay  = 25;
  memset(&cfg.name_str[0], 0, sizeof(cfg.name_str));
  strlcpy(&cfg.name_str[0], "2_STOP", sizeof(cfg.name_str));
  if (btns[AP_OBJ::BTN_STOP].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("btns[AP_OBJ::BTN_STOP] Init Failed!");
}

{
  enLed::cfg_t cfg;
  cfg.gpio_port = STATUS_GPIO_Port;
  cfg.gpio_pin  = STATUS_Pin;
  if (leds[AP_OBJ::LED_STATUS].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("status_led Init Failed!");

  cfg.gpio_port = IO_OUT_0_GPIO_Port;
  cfg.gpio_pin  = IO_OUT_0_Pin;
  if (leds[AP_OBJ::LED_OP_G_READY].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("OP Green Ready LED Init Failed!");

  cfg.gpio_port = IO_OUT_1_GPIO_Port;
  cfg.gpio_pin  = IO_OUT_1_Pin;
  if (leds[AP_OBJ::LED_OP_Y_BUSY].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("OP Yellow Busy LED Init Failed!");

  cfg.gpio_port = IO_OUT_2_GPIO_Port;
  cfg.gpio_pin  = IO_OUT_2_Pin;
  if (leds[AP_OBJ::LED_OP_R_ERROR].Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("OP Red Error LED Init Failed!");
}

/* operating panel sw initial */
{
  enOp::cfg_t cfg = {};
  cfg.ptr_mcu_io  = &mcu_io;
  cfg.ptr_mcu_reg = &mcu_reg;

  cfg.ptr_btn_reset = &btns[AP_OBJ::BTN_RESET];
  cfg.ptr_btn_start = &btns[AP_OBJ::BTN_START];
  cfg.ptr_btn_stop  = &btns[AP_OBJ::BTN_STOP];
  cfg.ptr_btn_estop = nullptr;

  cfg.ptr_led_op_g   = &leds[AP_OBJ::LED_OP_G_READY];
  cfg.ptr_led_op_y   = &leds[AP_OBJ::LED_OP_Y_BUSY];
  cfg.ptr_led_op_r   = &leds[AP_OBJ::LED_OP_R_ERROR];
  cfg.ptr_led_status = &leds[AP_OBJ::LED_STATUS];

  if (op_panel.Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("OP_PANEL Init Failed!");
}

{
  cnMotors::cfg_t cfg{};
  cfg.ptr_apReg = &mcu_reg;
  cfg.ptr_io    = &mcu_io;
  cfg.ptr_motor = motor;

  if (motors.Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("motors Init Failed!");
}

/* automanager initial */
{
  // cnAuto::cfg_t auto_cfg = {};
  // auto_cfg.ptr_apReg = &mcu_reg;
  // auto_cfg.ptr_op = &op_panel;
  // auto_cfg.ptr_io = &mcu_io;
  // if (autoManager.Init(auto_cfg) != ERROR_SUCCESS)
  //   LOG_PRINT("autoManager Init Failed!");
}

/* task jos initial */
{
  // cnTasks::cfg_t cfg = {};
  // if (tasks.Init(cfg) != ERROR_SUCCESS)
  //   LOG_PRINT("tasks Init Failed!");
}

{
  api_cmd::cfg_t cfg{};
  cfg.ptr_comm     = &cmd_comm;
  cfg.ptr_io       = &mcu_io;
  cfg.ptr_mcu_data = &mcu_data;
  cfg.ptr_mcu_reg  = &mcu_reg;
  cfg.ptr_motors   = &motors;

  if (tool_cmd.Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("tool_cmd Init  Fail!");
}

/*Assign Obj */
mcu_io.Init();

#ifdef _USE_HW_CLI
cliAdd("app", cliApp);
#endif
}

// extern TIM_HandleTypeDef htim3;
// extern TIM_HandleTypeDef htim14;
// extern TIM_HandleTypeDef htim16;

void apMain(void)
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

    timer_ch = swtimerGetHandle();
    if (timer_ch >= 0)
    {
      swtimerSet(timer_ch, 50, LOOP_TIME, apISR_50ms, NULL);
      swtimerStart(timer_ch);
    }
    else
    {
      LOG_PRINT("[NG] 50ms swtimerGetHandle()");
    }
  }
  // end of swtimer

  {
    /* for motor control, start timer*/
    timStart(_DEF_TIM1);
    timStart(_DEF_TIM2);
    timStart(_DEF_TIM3);
  }

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

      // LOG_PRINT("TIM14->SR [0x%08X], motor2 step[%d] wait[%d]",TIM14->SR, motor[AP_OBJ::MOTOR_B].m_step.curr_step, motor[AP_OBJ::MOTOR_B].m_step.wait_step);
    }

    /* 0. auto management*/
    // autoManager.ThreadJob();

    /**/
    // tasks.ThreadJob();

    /* 1. tool cmd*/
    tool_cmd.ThreadJob();

    eventOpPanel();
    updateApReg();

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

  const enOp::status_e system_init  = enOp::status_e::INIT;
  const enOp::status_e auto_stop    = enOp::status_e::STEP_STOP;
  const enOp::status_e auto_ready   = enOp::status_e::RUN_READY;
  const enOp::status_e auto_working = enOp::status_e::RUN;
  const enOp::status_e error_stop   = enOp::status_e::ERR_STOP;

  /**
   * @brief set tool system status
   *
   */
  {
    if (motors.IsOriginCplt(AP_OBJ::MOTOR_MAX))
    {
      op_panel.SetStatus(enOp::status_e::STEP_STOP);
    }
    else
    {
      op_panel.SetStatus(enOp::status_e::INIT);
    }
  }

  //constexpr uint8_t test_motor_idx = AP_OBJ::MOTOR_B;
  switch (op_panel.GetStatus())
  {
  case system_init:
  {
    if (btns[AP_OBJ::BTN_START].IsLongKey())
    {
      LOG_PRINT("start btn Pressed");
      if (motors.IsMotorOn())
      {
        if (motors.DoOrigin(AP_OBJ::MOTOR_MAX) != ERROR_SUCCESS)
          LOG_PRINT("Motor Do Origin fail!");
      }
      else
      {
        if (motors.MotorOnOff(AP_OBJ::MOTOR_MAX, true) != ERROR_SUCCESS)
          LOG_PRINT("MotorOnOff fail!");
      }

      // if (motor[test_motor_idx].m_status.motor_enabled == false)
      //   motor[test_motor_idx].Enable();

      // if (motor[test_motor_idx].SetZeroPosition() != ERROR_SUCCESS)
      //   LOG_PRINT("SetZeroPosition fail!");
    }
  }
  break;

  case auto_stop:
  {
    // if (btns[AP_OBJ::BTN_START].IsLongKey())
    // {
    //   btns[AP_OBJ::BTN_START].ResetEvent();
    //   if (motor[test_motor_idx].m_status.motor_enabled == false)
    //     motor[test_motor_idx].Enable();
    // }
    // else if (btns[AP_OBJ::BTN_RESET].IsLongKey())
    // {
    //   btns[AP_OBJ::BTN_RESET].ResetEvent();
    //   // motor[test_motor_idx].Run(1600, 50);
    //   if (motor[test_motor_idx].SetZeroPosition() != ERROR_SUCCESS)
    //     LOG_PRINT("SetZeroPosition fail!");
    //   // motor[test_motor_idx].test_constant_run(1600,1);

    //   // else
    //   //   LOG_PRINT("state [0x%02X]", motor[test_motor_idx].state);
    // }
    // else if (btns[AP_OBJ::BTN_STOP].IsShortKey())
    // {
    //   btns[AP_OBJ::BTN_STOP].ResetEvent();
    //   if (motor[test_motor_idx].m_status.motor_enabled)
    //     motor[test_motor_idx].Disable();
    // }
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
    const enOp::status_e system_init  = enOp::status_e::INIT;
    const enOp::status_e auto_stop    = enOp::status_e::STEP_STOP;
    const enOp::status_e auto_ready   = enOp::status_e::RUN_READY;
    const enOp::status_e auto_working = enOp::status_e::RUN;
    const enOp::status_e error_stop   = enOp::status_e::ERR_STOP;

    // const enOp::lamp_e g_ready = enOp::LAMP_READY;
    // const enOp::lamp_e y_busy = enOp::LAMP_BUSY;
    // const enOp::lamp_e r_error = enOp::LAMP_ERROR;

    static uint32_t on_led_ms;
    switch (op_panel.GetStatus())
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

  /**
   * @brief indicator led
   *
   */
  {
    bool is_run[AP_OBJ::MOTOR_MAX] =
        {motor[AP_OBJ::MOTOR_A].m_status.moving != 0,
         motor[AP_OBJ::MOTOR_B].m_status.moving != 0,
         motor[AP_OBJ::MOTOR_C].m_status.moving != 0};
    enum : uint8_t
    {
      LED_IDLE,
      LED_ON,
      LED_OFF,
    };

    constexpr uint8_t LED_IDLE_TIME = 50;

    static uint32_t motor_state_pre_time[AP_OBJ::MOTOR_MAX];
    static uint8_t  motor_state[AP_OBJ::MOTOR_MAX] = {LED_IDLE, LED_IDLE, LED_IDLE};
    for (uint8_t i = 0; i < AP_OBJ::MOTOR_MAX; i++)
    {
      switch (motor_state[i])
      {
      case LED_IDLE:
        if (is_run[i])
        {
          motor_state[i]          = LED_ON;
          motor_state_pre_time[i] = millis();
          op_panel.LampOnOff((enOp::lamp_e)(enOp::LAMP_READY + i), true);
        }
        break;

      case LED_ON:
        if (millis() - motor_state_pre_time[i] >= LED_IDLE_TIME)
        {
          motor_state_pre_time[i] = millis();
          motor_state[i]          = LED_OFF;
          op_panel.LampOnOff((enOp::lamp_e)(enOp::LAMP_READY + i), false);
        }

      case LED_OFF:
        if (millis() - motor_state_pre_time[i] >= LED_IDLE_TIME)
        {
          motor_state_pre_time[i] = millis();
          motor_state[i]          = LED_IDLE;
          // op_panel.LampOnOff((enOp::lamp_e)(enOp::LAMP_READY + i), false);
        }

      default:
        break;
      }
    }
  }
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
    io_max
  };
  enum : uint8_t
  {
    motor_1d,
    motor_2d,
    motor_3d,
    motor_max
  };
  enum : uint8_t
  {
    data_1d,
    data_2d,
    data_max
  };
  mcu_data.reg_sys                = mcu_io.m_sysio.system_io;
  mcu_data.reg_state              = mcu_reg.state_reg.ap_state;
  mcu_data.reg_option             = mcu_reg.option_reg.ap_option;
  mcu_data.reg_err                = mcu_reg.error_reg.ap_error;
  mcu_data.io_in[io_1d]           = mcu_io.m_in.data;
  mcu_data.io_out[io_1d]          = mcu_io.m_out.data;
  mcu_data.motor_cnt              = motor_max;
  mcu_data.motor_pulse[motor_1d]  = motor[AP_OBJ::MOTOR_A].step_position; // 1'000; // 500'000;
  mcu_data.motor_status[motor_1d] = motor[AP_OBJ::MOTOR_A].m_status.sc_status;
  mcu_data.motor_pulse[motor_2d]  = motor[AP_OBJ::MOTOR_B].step_position;
  mcu_data.motor_status[motor_2d] = motor[AP_OBJ::MOTOR_B].m_status.sc_status;
  mcu_data.motor_pulse[motor_3d]  = motor[AP_OBJ::MOTOR_C].step_position;
  mcu_data.motor_status[motor_3d] = motor[AP_OBJ::MOTOR_C].m_status.sc_status;
  mcu_data.datas[data_1d]         = 0;
  mcu_data.datas[data_2d]         = 0;
}

#ifdef _USE_EXHW_IICOMM_DEV
void SendPeriodDataIIC()
{
  if (cmd_comm.SetMcuData(&mcu_data) != ERROR_SUCCESS)
    LOG_PRINT("cmd_comm.SetMcuData Error");
}

void processIIC()
{
  switch (cmd_comm.Process())
  {
  case iic_cmd::CMD_ORD_MOTOR_ENABLE:
  {
    // LOG_PRINT("u8Data[0][0x%02X],u8Data[1][0x%02X],u8Data[2][0x%02X],u8Data[3][0x%02X]",
    //           cmd_comm.orderDatas[0].u8Data[0], cmd_comm.orderDatas[0].u8Data[1], cmd_comm.orderDatas[0].u8Data[2], cmd_comm.orderDatas[0].u8Data[3]);
    if (cmd_comm.orderDatas[0].u8Data[0])
    {
      if (motors.MotorOnOff(cmd_comm.orderDatas[0].u8Data[0]) != ERROR_SUCCESS)
        LOG_PRINT("MotorOnOff Error");
      /*
      data value B0000'0001  motor 1
      data value B0000'0010  motor 2
      data value B0000'0100  motor 3
      data value B0000'1000  motor 4
      motor 1 ~ 4 B0000'1111
      */
      // MotorOnOff
    }
    goto reset_reg;
  }
  break;

  case iic_cmd::CMD_ORD_MOTOR_DISABLE:
  {
    if (cmd_comm.orderDatas[0].u8Data[0])
    {
      if (motors.MotorOnOff(cmd_comm.orderDatas[0].u8Data[0], false) != ERROR_SUCCESS)
        LOG_PRINT("MotorOnOff Error");
    }
    goto reset_reg;
  }
  break;

  case iic_cmd::CMD_ORD_MOTOR_RUN:
  {
    uint8_t motor_id = cmd_comm.orderDatas[0].u8Data[0];
    data_t  cnt_value{cmd_comm.orderDatas[0].u8Data[1], cmd_comm.orderDatas[0].u8Data[2], cmd_comm.orderDatas[0].u8Data[3], 0};
    // LOG_PRINT("CMD_ORD_MOTOR_RUN ID[0x%02X] CNT[%d]", motor_id, cnt_value.s32D);
    if ((motor_id > 0) && (cnt_value.s32D != 0))
    {
      if (motors.Move(motor_id, cnt_value.s32D) != ERROR_SUCCESS)
        LOG_PRINT("Move Error");
    }
    goto reset_reg;
  }
  break;

  case iic_cmd::CMD_ORD_MOTOR_ORG:
  {
    uint8_t motor_id = cmd_comm.orderDatas[0].u8Data[0];
    if (motor_id > 0)
    {
      if (motors.DoOrigin(motor_id) != ERROR_SUCCESS)
        LOG_PRINT("DoOrigin Error");
    }
    goto reset_reg;
  }
  break;

  default:
    break;
  }

  if (cmd_comm.GetOutReg() != ERROR_SUCCESS)
  {
    LOG_PRINT("cmd_comm.GetOutReg Error");
  }
  else
  {
    // mcu_io.SetOutputReg(cmd_comm.outRegs[0]);
  }
  return;

reset_reg:
  if (cmd_comm.ResetOrderRegister() != ERROR_SUCCESS)
  {
    LOG_PRINT("cmd_comm.ResetOrderRegister Error");
  }
}
#else

#endif

void apISR_1ms(void *arg)
{
  updateErr();
#ifdef _USE_EXHW_IICOMM_DEV
  processIIC();
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

void apISR_10ms(void *arg)
{
  updateLamp();
#ifdef _USE_EXHW_IICOMM_DEV
  SendPeriodDataIIC();
#endif

  for (auto &elm : btns)
    elm.ISR();
}

void apISR_50ms(void *arg)
{

  if (mcu_reg.option_reg.use_auto_senddata)
  {
    if (std::memcmp((uint8_t *)&mcu_data, (uint8_t *)&for_compare_data, sizeof(mcu_data)) != 0)
    {
      std::memcpy((uint8_t *)&for_compare_data, (uint8_t *)&mcu_data, sizeof(mcu_data));
      if (tool_cmd.SendData(CMD::TX_TOOL_DATA) != ERROR_SUCCESS)
        LOG_PRINT("tool_cmd.SetMcuData Error");
    }
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

    // int result = 0;
  }

  if (ret == false)
  {
    cliPrintf("app info \n");
  }
}
#endif
