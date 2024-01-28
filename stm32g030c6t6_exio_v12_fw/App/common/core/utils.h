/*
 * utils.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_UTILS_H_
#define INCLUDE_UTILS_H_




#include "def.h"


#ifdef __cplusplus
 extern "C" {
#endif



#include "def.h"

uint8_t utilParseArgs(char *argStr, char **argv, const char* delim_char, int max);


#ifdef __cplusplus
 }
#endif



#endif /* INCLUDE_UTILS_H_ */
