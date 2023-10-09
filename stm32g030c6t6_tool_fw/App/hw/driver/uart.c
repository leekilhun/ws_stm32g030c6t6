/*
 * uart.c
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "uart.h"

#ifdef _USE_HW_UART

#include "qbuffer.h"

#define UART_BUF_LENGTH 256

#define UART_MAX_CH HW_UART_MAX_CH

typedef struct
{
  bool is_open;
  uint32_t baud;
  uint8_t rx_buf[UART_BUF_LENGTH];
  uint8_t rx_data;
  uint8_t *p_tx_buf;
  UART_HandleTypeDef *p_huart;
  DMA_HandleTypeDef *p_hdma_rx;
} uart_tbl_t;

#ifdef _USE_HW_CLI
static void cliUart(cli_args_t *args);
#endif

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;

static qbuffer_t qbuffer[UART_MAX_CH];
static uart_tbl_t uart_tbl[UART_MAX_CH];

#ifdef _USE_HW_UART_1_DMA_TX
static uint8_t uart1_tx_data[UART_BUF_LENGTH];
#endif
#ifdef _USE_HW_UART_2_DMA_TX
static uint8_t uart2_tx_data[UART_BUF_LENGTH];
#endif

bool uartInit(void)
{

  for (int i = 0; i < UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;
    uart_tbl[i].rx_data = 0;
    uart_tbl[i].p_tx_buf = NULL;
    uart_tbl[i].p_huart = NULL;
    uart_tbl[i].p_hdma_rx = NULL;
    memset(&uart_tbl[i].rx_buf[0], 0, UART_BUF_LENGTH);

    switch (i)
    {
    case _DEF_UART1:
#ifdef _USE_HW_UART_1_DMA_TX
      uart_tbl[i].p_tx_buf = &uart1_tx_data[0];
#endif
      uart_tbl[i].p_huart = &huart1;
      uart_tbl[i].p_hdma_rx = &hdma_usart1_rx;
      break;

    case _DEF_UART2:
#ifdef _USE_HW_UART_2_DMA_TX
      uart_tbl[i].p_tx_buf = &uart2_tx_data[0];
#endif
      uart_tbl[i].p_huart = &huart2;
      uart_tbl[i].p_hdma_rx = &hdma_usart2_rx;
      break;
    default:
      break;
    }
  }

#ifdef _USE_HW_CLI
  cliAdd("uart", cliUart);
#endif
  return true;
}

bool uartDeInit(void)
{
  return true;
}

bool uartIsInit(void)
{
  return true;
}

bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;

  if (ch >= UART_MAX_CH)
  {
    return false;
  }

  UART_HandleTypeDef *ptr_uart = uart_tbl[ch].p_huart;
  DMA_HandleTypeDef *ptr_hdma_rx = uart_tbl[ch].p_hdma_rx;

  ptr_uart->Init.BaudRate = baud;
  HAL_UART_DeInit(ptr_uart);
  if (HAL_UART_Init(ptr_uart) != HAL_OK)
  {
    return false;
  }

  ret = true;
  uart_tbl[ch].is_open = true;
  qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);

  bool is_dma = false;
  switch (ch)
  {
  case _DEF_UART1:
#ifdef _USE_HW_UART_1_DMA_RX
    is_dma = true;
#endif
    break;

  case _DEF_UART2:
#ifdef _USE_HW_UART_2_DMA_RX
    is_dma = true;
#endif
    break;

  default:
    break;
  }

  if (is_dma)
  {
    if (HAL_UART_Receive_DMA(ptr_uart, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
    {
      logPrintf("HAL_UART_Receive_DMA failed! ch[%d]", _DEF_UART1);
      ret = false;
    }
    else
    {
      qbuffer[ch].in = qbuffer[ch].len - ptr_hdma_rx->Instance->CNDTR;
      qbuffer[ch].out = qbuffer[ch].in;
    }
  }
  else
  {
    if (HAL_UART_Receive_IT(ptr_uart, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
    {
      ret = false;
    }
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

  switch (ch)
  {
  case _DEF_UART1:
#ifdef _USE_HW_UART_1_DMA_RX
    qbuffer[ch].in = (qbuffer[ch].len - uart_tbl[ch].p_hdma_rx->Instance->CNDTR);
#endif
    ret = qbufferAvailable(&qbuffer[ch]);
    break;

  case _DEF_UART2:
#ifdef _USE_HW_UART_2_DMA_RX
    qbuffer[ch].in = (qbuffer[ch].len - uart_tbl[ch].p_hdma_rx->Instance->CNDTR);
#endif
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
  uint32_t ret = 0;
  HAL_StatusTypeDef hal_ret = HAL_OK;
  bool is_dma = false;
  switch (ch)
  {
  case _DEF_UART1:
#ifdef _USE_HW_UART_1_DMA_TX
    is_dma = true;
#endif
    break;
  case _DEF_UART2:
#ifdef _USE_HW_UART_2_DMA_TX
    is_dma = true;
#endif
    break;
  default:
    break;
  }
  if (is_dma)
  {
    uint32_t pre_time;
    pre_time = millis();
    while (uart_tbl[ch].p_huart->gState != HAL_UART_STATE_READY)
    {
      if (millis() - pre_time >= 100)
      {
        break;
      }
    }
    if (uart_tbl[ch].p_huart->gState == HAL_UART_STATE_READY)
    {
      memset(uart_tbl[ch].p_tx_buf, 0, UART_BUF_LENGTH);
      memcpy(uart_tbl[ch].p_tx_buf, p_data, length);
      hal_ret = HAL_UART_Transmit_DMA(uart_tbl[ch].p_huart, uart_tbl[ch].p_tx_buf, length);
    }
  }
  else
  {
    hal_ret = HAL_UART_Transmit(uart_tbl[ch].p_huart, p_data, length, 100);
  }

  if (hal_ret == HAL_OK)
    ret = length;

  return ret;
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
  uint32_t ret = 0;
  va_list arg;
  char print_buf[256];

  va_start(arg, fmt);

  int len;
  len = vsnprintf(print_buf, 256, fmt, arg);
  va_end(arg);

  if (len > 0)
  {
    ret = uartWrite(ch, (uint8_t *)print_buf, len);
  }

  return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
  uint32_t ret = 0;

  switch (ch)
  {
  case _DEF_UART1:
  case _DEF_UART2:
    ret = uart_tbl[ch].baud;
    break;

  default:
    break;
  }

  return ret;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {
#ifndef _USE_HW_UART_1_DMA_RX
    qbufferWrite(&qbuffer[_DEF_UART2], &uart_tbl[_DEF_UART2].rx_data, 1);
    HAL_UART_Receive_IT(uart_tbl[_DEF_UART2].p_huart, (uint8_t *)&uart_tbl[_DEF_UART2].rx_data, 1);
#endif
  }
  else if (huart->Instance == USART2)
  {
#ifndef _USE_HW_UART_2_DMA_RX
    qbufferWrite(&qbuffer[_DEF_UART3], &uart_tbl[_DEF_UART3].rx_data, 1);
    HAL_UART_Receive_IT(uart_tbl[_DEF_UART3].p_huart, (uint8_t *)&uart_tbl[_DEF_UART3].rx_data, 1);
#endif
  }
}

#ifdef _USE_HW_CLI
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
