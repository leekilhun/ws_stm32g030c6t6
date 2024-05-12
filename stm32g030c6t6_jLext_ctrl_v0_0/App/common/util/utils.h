/**
 * @file utils.h
 * @author lee kil hun (gns2.lee@)
 * @brief 
 * @version 0.1
 * @date 2024-04-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef __COMMOM_UTILS_H__
#define __COMMOM_UTILS_H__

#include "def.h"
#ifdef __cplusplus
extern "C" {
#endif
uint8_t utilParseArgs(char *argStr, char **argv, const char* delim_char, int max);

#ifdef __cplusplus
}
#endif
#endif //__COMMOM_UTILS_H__