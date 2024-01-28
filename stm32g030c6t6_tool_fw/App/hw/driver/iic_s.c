/*
 * iic_s.c
 *
 *  Created on: 2023. 10. 18.
 *      Author: gns2.lee
 * 
 * 

// https://youtu.be/X7WZQka2FHQ

 */

#include "iic_s.h"

#ifdef _USE_HW_IIC_S
#include "cli.h"

#ifdef _USE_HW_CLI
static void cliIIC_S(cli_args_t *args);
#endif

//#define TEST_LOG(fmt, ...)   LOG_PRINT(fmt, ##__VA_ARGS__)
#define TEST_LOG(fmt, ...)


//address 0x3e
#define SLAVE_ADDRESS  0x1f << 1

typedef struct
{
  I2C_HandleTypeDef *p_hi2c;
  DMA_HandleTypeDef *p_hdma_rx;
  DMA_HandleTypeDef *p_hdma_tx;

  GPIO_TypeDef *scl_port;
  int scl_pin;

  GPIO_TypeDef *sda_port;
  int sda_pin;
} iics_tbl_t;

static iics_tbl_t iics_tbl[IIC_S_MAX_CH];

static uint32_t iics_timeout[IIC_S_MAX_CH];
static uint32_t iics_errcount[IIC_S_MAX_CH];

static bool is_init = false;
static bool is_begin[IIC_S_MAX_CH];
static uint32_t iics_freq[] = {
        0x10707DBC, // 100Khz standard mode
        0x00602173, // 400Khz fast mode
        0x00300B29, // 1000Khz fast plus mode
    };

// static void delay_us(uint32_t us);

extern I2C_HandleTypeDef hi2c1;

#ifdef _USE_HW_IIC_S_1_DMA_RX
extern DMA_HandleTypeDef hdma_i2c1_rx;
#endif
#ifdef _USE_HW_IIC_S_1_DMA_TX
extern DMA_HandleTypeDef hdma_i2c1_tx;
#endif


bool iicsInit(void)
{
  uint32_t i;

  for (i = 0; i < IIC_S_MAX_CH; i++)
  {
    iics_timeout[i] = 10;
    iics_errcount[i] = 0;
    is_begin[i] = false;
    iics_tbl[i].p_hdma_rx = NULL;
    iics_tbl[i].p_hdma_tx = NULL;
    iics_tbl[i].p_hi2c = NULL;
    iics_tbl[i].scl_port = NULL;
    iics_tbl[i].scl_pin = 0;
    iics_tbl[i].sda_port = NULL;
    iics_tbl[i].sda_pin = 0;
  }

  iics_tbl[0].p_hi2c = &hi2c1;
  iics_tbl[0].scl_port = GPIOB;
  iics_tbl[0].scl_pin = GPIO_PIN_6;
  iics_tbl[0].sda_port = GPIOB;
  iics_tbl[0].sda_pin = GPIO_PIN_7;

#ifdef _USE_HW_CLI
  cliAdd("iics", cliIIC_S);
#endif

  is_init = true;
  return true;
}

bool iicsIsInit(void)
{
  return is_init;
}
bool iicsBegin(uint8_t ch, iics_freq_t freq_khz)
{
  bool ret = false;

  I2C_HandleTypeDef *p_handle = iics_tbl[ch].p_hi2c;

  if (ch >= IIC_S_MAX_CH)
  {
    return false;
  }

  switch (ch)
  {
  case _DEF_IIC_S1:
  {
    is_begin[ch] = false;

    p_handle->Instance = I2C1;
    p_handle->Init.Timing = iics_freq[freq_khz];
    p_handle->Init.OwnAddress1 = SLAVE_ADDRESS; //0x1f << 1;
    p_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    p_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    p_handle->Init.OwnAddress2 = 0x00;
    p_handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    p_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    p_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
#ifdef _USE_HW_IIC_S_1_DMA_RX
    iics_tbl[ch].p_hdma_rx = &hdma_i2c1_rx;
#endif
    iicsReset(ch);

    HAL_I2C_DeInit(p_handle);
    if (HAL_I2C_Init(p_handle) == HAL_OK)
    {
      /* Enable the Analog I2C Filter */
      HAL_I2CEx_ConfigAnalogFilter(p_handle, I2C_ANALOGFILTER_ENABLE);

      /* Configure Digital filter */
      HAL_I2CEx_ConfigDigitalFilter(p_handle, 0);

      HAL_I2C_EnableListen_IT(p_handle);
      ret = true;
      is_begin[ch] = true;
    }
  }
  break;

  default:
    break;
  }
  return ret;
}
bool iicsIsBegin(uint8_t ch)
{
  return is_begin[ch];
}
void iicsReset(uint8_t ch)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  iics_tbl_t *p_pin = &iics_tbl[ch];

  GPIO_InitStruct.Pin = p_pin->scl_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(p_pin->scl_port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = p_pin->sda_pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(p_pin->sda_port, &GPIO_InitStruct);

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
  delay_us(5);

  for (int i = 0; i < 9; i++)
  {

    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
    delay_us(5);
    HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
    delay_us(5);
  }

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_RESET);
  delay_us(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_RESET);
  delay_us(5);

  HAL_GPIO_WritePin(p_pin->scl_port, p_pin->scl_pin, GPIO_PIN_SET);
  delay_us(5);
  HAL_GPIO_WritePin(p_pin->sda_port, p_pin->sda_pin, GPIO_PIN_SET);
}
bool iicsRecovery(uint8_t ch)
{
  iicsReset(ch);
  return iicsBegin(ch, iics_freq[ch]);
}

