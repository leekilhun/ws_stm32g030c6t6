/**
 * @file uart.cpp
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "uart.h"

#ifdef _USE_HW_UART

#include "qbuffer.h"
#include "cli.h"

  /****************************************************
   *  macro and define
   ****************************************************/
#define UART_BUF_LENGTH 256
#define UART_MAX_CH HW_UART_MAX_CH

#define USE_DEF_UART1_DMA_TX
// #define USE_DEF_UART2_DMA_TX
// #define USE_DEF_UART3_DMA_TX
// #define USE_DEF_UART4_DMA_TX

#define USE_DEF_UART1_DMA_RX
// #define USE_DEF_UART2_DMA_RX
// #define USE_DEF_UART3_DMA_RX
// #define USE_DEF_UART4_DMA_RX

  /****************************************************
   *  typedef
   ****************************************************/
typedef struct
{
  UART_HandleTypeDef* ptr_handle;
  DMA_HandleTypeDef* ptr_hdma_rx;
  DMA_HandleTypeDef* ptr_hdma_tx;
  bool is_open;
  uint32_t baud;
  uint8_t rx_buf[UART_BUF_LENGTH];
  uint8_t rx_data;
  uint8_t *ptr_tx_buf;
} uart_tbl_t;

  /****************************************************
   *  data
   ****************************************************/  
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

static qbuffer_t qbuffer[UART_MAX_CH];
static uart_tbl_t uart_tbl[UART_MAX_CH];


#ifdef USE_DEF_UART1_DMA_TX
static uint8_t uart1_tx_data[UART_BUF_LENGTH];
#endif
#ifdef USE_DEF_UART2_DMA_TX
static uint8_t def_uart1_tx_data[UART_BUF_LENGTH];
#endif


#if CLI_USE(HW_UART)
static void cliUart(cli_args_t *args);
#endif


  /****************************************************
   *  function
   ****************************************************/
bool uartInit(void)
{

 const uint32_t default_baudrate = 115200;
  /* init ch 1*/
  uint8_t idx = _DEF_UART1;
  uart_tbl[idx].is_open = false;
  uart_tbl[idx].baud    = default_baudrate;
  uart_tbl[idx].rx_data = 0;
  memset(&uart_tbl[idx].rx_buf[0], 0, UART_BUF_LENGTH);
  uart_tbl[idx].ptr_handle = &huart1;
  uart_tbl[idx].ptr_hdma_rx = NULL;
  uart_tbl[idx].ptr_hdma_tx = &hdma_usart1_tx;
  uart_tbl[idx].ptr_tx_buf = uart1_tx_data;

    /* init ch 2*/
  idx = _DEF_UART2;
  uart_tbl[idx].is_open = false;
  uart_tbl[idx].baud    = default_baudrate;
  uart_tbl[idx].rx_data = 0;
  memset(&uart_tbl[idx].rx_buf[0], 0, UART_BUF_LENGTH);
  uart_tbl[idx].ptr_handle = &huart2;
  uart_tbl[idx].ptr_hdma_rx = NULL;
  uart_tbl[idx].ptr_hdma_tx = NULL;
  uart_tbl[idx].ptr_tx_buf = NULL;

  for (int i = 0; i < UART_MAX_CH; i++)
  {
#ifdef _USE_HW_RTOS
    uart_mutex_lock[i] = osMutexCreate(&uart_mutex_lock_def[i]);
#endif
  }

#if CLI_USE(HW_UART)
  cliAdd("uart", cliUart);
#endif
  return true;
}

bool uartDeInit(void)
{
  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  UART_HandleTypeDef* ptr_uart = uart_tbl[ch].ptr_handle;

  if (ch >= UART_MAX_CH)
    return false;

  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
  {
    if (ptr_uart == NULL)
    {
      LOG_PRINT(" ptr_uart is nullptr ch[%d]", ch);
      return false;
    }
    ptr_uart->Init.BaudRate = baud;
    HAL_UART_DeInit(ptr_uart);
    if (HAL_UART_Init(ptr_uart) != HAL_OK)
      return false;
    else
    {
      ret                  = true;
      uart_tbl[ch].is_open = true;
    }
    ret                  = true;
    uart_tbl[ch].is_open = true;
    qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);

    if (uart_tbl[ch].ptr_hdma_rx != NULL)
    {
      if (HAL_UART_Receive_DMA(ptr_uart, (uint8_t*)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
      {
        LOG_PRINT("HAL_UART_Receive_DMA failed! ch[%d]", ch);
        ret = false;
      }
      else
      {
        LOG_PRINT("HAL_UART_Receive_DMA success ! ch[%d]", ch);
        qbuffer[ch].in  = qbuffer[ch].len - uart_tbl[ch].ptr_hdma_rx->Instance->CNDTR;
        qbuffer[ch].out = qbuffer[ch].in;
      }
    }
    else
    {
      if (HAL_UART_Receive_IT(ptr_uart, (uint8_t*)&uart_tbl[ch].rx_data, 1) != HAL_OK)
        ret = false;
    }
  }
  break;

  default:
    break;
  }

  return ret;
}

