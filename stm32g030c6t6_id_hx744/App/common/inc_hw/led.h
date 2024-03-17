/*
 * led.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_LED_H_
#define INCLUDE_LED_H_

#include "hw_def.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define LED_MAX_CH  HW_LED_MAX_CH

   bool ledInit(void);
   void ledOn(uint8_t ch);
   void ledOff(uint8_t ch);
   void ledToggle(uint8_t ch);


#ifdef __cplusplus
 }
#endif

#endif /* INCLUDE_LED_H_ */
