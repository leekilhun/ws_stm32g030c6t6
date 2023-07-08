/*
 * uart.c
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */



#include "uart.h"

#ifdef _USE_HW_UART

#include "qbuffer.h"

#define UART_BUF_LENGTH      128

#define UART_MAX_CH         HW_UART_MAX_CH

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#ifdef _USE_HW_UART_1_DMA
extern DMA_HandleTypeDef hdma_usart1_rx;
#endif
#ifdef _USE_HW_UART_2_DMA
extern DMA_HandleTypeDef hdma_usart2_rx;
#endif


typedef struct
{
  bool     is_open;
  uint32_t baud;
  uint8_t  rx_buf[UART_BUF_LENGTH];
  uint8_t  rx_data;
} uart_tbl_t;


static qbuffer_t qbuffer[UART_MAX_CH];
static uart_tbl_t uart_tbl[UART_MAX_CH];



bool uartInit(void)
{

  for (int i=0; i<UART_MAX_CH; i++)
  {
    uart_tbl[i].is_open = false;
    uart_tbl[i].baud = 115200;
    uart_tbl[i].rx_data = 0;
  }
  memset(&uart_tbl[1].rx_buf[0],0x00,UART_BUF_LENGTH);
  return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
  bool ret = false;
  switch(ch)
  {
    case _DEF_UART1:
    {
      ret = true;
      uart_tbl[ch].is_open = true;
      qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);

#ifdef _USE_HW_UART_1_DMA
      if (huart1.RxState == HAL_UART_STATE_READY)
      {
        if(HAL_UART_Receive_DMA(&huart1, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
        {
          LOG_PRINT("HAL_UART_Receive_DMA failed! ch[%d]",_DEF_UART1);
          ret = false;
        }
        else
        {
          qbuffer[ch].in  = qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR;
          qbuffer[ch].out = qbuffer[ch].in;
        }
      }
#else
      if (HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
      {
        ret = false;
      }
#endif

    }
    break;

    case _DEF_UART2:
    {
      ret = true;
      uart_tbl[ch].is_open = true;
      qbufferCreate(&qbuffer[ch], &uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH);
#ifdef _USE_HW_UART_2_DMA
      if (huart2.RxState == HAL_UART_STATE_READY)
      {
        if(HAL_UART_Receive_DMA(&huart2, (uint8_t *)&uart_tbl[ch].rx_buf[0], UART_BUF_LENGTH) != HAL_OK)
        {
          ret = false;
        }
        else
        {
          qbuffer[ch].in  = qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR;
          qbuffer[ch].out = qbuffer[ch].in;
        }
      }
#else
      if (HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[ch].rx_data, 1) != HAL_OK)
      {
        ret = false;
      }
#endif
    }
    break;

  }

  return ret;
}



uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
  uint32_t ret = 0;
  HAL_StatusTypeDef hal_ret = HAL_OK;
  switch(ch)
  {
    case _DEF_UART1:
    {
      hal_ret = HAL_UART_Transmit(&huart1, p_data, length, 100);
      if (hal_ret == HAL_OK)
        ret = length;
    }
    break;
    case _DEF_UART2:
    {
      hal_ret = HAL_UART_Transmit(&huart2, p_data, length, 100);
      if (hal_ret == HAL_OK)
        ret = length;
    }
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
  switch(ch)
  {
    case _DEF_UART1:
#ifdef _USE_HW_UART_1_DMA
      qbuffer[ch].in = (qbuffer[ch].len - hdma_usart1_rx.Instance->CNDTR);
#endif
      ret = qbufferAvailable(&qbuffer[ch]);
      break;

    case _DEF_UART2:
#ifdef _USE_HW_UART_2_DMA
      qbuffer[ch].in = (qbuffer[ch].len - hdma_usart2_rx.Instance->CNDTR);
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
  while(uartAvailable(ch))
  {
    if (millis()-pre_time >= 10)
    {
      break;
    }
    uartRead(ch);
  }
  return true;
}


uint8_t  uartRead(uint8_t ch)
{
  uint8_t ret = 0;
  switch(ch)
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


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance == USART1)
  {
#ifdef _USE_HW_UART_1_DMA
#else
    qbufferWrite(&qbuffer[_DEF_UART1], &uart_tbl[_DEF_UART1].rx_data, 1);
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_tbl[_DEF_UART1].rx_data, 1);
#endif
  }
  else if (huart->Instance == USART2)
  {
#ifdef _USE_HW_UART_2_DMA
#else
    qbufferWrite(&qbuffer[_DEF_UART2], &uart_tbl[_DEF_UART2].rx_data, 1);
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_tbl[_DEF_UART2].rx_data, 1);
#endif
  }
}

#endif
