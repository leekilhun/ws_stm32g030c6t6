/*
 * hw_def.h
 *
 *  Created on: Jun 9, 2023
 *      Author: gns2l
 */

#ifndef HW_HW_DEF_H_
#define HW_HW_DEF_H_



#include "bsp.h"

/****************************************************
  firmware information
 ****************************************************/
#define _DEF_FW_VERSION             "V240226R1"
/**
 * @brief use extern io fw
 * 
 */
//#define _DEF_FW_EX_IO_I2C
#ifdef _DEF_FW_EX_IO_I2C
#define _DEF_FW_NAME                "EX_IO_IIC"
#else
#define _DEF_FW_NAME                "EX_IO_HX711"
#endif



#define _USE_HW_UART
#define      HW_UART_MAX_CH         1
#define      HW_UART_CH_MAIN        _DEF_UART1
#define      HW_LOG_CH              _DEF_UART1
#define _USE_HW_UART_1_DMA_RX
#define _USE_HW_UART_1_DMA_TX

//#define _USE_HW_UART_2_DMA_RX


#define _USE_HW_SWTIMER
#define      HW_SWTIMER_MAX_CH      4


#define _USE_HW_ADC
//#define _USE_HW_ADC_DMA_RX
#define _USE_HW_WDG

//#define _USE_HW_TIM
#ifdef  _USE_HW_TIM
#define      HW_TIM_MAX_CH          3
#define        _TIM_3               _DEF_TIM1
#define        _TIM_14               _DEF_TIM2
#define        _TIM_16               _DEF_TIM3
#endif



//#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    (32/2)
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    (8/2)
#define      HW_CLI_LINE_BUF_MAX    (64/2)



#define      HW_SLAVE_ADDRESS       0x41
#ifdef _DEF_FW_EX_IO_I2C
#define  _USE_HW_IIC_S
#define       HW_IIC_S_MAX_CH       1
 #define        _IIC_SLAVE          _DEF_IIC_S1
 #define _USE_HW_IIC_S_1_DMA_RX
 #define _USE_HW_IIC_S_1_DMA_TX
#else
#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          1
#define _USE_HW_I2C_1_DMA_RX
#endif

#endif /* HW_HW_DEF_H_ */
