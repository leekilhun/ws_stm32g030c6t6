/**
 * @file log.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-05-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifndef COMMON_INC_HW_LOG_H_
#define COMMON_INC_HW_LOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "hw_def.h"

#define LOG_CH           HW_LOG_CH
#define LOG_BOOT_BUF_MAX HW_LOG_BOOT_BUF_MAX
#define LOG_LIST_BUF_MAX HW_LOG_LIST_BUF_MAX

  bool logInit(void);
  void logEnable(void);
  void logDisable(void);
  bool logOpen(uint8_t ch, uint32_t baud);
  bool logIsOpen(void);
  void logBoot(uint8_t enable);
  void logPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_INC_HW_LOG_H_ */