bool iicsIsReady(uint8_t ch)
{
  return (iics_tbl[ch].p_hi2c->State == HAL_I2C_STATE_READY);
}


void iicsSetTimeout(uint8_t ch, uint32_t timeout)
{
  iics_timeout[ch] = timeout;
}
uint32_t iicsGetTimeout(uint8_t ch)
{
  return iics_timeout[ch];
}

void iicsClearErrCount(uint8_t ch)
{
  iics_errcount[ch] = 0;
}

uint32_t iicsGetErrCount(uint8_t ch)
{
  return iics_errcount[ch];
}

uint32_t iicsSlaveAvailable(uint8_t ch)
{
  return 0;
}
uint8_t iicsSlaveRead(uint8_t ch)
{
  return 0;
}

/**
 * @brief 마스터에서 Slave의 데이터를 read 
 * 
 * @param addr 
 * @param p_data 
 * @return __weak 
 */
__weak void i2cReadCallback(uint8_t addr, uint8_t *p_data)
{
}


/**
 * @brief 마스터보드에서 Slave로 보낸 데이터 (첫 번째 커맨드 바이트 제외한)  
 * 
 * @param addr 커맨드 구분
 * @param p_data 
 * @return __weak 
 */
__weak void i2cWriteCallback(uint8_t addr, uint8_t *p_data)
{
}

typedef enum
{
  IICS_CMD_IDLE,
  IICS_CMD_GET_ADDR,
  IICS_CMD_WR_DATA,
  IICS_CMD_RD_DATA,
} iic_cmd_t;


volatile iic_cmd_t iic_cmd_state = IICS_CMD_IDLE;
volatile bool iic_cmd_done = false;

static uint8_t iic_rx_buf[2];
static uint8_t iic_tx_buf[2];

uint8_t iic_cmd_addr = 0;

#ifdef IICS_SEQ_DEBUG

typedef enum
{
  IIC_SEQ_0,
  IIC_SEQ_1,
  IIC_SEQ_2,
  IIC_SEQ_3,
  IIC_SEQ_4,
  IIC_SEQ_5,
  IIC_SEQ_6,
  IIC_SEQ_7,
  IIC_SEQ_8,
  IIC_SEQ_9,
} iics_seq_t;

uint8_t iic_log_idx = 0;
uint8_t iic_log_state[40] = {0,};
#endif

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  iic_cmd_done = true;
#ifdef IICS_SEQ_DEBUG
  iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_0;
  iic_log_state[(iic_log_idx++) % 40] = iic_cmd_addr;
#endif
  HAL_I2C_EnableListen_IT(hi2c);
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
 switch (TransferDirection)
  {
  case I2C_DIRECTION_TRANSMIT: // if the master wants to transmit the data
    iic_cmd_state = IICS_CMD_GET_ADDR;
#ifdef _USE_HW_IIC_S_1_DMA_TX
    HAL_I2C_Slave_Seq_Receive_DMA(hi2c, iic_rx_buf, 1, I2C_FIRST_FRAME);
#else
    HAL_I2C_Slave_Seq_Receive_IT(hi2c, iic_rx_buf, 1, I2C_FIRST_FRAME);
#endif
    iic_cmd_done = false;
#ifdef IICS_SEQ_DEBUG
    iic_log_idx = 0;
    memset(iic_log_state, 0, sizeof(iic_log_state));
    iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_1;
    iic_log_state[(iic_log_idx++) % 40] = iic_rx_buf[0];
#endif
    break;

  case I2C_DIRECTION_RECEIVE:
  default:
    iic_cmd_state = IICS_CMD_RD_DATA;
    i2cReadCallback(iic_cmd_addr, iic_tx_buf);
#ifdef IICS_SEQ_DEBUG
    iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_2;
    iic_log_state[(iic_log_idx++) % 40] = iic_tx_buf[0];
#endif
    iic_cmd_addr++;     
#ifdef _USE_HW_IIC_S_1_DMA_TX
    HAL_I2C_Slave_Seq_Transmit_DMA(hi2c, iic_tx_buf, 1, I2C_NEXT_FRAME); // I2C_FIRST_FRAME
#else
    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, iic_tx_buf, 1, I2C_NEXT_FRAME);