bool uartIsOpen(uint8_t ch)
{
  bool ret = false;

  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
  case _DEF_UART3:
    ret = uart_tbl[ch].is_open;
    break;
  default:
    break;
  }
  return ret;
}

bool uartClose(uint8_t ch)
{
  uart_tbl[ch].is_open = false;
  return true;
}

uint32_t uartAvailable(uint8_t ch)
{
  uint32_t ret = 0;

  if (uart_tbl[ch].ptr_hdma_rx != NULL)
    qbuffer[ch].in = (qbuffer[ch].len - uart_tbl[ch].ptr_hdma_rx->Instance->CNDTR);

  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
    ret = qbufferAvailable(&qbuffer[ch]);
    break;
  default:
    break;
  }
  return ret;
}

bool uartFlush(uint8_t ch)
{
  uint32_t pre_time;

  pre_time = millis();
  while (uartAvailable(ch))
  {
    if (millis() - pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }

  return true;
}

uint8_t uartRead(uint8_t ch)
{
  uint8_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
    qbufferRead(&qbuffer[ch], &ret, 1);
    break;
  default:
    break;
  }

  return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
  {
    if (uart_tbl[ch].ptr_hdma_tx != NULL)
    {
      uint32_t pre_time;
      pre_time = millis();
      while (uart_tbl[ch].ptr_handle->gState != HAL_UART_STATE_READY)
      {
        if (millis() - pre_time >= 100)
        {
          break;
        }
      }
      if (uart_tbl[ch].ptr_handle->gState == HAL_UART_STATE_READY)
      {
        memset(uart_tbl[ch].ptr_tx_buf, 0, UART_BUF_LENGTH);
        memcpy(uart_tbl[ch].ptr_tx_buf, p_data, length);
        if(HAL_UART_Transmit_DMA(uart_tbl[ch].ptr_handle, uart_tbl[ch].ptr_tx_buf, length)  != HAL_OK)
        {
          LOG_PRINT("Transmit_DMA failed");
          return 0;
        }
        return length;
      }
    }
    else
    {
      if (HAL_UART_Transmit(uart_tbl[ch].ptr_handle, p_data, length, 100) == HAL_OK)
        return length;
    }
  }
  break;
  default:
    break;
  }

  return 0;
}

uint32_t uartVPrintf(uint8_t ch, const char *fmt, va_list arg)
{
  uint32_t ret = 0;
  char print_buf[256];

  int len;
  len = vsnprintf(print_buf, 256, fmt, arg);

  if (len > 0)
  {
    ret = uartWrite(ch, (uint8_t *)print_buf, len);
  }

  return ret;
}

uint32_t uartPrintf(uint8_t ch, const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);
  ret = uartWrite(ch, (uint8_t *)buf, len);
  va_end(args);

  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
    return uart_tbl[ch].baud;
  default:
    break;
  }

  return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {
#ifdef USE_DEF_UART1_DMA_RX
#else
    qbufferWrite(&qbuffer[_DEF_UART1], &uart_tbl[_DEF_UART1].rx_data, 1);
    HAL_UART_Receive_IT(uart_tbl[_DEF_UART1].ptr_handle, (uint8_t*)&uart_tbl[_DEF_UART1].rx_data, 1);
#endif
  }
  else if (huart->Instance == USART2)
  {
#ifdef USE_DEF_UART2_DMA_RX
#else
    qbufferWrite(&qbuffer[_DEF_UART2], &uart_tbl[_DEF_UART2].rx_data, 1);
    HAL_UART_Receive_IT(uart_tbl[_DEF_UART2].ptr_handle, (uint8_t*)&uart_tbl[_DEF_UART2].rx_data, 1);
#endif
  }
}


  /****************************************************
   *  test code
   ****************************************************/
#if CLI_USE(HW_UART)
void cliUart(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i = 0; i < UART_MAX_CH; i++)
    {
      cliPrintf("_DEF_UART%d :%d bps\n", i + 1, uartGetBaud(i));
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "test"))
  {
    uint8_t uart_ch;

    uart_ch = constrain(args->getData(1), 1, UART_MAX_CH) - 1;

    if (uart_ch != cliGetPort())
    {
      uint8_t rx_data;

      while (1)
      {
        if (uartAvailable(uart_ch) > 0)
        {
          rx_data = uartRead(uart_ch);
          cliPrintf("<- _DEF_UART%d RX : 0x%X\n", uart_ch + 1, rx_data);
        }

        if (cliAvailable() > 0)
        {
          rx_data = cliRead();
          if (rx_data == 'q')
          {
            break;
          }
          else
          {
            uartWrite(uart_ch, &rx_data, 1);
            cliPrintf("-> _DEF_UART%d TX : 0x%X\n", uart_ch + 1, rx_data);
          }
        }
      }
    }
    else
    {
      cliPrintf("This is cliPort\n");
    }
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("uart info\n");
    cliPrintf("uart test ch[1~%d]\n", HW_UART_MAX_CH);
  }
}
#endif

#endif
