/**
 * @file hx711.h
 * @author  kilhun lee (gns2.lee@samsung.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * 
 * https://m.blog.naver.com/pkr7098/221539478274
 */


#ifndef COMMON_INC_EXHW_HX711_H_
#define COMMON_INC_EXHW_HX711_H_



#ifdef __cplusplus
extern "C" {
#endif



#include "exhw_def.h"

typedef enum
{
  HX711_GAIN_128 = 1,
  HX711_GAIN_32  = 2,
  HX711_GAIN_64  = 3,
} hx_711_gain_t;
typedef enum
{
  HX711_CH_A = 0,
  HX711_CH_B  = 1,
} hx_711_ch_t;


#ifdef _USE_EXHW_HX711

  bool hx711_Init(void);
  bool hx711_IsInit(uint8_t ch);
  bool hx711_IsAvailable(uint8_t ch);
  bool  hx711_SetGain(uint8_t ch, hx_711_gain_t gain);
  void hx711_SetCoefficient(uint8_t ch, float coefficient);
  int32_t hx711_Read(uint8_t ch);
  int32_t hx711_GetData(uint8_t ch);
  float hx11_ReadWeight(uint8_t ch, uint8_t sample_feq);
  void hx711_PowerOn(uint8_t ch);
  void hx711_PowerDown(uint8_t ch);


#endif

#ifdef __cplusplus
}
#endif




#endif /* COMMON_INC_EXHW_HX711_H_ */