#endif
    break;
  }
}



void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{  
  switch (iic_cmd_state)
  {
  case IICS_CMD_GET_ADDR:
    iic_cmd_addr = iic_rx_buf[0];
    iic_cmd_state = IICS_CMD_WR_DATA;
#ifdef IICS_SEQ_DEBUG
    iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_3;
    iic_log_state[(iic_log_idx++) % 40] = iic_cmd_addr;
#endif
    HAL_I2C_Slave_Seq_Receive_DMA(hi2c, iic_rx_buf, 1, I2C_FIRST_FRAME);
    break;

  case IICS_CMD_WR_DATA:
    i2cWriteCallback(iic_cmd_addr, iic_rx_buf);
#ifdef IICS_SEQ_DEBUG
    iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_4;
    iic_log_state[(iic_log_idx++) % 40] = iic_rx_buf[0];
#endif
    iic_cmd_addr++;
    HAL_I2C_Slave_Seq_Receive_DMA(hi2c, iic_rx_buf, 1, I2C_LAST_FRAME);
    break;

  default:
    break;
  }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
 if (iic_cmd_done == true)
    return;

  switch (iic_cmd_state)
  {
  case IICS_CMD_RD_DATA:
    i2cReadCallback(iic_cmd_addr, iic_tx_buf);
#ifdef IICS_SEQ_DEBUG
    iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_5;
    iic_log_state[(iic_log_idx++) % 40] = iic_tx_buf[0];
#endif
    iic_cmd_addr++;
#ifdef _USE_HW_IIC_S_1_DMA_TX
  HAL_I2C_Slave_Seq_Transmit_DMA(hi2c, iic_tx_buf, 1, I2C_NEXT_FRAME);
#else
  HAL_I2C_Slave_Seq_Transmit_IT(hi2c, iic_tx_buf, 1, I2C_NEXT_FRAME);
#endif
    break;

  default:
    break;
  }
}



void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  uint32_t errorcode = HAL_I2C_GetError(hi2c);
#ifdef IICS_SEQ_DEBUG
  iic_log_state[(iic_log_idx++) % 40] = (uint8_t)IIC_SEQ_6;
  iic_log_state[(iic_log_idx++) % 40] = errorcode;
#endif

  if (errorcode == HAL_I2C_ERROR_NONE)
    return;

  if (errorcode == HAL_I2C_ERROR_AF) // AF error
  {
   
  }

  /* BERR Error commonly occurs during the Direction switch
   * Here we the software reset bit is set by the HAL error handler
   * Before resetting this bit, we make sure the I2C lines are released and the bus is free
   * I am simply reinitializing the I2C to do so
   */
  else if (errorcode == HAL_I2C_ERROR_BERR) // BERR Error
  {
    HAL_I2C_DeInit(hi2c);
    HAL_I2C_Init(hi2c);
    //memset(RxData, '\0', RxSIZE); // reset the Rx buffer
    //rxcount = 0;                  // reset the count
  }

  HAL_I2C_EnableListen_IT(hi2c);
}


void HAL_I2C_AbortCpltCallback(I2C_HandleTypeDef *hi2c)
{
  return;
}

#ifdef _USE_HW_CLI

void cliIIC_S(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "cmd") == true)
    {
      cliPrintf("command addr [0x%02X] state [0x%02X] \n", iic_cmd_addr, iic_cmd_state);

      ret = true;
    }
    else if (args->isStr(0, "log") == true)
    {

      for (int i = 0; i < 40; i = i + 2)
      {
        cliPrintf("log %02d - [0x%02X] : [0x%02X] \n", i, iic_log_state[i], iic_log_state[i + 1]);
      }

      ret = true;
    }
  }
  else if (args->argc == 2)
  {
    if (args->isStr(0, "open") == true)
    {
      uint8_t ch = (uint16_t)args->getData(1);
      bool iic_ret = iicsBegin(ch, iics_freq[ch]);
      if (iic_ret == true)
      {
        cliPrintf("IIC_S CH%d Open OK\n", ch + 1);
      }
      else
      {
        cliPrintf("IIC_S CH%d Open Fail\n", ch + 1);
      }
    }
  }

  if (ret == false)
  {
    cliPrintf("iics log \n");
    cliPrintf("iics test \n");
    cliPrintf("iics open ch[1~%d]\n", IIC_S_MAX_CH);
  }
}
#endif
#endif
// _USE_HW_IIC_S
