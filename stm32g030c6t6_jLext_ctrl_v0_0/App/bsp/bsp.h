/**
 * @file bsp.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef BSP_BSP_H_
#define BSP_BSP_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"


#include "stm32g0xx_hal.h"


  bool bspInit(void);
  void bspDeInit(void);
  void delay(uint32_t time_ms);
  void delay_us(uint32_t us);
  uint32_t millis(void);
  void logPrintf(const char *fmt, ...);
  void logView(const char* file, const char* func, const int line, const char* fmt, ...);


#ifndef LOG_PRINT
#define LOG_PRINT(fmt, ...)  logView(__FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif


#endif /* BSP_BSP_H_ */
