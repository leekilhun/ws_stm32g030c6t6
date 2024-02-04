/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"


#if 1

/**
 * @brief
 * It must be written in non-block code as a function called in 1ms time.
 * @param arg
 */
static void apISR_1ms(void *arg);
static void apISR_10ms(void *arg);
static void apISR_50ms(void *arg);

static void updateApReg();
static void updateLamp();
static void eventOpPanel();

// callback function
// static errno_t SetOutputRegist(uint32_t reg);
// static void    errorCBFunc(void *obj, void *w_parm, void *l_parm);

/****************************************************
  0. mcu data and register
 ****************************************************/
ap_reg mcu_reg;
ap_io  mcu_io;
/****************************************************
  1. ap instances
 ****************************************************/
enLed   leds[AP_OBJ::LED_MAX];
enOp    op_panel;
enBtn   btns[AP_OBJ::BTN_MAX];
enWheel wheels;

#define StatusLeDToggle HAL_GPIO_TogglePin(STATUS_GPIO_Port, STATUS_Pin)

void apInit(void)
{

  {
    constexpr uint8_t hc165_ch = 0;
    ap_io::cfg_t      cfg;
    cfg.ext_in_hc165_ch = hc165_ch;
    if (mcu_io.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("mcu_io Init Failed!");
  }

  {
    enWheel::cfg_t cfg;
    cfg.timer_id              = _TIM_3;
    cfg.ptr_timer             = timGetData(_TIM_3);
    cfg.pulse_per_revolution  = 100;
    cfg.clock_frequence       = 64'000'000;
    cfg.speed_cals_interval   = 1'000;
    cfg.speed_unit_conversion = 60;
    cfg.set_name("WHEEL");
    if (wheels.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("wheels Init Failed!");
  }

  {
    enLed::cfg_t cfg;
    cfg.gpio_port = IO_OPE_LAMP_START_GPIO_Port;
    cfg.gpio_pin  = IO_OPE_LAMP_START_Pin;
    cfg.set_name("LED_OP_G_START");
    if (leds[AP_OBJ::LED_OP_G_START].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_OP_G_START Init Failed!");

    cfg.gpio_port = IO_OPE_LAMP_STOP_GPIO_Port;
    cfg.gpio_pin  = IO_OPE_LAMP_STOP_Pin;
    cfg.set_name("LED_OP_G_STOP");
    if (leds[AP_OBJ::LED_OP_R_STOP].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_OP_R_STOP Init Failed!");

    cfg.gpio_port = IO_OPE_LAMP_RESET_GPIO_Port;
    cfg.gpio_pin  = IO_OPE_LAMP_RESET_Pin;
    cfg.set_name("LED_OP_Y_RESET");
    if (leds[AP_OBJ::LED_OP_Y_RESET].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_OP_Y_RESET Init Failed!");

    cfg.gpio_port = IO_TOWER_GREEN_GPIO_Port;
    cfg.gpio_pin  = IO_TOWER_GREEN_Pin;
    cfg.set_name("LED_TOWER_GREEN");
    if (leds[AP_OBJ::LED_TOWER_GREEN].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_TOWER_GREEN Init Failed!");

    cfg.gpio_port = IO_TOWER_YELLOW_GPIO_Port;
    cfg.gpio_pin  = IO_TOWER_YELLOW_Pin;
    cfg.set_name("LED_TOWER_YELLOW");
    if (leds[AP_OBJ::LED_TOWER_YELLOW].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_TOWER_YELLOW Init Failed!");

    cfg.gpio_port = IO_TOWER_RED_GPIO_Port;
    cfg.gpio_pin  = IO_TOWER_RED_Pin;
    cfg.set_name("LED_TOWER_RED");
    if (leds[AP_OBJ::LED_TOWER_RED].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("LED_TOWER_RED Init Failed!");
  }

  {
    enBtn::cfg_t cfg;
    cfg.gpio_port          = IO_OPE_KEY_START_GPIO_Port;
    cfg.gpio_pin           = IO_OPE_KEY_START_Pin;
    cfg.repeat_time_detect = 10; // ISR count
    cfg.repeat_time        = 30;
    cfg.repeat_time_delay  = 60;
    cfg.set_name("1_OP_START");
    if (btns[AP_OBJ::BTN_OP_START].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_START] Init Failed!");

    cfg.gpio_port          = IO_OPE_KEY_STOP_GPIO_Port;
    cfg.gpio_pin           = IO_OPE_KEY_STOP_Pin;
    cfg.repeat_time_detect = 10; // ISR count
    cfg.repeat_time        = 30;
    cfg.repeat_time_delay  = 60;
    cfg.set_name("2_OP_STOP");
    if (btns[AP_OBJ::BTN_OP_STOP].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_STOP] Init Failed!");

    cfg.gpio_port          = IO_OPE_KEY_RESET_GPIO_Port;
    cfg.gpio_pin           = IO_OPE_KEY_RESET_Pin;
    cfg.repeat_time_detect = 10; // ISR count
    cfg.repeat_time        = 30;
    cfg.repeat_time_delay  = 60;
    cfg.set_name("3_OP_RESET");
    if (btns[AP_OBJ::BTN_OP_RESET].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_RESET] Init Failed!");

    cfg.gpio_port          = IO_OPE_ESTOP_GPIO_Port;
    cfg.gpio_pin           = IO_OPE_ESTOP_Pin;
    cfg.repeat_time_detect = 10; // ISR count
    cfg.repeat_time        = 30;
    cfg.repeat_time_delay  = 60;
    cfg.set_name("4_OP_ESTOP");
    if (btns[AP_OBJ::BTN_OP_ESTOP].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("btns[AP_OBJ::BTN_ESTOP] Init Failed!");
  }

  {
    enOp::cfg_t cfg;
    cfg.ptr_led_status            = &leds[AP_OBJ::LED_TOWER_GREEN];
    cfg.ptr_led_op_g              = &leds[AP_OBJ::LED_OP_G_START];
    cfg.ptr_led_op_y              = &leds[AP_OBJ::LED_OP_Y_RESET];
    cfg.ptr_led_op_r              = &leds[AP_OBJ::LED_OP_R_STOP];
    cfg.ptr_btn_start             = &btns[AP_OBJ::BTN_OP_START];
    cfg.ptr_btn_stop              = &btns[AP_OBJ::BTN_OP_STOP];
    cfg.ptr_btn_reset             = &btns[AP_OBJ::BTN_OP_RESET];
    cfg.ptr_btn_estop             = &btns[AP_OBJ::BTN_OP_ESTOP];
    cfg.tower_buzzer_gpio_port    = IO_TOWER_BUZZER_GPIO_Port;
    cfg.tower_buzzer_gpio_pin     = IO_TOWER_BUZZER_Pin;
    cfg.sensor_safety_1_gpio_port = IO_IN_SAFETY1_GPIO_Port;
    cfg.sensor_safety_1_gpio_pin  = IO_IN_SAFETY1_Pin;
    cfg.sensor_safety_2_gpio_port = IO_IN_SAFETY2_GPIO_Port;
    cfg.sensor_safety_2_gpio_pin  = IO_IN_SAFETY2_Pin;
    cfg.ptr_mcu_io                = &mcu_io;
    cfg.ptr_mcu_reg               = &mcu_reg;
    if (op_panel.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("op_panel Init Failed!");
  }
}

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
  }

  uint32_t pre_time;
  pre_time = millis();

  LOG_PRINT("Start App Loop");
  /* Infinite loop */

  uint8_t test_addr         = 0;
  uint8_t test_campare_data = 0;
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();


      {
        uint8_t set_bit = 0x01;
        set_bit         = 1 << test_addr;
        mcu_io.SetOutputReg(set_bit);
        ++test_addr;
        test_addr = test_addr % 8;
        if (std::memcmp(&test_campare_data, &mcu_io.m_in[0], 1) != 0)
        {
          LOG_PRINT("read in : 0x%02X", mcu_io.m_in[0]);
        }

        test_campare_data = mcu_io.m_in[0];
      }
    }

    /* 1. io register */
    mcu_io.Update_io();

    eventOpPanel();
  }
}

/*
 * operating button event
 */
void eventOpPanel()
{
  constexpr enOp::status_e system_init  = enOp::status_e::INIT;
  constexpr enOp::status_e auto_stop    = enOp::status_e::STEP_STOP;
  constexpr enOp::status_e auto_ready   = enOp::status_e::RUN_READY;
  constexpr enOp::status_e auto_working = enOp::status_e::RUN;
  constexpr enOp::status_e error_stop   = enOp::status_e::ERR_STOP;

  // const enOp::lamp_e g_ready = enOp::LAMP_READY;
  // const enOp::lamp_e y_busy = enOp::LAMP_BUSY;
  // const enOp::lamp_e r_error = enOp::LAMP_ERROR;

  switch (op_panel.GetStatus())
  {
  case system_init:
  {
    if (btns[AP_OBJ::BTN_OP_START].IsLongKey())
    {
      // LOG_PRINT("start btn Pressed");
    }
    if (btns[AP_OBJ::BTN_OP_STOP].IsShortKey())
    {
    }
  }
  break;

  case auto_stop:
  {
    if (btns[AP_OBJ::BTN_OP_START].IsLongKey())
    {
    }
    else if (btns[AP_OBJ::BTN_OP_RESET].IsLongKey())
    {
    }
    else if (btns[AP_OBJ::BTN_OP_STOP].IsShortKey())
    {
    }
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

      StatusLeDToggle;
    }
  }
  // end of status led

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
      op_panel.LampToggle(enOp::LAMP_ALL);
      // op_panel.LampToggle(enOp::LAMP_START);
      // op_panel.LampToggle(enOp::LAMP_STOP);
      // op_panel.LampToggle(enOp::LAMP_RESET);
    }
  }
  break;

  case auto_stop:
  {
    op_panel.LampOnOff(enOp::LAMP_STOP);

    op_panel.LampOnOff(enOp::LAMP_START, false);
    op_panel.LampOnOff(enOp::LAMP_RESET, false);
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
 * mcu register update
 */
void updateApReg()
{
}

void apISR_1ms(void *arg)
{

  updateApReg();

  wheels.periodElapsedCallback();

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
  {
    static uint16_t pre_cnt = 0;
    if (wheels.m_timer_counter != pre_cnt)
    {
      pre_cnt = wheels.m_timer_counter;
      wheels.wheelCntView();
    }
  }
  updateLamp();
  for (auto &elm : btns)
    elm.ISR();
}

void apISR_50ms(void *arg)
{
}

/////////////////////////////////////////////////////////////////////////////////////////

// errno_t SetOutputRegist(uint32_t reg)
// {
//   // if (api_extio.m_IsInit)
//   //   return api_extio.RequestCmd(MSG::CMD_TYPE::CMD_CTRL_IO_OUT, reg);
//   return -1;
// }

/**
 * @brief
 *    timeout error processing of wait functions such as cyl, vac, etc.
 * @param obj
 * @param w_parm
 * @param l_parm
 */
// void errorCBFunc(void *obj, void *w_parm, void *l_parm)
// {
//   // TODO:: cyl, vac 등 wait 함수의 타임아웃 에러 처리

//   log_dat *p_data = (log_dat *)l_parm;
//   LOG_PRINT("error_call obj[0x%04X] [%s]", p_data->log_buff.log.data());
// }
/////////////////////////////////////////////////////////////////////////////////////////


#endif

#if 0
static void apISR_1ms(void *arg);
static void apISR_10ms(void *arg);


#define StatusLeDToggle HAL_GPIO_TogglePin(STATUS_GPIO_Port, STATUS_Pin)

/* TIM3 정보 */
#define TIM3_PULSE_PER_REVOLUTION 100 // 1회전 당 펄스 발생 수
#define TIM3_CLK_FREQ 64000000        // TIM3 클럭 주파수
#define SPEED_CALC_INTERVAL 1000      // 속도 계산 주기 (us)
#define SPEED_UNIT_CONVERSION 60      // RPM으로 변환하기 위한 상수

extern TIM_HandleTypeDef htim3;

/* TIM3 캡처 정보 */
volatile uint32_t TIM3_capture_count = 0;      // TIM3 입력 캡처 카운트
volatile uint32_t TIM3_capture_value = 0;      // TIM3 입력 캡처 값
volatile uint32_t TIM3_prev_capture_value = 0; // 이전 TIM3 입력 캡처 값
volatile uint32_t TIM3_pulse_count = 0;        // TIM3 펄스 발생 수
volatile uint32_t TIM3_direction = 0;          // TIM3 방향
volatile uint32_t TIM3_speed = 0;              // TIM3 속도
volatile uint32_t wheel_cnt = 0;

int16_t encoder_velocity;
int32_t encoder_position;
uint16_t timer_counter;

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

  // HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  // HAL_TIM_Base_Start_IT(&htim3);

}

void apMain(void)
{
  LOG_PRINT("[OK] App Wheel Encoder Test");
  uint32_t pre_time;
  uint16_t pre_value = 0;
  pre_time = millis();
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();
      if (pre_value != timer_counter)
      {
        pre_value = timer_counter;
        LOG_PRINT("timer_counter : %d", timer_counter);
      }
    }

  }
}

void updateLED()
{
  /**
   * @brief
   * status led
   */
  {
    static uint32_t pre_time_update_led;

    if (millis() - pre_time_update_led >= 500)
    {
      pre_time_update_led = millis();
      StatusLeDToggle;
    }
  }
  // end of status led
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) // TIM3에서 인터럽트가 발생한 경우
  {
    timer_counter = __HAL_TIM_GET_COUNTER(htim);
  }
  //HAL_TIM_Base_Start_IT(&htim3);
  //HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 |TIM_CHANNEL_2);
}

/* TIM3 입력 캡처 인터럽트 핸들러 함수 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3) // TIM3에서 인터럽트가 발생한 경우
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // TIM3 채널 1에서 인터럽트가 발생한 경우
    {
      TIM3_capture_value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1); // TIM3 채널 1의 캡처 값 읽기
      if (TIM3_capture_value > TIM3_prev_capture_value)                      // TIM3 방향이 정방향인 경우
      {
        TIM3_direction = 1;
        wheel_cnt++;
      }
      else // TIM3 방향이 역방향인 경우
      {
        TIM3_direction = -1;
        if (wheel_cnt > 1)
          wheel_cnt--;
      }

      TIM3_pulse_count++;                           // TIM3 펄스 발생 수 증가
      TIM3_prev_capture_value = TIM3_capture_value; // 이전 TIM3 입력 캡처 값 저장

      // print out
      LOG_PRINT("TIM3 Channel 1 Captured Value: %d, Direction: %d",  TIM3_capture_value, TIM3_direction);
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // TIM3 채널 2에서 인터럽트가 발생한 경우
    {
      TIM3_capture_value = HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_2); // TIM3 채널 2의 캡처 값 읽기

      uint32_t time_diff = TIM3_capture_value - TIM3_prev_capture_value;                                           // TIM3 펄스 간 시간 차이 계산
      TIM3_speed = (uint32_t)((float)TIM3_PULSE_PER_REVOLUTION / (float)time_diff * (float)TIM3_CLK_FREQ / 60.0f); // TIM3 속도 계산 (rpm)

      TIM3_prev_capture_value = TIM3_capture_value; // 이전 TIM3 입력 캡처 값 저장

      // print out
       LOG_PRINT("TIM3 Channel 2 Captured Value: %d, Direction: %d",  TIM3_capture_value, TIM3_speed);
    }
  }
}

void apISR_1ms(void *arg)
{
  timer_counter = __HAL_TIM_GET_COUNTER(&htim3);
}

void apISR_10ms(void *arg)
{
  updateLED();
  //
}

#endif
