/**
 * @file hw.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef __HW_HPP__
#define __HW_HPP__
#ifdef __cplusplus
extern "C" {
#endif



#include "hw_def.h"

#include "cli.h"
#include "uart.h"
#include "swTimer.h"
#include "i2c.h"
#include "tim.h"


bool hwInit(void);


#ifdef __cplusplus
}
#endif
#endif //__HW_HPP__