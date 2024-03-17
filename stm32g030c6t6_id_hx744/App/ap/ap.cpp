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
ap_reg mcu_reg;
mcu_io_align_12_t mcu_data;
mcu_io_align_12_t for_compare_data;

/****************************************************
  1. ap instances
 ****************************************************/
enLed leds[AP_OBJ::LED_MAX];

#ifndef _USE_EXHW_IICOMM_DEV
dev_io io_comm;
#endif

enLoadCell loadcell;

void apInit(void)
{

#ifdef _USE_EXHW_IICOMM_DEV

  
  {
    iic_cmd::cfg_t cfg{};
    cfg.ReadReg = iicomm_devReadReg;
    cfg.WriteReg = iicomm_devWriteReg;
    cmd_comm.Init(cfg);
  }
#else
  {
    using namespace CMD;
    uart_cmd::cfg_t cfg{};
    cfg.ch = HW_UART_CH_MAIN;
    cfg.baud = 115200;
    cmd_comm.Init(cfg);
  }
#endif

#ifdef _USE_HW_CLI

#endif

  {
    enLed::cfg_t cfg;
    cfg.gpio_port = STATUS_GPIO_Port;
    cfg.gpio_pin  = STATUS_Pin;
    if (leds[AP_OBJ::LED_STATUS].Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("status_led Init Failed!");
  }

{
  enLoadCell::cfg_t cfg{};
  cfg.loadcell_ch = 0;
  cfg.hx711_IsInit = hx711_IsInit;
  cfg.hx711_IsAvailable = hx711_IsAvailable;
  cfg.hx711_SetGain = hx711_SetGain;
  cfg.hx711_SetCoefficient = hx711_SetCoefficient;
  cfg.hx711_GetData = hx711_GetData;
  cfg.hx11_ReadWeight = hx11_ReadWeight;
  cfg.hx711_PowerOn = hx711_PowerOn;
  cfg.hx711_PowerDown = hx711_PowerDown;
  std::strcpy(cfg.name_str, "loadcell");

  if (loadcell.Init(cfg) != ERROR_SUCCESS)
    LOG_PRINT("loadcell Init Failed!");
}
#ifndef _USE_EXHW_IICOMM_DEV
  {
    dev_io::cfg_t cfg{};
    cfg.ptr_comm = &cmd_comm;
    cfg.ptr_io = &mcu_io;
    cfg.ptr_mcu_data = &mcu_data;
    cfg.ptr_mcu_reg = &mcu_reg;

    if (io_comm.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("io_comm Init Failed!");
  }
  /* ap io */
  {
    ap_io::cfg_t cfg;
    cfg.ex_io_i2c_ch = _DEF_I2C1;
    cfg.io_dev_addr = HW_SLAVE_ADDRESS;
    cfg.i2cBegin = i2cBegin;
    cfg.i2cReadBytes = i2cReadBytes;
    cfg.i2cWriteBytes = i2cWriteBytes;
    cfg.i2cRecovery = i2cRecovery;

    if (mcu_io.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("mcu_io Init Failed!");
  }

#else
  /* ap io */
  {
    ap_io::cfg_t cfg;
    cfg.ex_io_i2c_ch = 0;

    if (mcu_io.Init(cfg) != ERROR_SUCCESS)
      LOG_PRINT("mcu_io Init Failed!");
  }

#endif

#ifdef _USE_HW_CLI
  cliAdd("app", cliApp);
#endif
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
  }
  // end of swtimer

  uint32_t pre_time;
  // uint32_t out_cnt = 0;
  pre_time = millis();



  //HAL_GPIO_WritePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin,GPIO_PIN_RESET);
  LOG_PRINT("Start App Loop");
  /* Infinite loop */
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();

      //HAL_GPIO_TogglePin(HX711_SCK_GPIO_Port,HX711_SCK_Pin);

      //LOG_PRINT("ADC Value : %d", adcReadVoltage(_DEF_ADC1));
    }


    loadcell.GetDataStep();

#ifndef _USE_EXHW_IICOMM_DEV
    /* 1. io cmd*/
    io_comm.ThreadJob();
#endif

#ifdef _USE_HW_CLI
    cliMain();
#endif
  }
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
}

void updateErr()
{
}
#ifndef _USE_EXHW_IICOMM_DEV
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
    data_1d,
    data_max
  };
  mcu_data.io_in[io_1d] = MaKEDWORD(mcu_io.m_in[ap_io::bank_main_1616], mcu_io.m_in[ap_io::bank_ext_1616]);
  mcu_data.io_out[io_1d] = MaKEDWORD(mcu_io.m_out[ap_io::bank_main_1616], mcu_io.m_out[ap_io::bank_ext_1616]);
  mcu_data.datas[data_1d] = 0;
}



void SendDataUart()
{
  if (std::memcmp((uint8_t *)&mcu_data, (uint8_t *)&for_compare_data, sizeof(mcu_data)) != 0)
  {
    std::memcpy((uint8_t *)&for_compare_data, (uint8_t *)&mcu_data, sizeof(mcu_data));    
    if (io_comm.SendData(CMD::TX_TOOL_DATA) != ERROR_SUCCESS)
      LOG_PRINT("io_comm.SetMcuData Error");
  }

}

#endif

#ifdef _USE_EXHW_IICOMM_DEV

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
    data_1d,
    data_max
  };
  mcu_data.io_in[io_1d] = MaKEDWORD(mcu_io.m_in, 0);
  mcu_data.io_out[io_1d] = MaKEDWORD(mcu_io.m_out, 0);
  mcu_data.datas[data_1d] = 0;
}

void SendPeriodDataIIC()
{
  if (std::memcmp((uint8_t *)&mcu_data, (uint8_t *)&for_compare_data, sizeof(mcu_data)) != 0)
  {
    std::memcpy((uint8_t *)&for_compare_data, (uint8_t *)&mcu_data, sizeof(mcu_data));
    if (cmd_comm.SetMcuData(&mcu_data) != ERROR_SUCCESS)
      LOG_PRINT("cmd_comm.SetMcuData Error");
  }
}

void processIIC()
{
  switch (cmd_comm.Process())
  {
  case iic_cmd::CMD_ORD_SET_OUT_REG:
  {
    if (cmd_comm.GetOutReg() != ERROR_SUCCESS)
    {
      LOG_PRINT("cmd_comm.GetOrderData Error");
    }
    else
    {
      mcu_io.SetOutputReg(LoWORD(cmd_comm.outRegs[0]));
    }

    goto reset_reg;
  }
  break;
    /*
    if (cmd_comm.GetOrderData() != ERROR_SUCCESS)
         {
           LOG_PRINT("cmd_comm.GetOrderData Error");
           return CMD_ERR_RET;
         }

    */
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

  updateApReg();

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
#else
  SendDataUart();

  //  static int32_t loadcel_data;
  //  constexpr uint8_t ch = 0;
  //  loadcel_data = hx711_Read(ch)/100;
  // //static uint16_t run_data;
  // if (loadcel_data > 0)
  //   uartPrintf(_DEF_UART1, ">data:%d\n", loadcel_data);


  if (loadcell.m_que.Available())
  {
    int32_t data = 0;
    loadcell.m_que.Get(&data);
    
    if (data != 0)
    {
      uartPrintf(_DEF_UART1, ">loadcell:%d\n", data);
    }
  }

#endif
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
