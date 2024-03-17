/*
 * adc.c
 *
 *  Created on: Oct 8, 2023
 *      Author: gns2l
 */





#include "adc.h"


#ifdef _USE_HW_ADC
#define HW_ADC_MAX_CH 3

extern ADC_HandleTypeDef hadc1;

uint16_t adc_data[HW_ADC_MAX_CH];


bool adcInit(void)
{


#ifdef _USE_HW_ADC_DMA_RX
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 3);
#else
#endif
  return true;
}

uint16_t adcRead(uint8_t ch)
{
#ifdef _USE_HW_ADC_DMA_RX
  return adc_data[ch];
#else
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, 10);
  adc_data[ch] = HAL_ADC_GetValue(&hadc1);
  return adc_data[ch];
#endif
}

uint32_t adcReadVoltage(uint8_t ch)
{
  uint32_t ret;
  uint32_t adc_data;


  adc_data = adcRead(ch);

  ret = (330 * adc_data) / 4096;

  return ret;
}

#endif
