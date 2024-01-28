/*
 * adc.h
 *
 *  Created on: Oct 8, 2023
 *      Author: gns2l
 */

#ifndef COMMON_INC_HW_ADC_H_
#define COMMON_INC_HW_ADC_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_ADC


#define ADC_MAX_CH     HW_ADC_MAX_CH

bool adcInit(void);
uint16_t adcRead(uint8_t ch);
uint32_t adcReadVoltage(uint8_t ch);


#endif

#ifdef __cplusplus
}
#endif

#endif /* COMMON_INC_HW_ADC_H_ */
