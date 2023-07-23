/*
 * hw_def.h
 *
 *  Created on: Jun 9, 2023
 *      Author: gns2l
 */

#ifndef HW_HW_DEF_H_
#define HW_HW_DEF_H_



#include "bsp.h"




#define _USE_HW_LED
#define      HW_LED_MAX_CH          1
#define      HW_LED_CH_DEBUG        _DEF_LED1


#define _USE_HW_UART
#define      HW_UART_MAX_CH         2
#define      HW_UART_CH_RS485       _DEF_UART1
#define      HW_UART_CH_DEBUG       _DEF_UART2
#define _USE_HW_UART_1_DMA
#define _USE_HW_UART_2_DMA



#define _USE_HW_TIM
#ifdef  _USE_HW_TIM
#define      HW_TIM_MAX_CH          1
#define        _TIM_3               0
#endif



//#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    32
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    8
#define      HW_CLI_LINE_BUF_MAX    64


//#define _USE_HW_I2C

#endif /* HW_HW_DEF_H_ */
