/**
 * @file def.hpp
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#ifndef DEF_HPP
#define DEF_HPP

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>

#include "main.h"



#define DEF_ERR_SUCCESS 0

#define _DEF_UART1 0
#define _DEF_UART2 1
#define _DEF_UART3 2
#define _DEF_UART4 3
#define _DEF_UART5 4
#define _DEF_UART6 5

#define _DEF_SPI1 0
#define _DEF_SPI2 1
#define _DEF_SPI3 2
#define _DEF_SPI4 3

#define _DEF_I2C1 0
#define _DEF_I2C2 1
#define _DEF_I2C3 2
#define _DEF_I2C4 3

#define _DEF_I2S1 0
#define _DEF_I2S2 1
#define _DEF_I2S3 2
#define _DEF_I2S4 3

#define _DEF_TIM1 0
#define _DEF_TIM2 1
#define _DEF_TIM3 2
#define _DEF_TIM4 3

#define _DEF_PWM1 0
#define _DEF_PWM2 1
#define _DEF_PWM3 2
#define _DEF_PWM4 3

#define _DEF_LOW 0
#define _DEF_HIGH 1

#define _DEF_INPUT 0
#define _DEF_INPUT_PULLUP 1
#define _DEF_INPUT_PULLDOWN 2
#define _DEF_OUTPUT 3
#define _DEF_OUTPUT_PULLUP 4
#define _DEF_OUTPUT_PULLDOWN 5

#define _DEF_CAN1 0
#define _DEF_CAN2 1
#define _DEF_CAN3 2
#define _DEF_CAN4 3

#define _DEF_CAN_100K 0
#define _DEF_CAN_125K 1
#define _DEF_CAN_250K 2
#define _DEF_CAN_500K 3
#define _DEF_CAN_1000K 4

#define _DEF_CAN_STD 0
#define _DEF_CAN_EXT 1


#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#ifndef cmax
#define cmax(a, b) (((a) > (b)) ? (a) : (b))
#define cmin(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef cmap
#define cmap(value, in_min, in_max, out_min, out_max) ((value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#define FLASH_MAGIC_NUMBER 0x5555AAAA

  typedef union
  {
    uint8_t u8Data[4];
    uint16_t u16Data[2];
    uint32_t u32Data;

    int8_t s8Data[4];
    int16_t s16Data[2];
    int32_t s32Data;

    uint8_t u8D;
    uint16_t u16D;
    uint32_t u32D;

    int8_t s8D;
    int16_t s16D;
    int32_t s32D;
  } data_t;

  typedef struct
  {
    uint8_t version[32];
    uint8_t name[32];
  } firm_version_t;

  typedef struct
  {
    uint32_t magic_number;

    //-- fw info
    //
    uint32_t addr_tag;
    uint32_t addr_fw;
    uint32_t size_tag;

    //-- tag info
    //
    uint32_t tag_flash_type;
    uint32_t tag_flash_start;
    uint32_t tag_flash_end;
    uint32_t tag_flash_length;
    uint32_t tag_flash_crc;
    uint32_t tag_length;
    uint8_t tag_date_str[32];
    uint8_t tag_time_str[32];

  } firm_tag_t;  


  



#endif // DEF_HPP