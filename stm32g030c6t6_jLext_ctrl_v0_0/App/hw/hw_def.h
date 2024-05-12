/**
 * @file hw_def.h
 * @author lee kil hun (gns2.lee@)
 * @brief
 * @version 0.1
 * @date 2024-04-30
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "bsp.h"

/*
use uart-dfu bootloader
*/
/****************************************************
   boot/ firmware memory information
 ****************************************************/
#define _DEF_FW_VERSION "V240506R1"
#define _DEF_FW_NAME   "jLext_ctrl_i8o8"

/* ------------------------------------------------- */

// #define _USE_HW_FLASH
// #define _USE_HW_RTC
// #define _USE_HW_RESET



#define _USE_HW_UART
#define      HW_UART_MAX_CH         2
#define      HW_LOG_CH              _DEF_UART2


#define _USE_HW_WDG

#define _USE_HW_SWTIMER
#define HW_SWTIMER_MAX_CH 4

// #define _USE_HW_TIM
// #define HW_TIM_MAX_CH 3

#define _USE_HW_I2C
#define HW_I2C_MAX_CH 1


#endif /* SRC_HW_HW_DEF_H_ */