/**
 * @file hx711.c
 * @author kilhun lee (gns2.lee@samsung.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-26
 * 
 * @copyright Copyright (c) 2024
 * 
 * 
 * https://github.com/nimaltd/HX711/blob/master/hx711.c
 * 
 */

#include "hx711.h"

#define HX711_MAX_CH _EXHW_HX711_MAX_CH


typedef struct
{
  bool is_init;
  hx_711_ch_t ch; // A ch = gain 64, 128, b ch = gain 32
  hx_711_gain_t gain;
  float coefficient;
  int32_t       offset;

  GPIO_TypeDef *do_port; //data output
  int           do_pin;

  GPIO_TypeDef *sck_port; //in sck
  int           sck_pin;
} hx711_tbl_t;

static hx711_tbl_t hx711_tbl[HX711_MAX_CH] =
    {
        { false, HX711_CH_A, HX711_GAIN_64, 1.0f, 0, HX7111_DT_GPIO_Port, HX7111_DT_Pin,  HX711_SCK_GPIO_Port, HX711_SCK_Pin}  // chanel 1
    };

bool hx711_Init(void)
{
  for ( int i = 0; i < HX711_MAX_CH; i++ )
  {
      hx711_tbl[i].is_init = true; 
      hx711_tbl[i].ch = HX711_CH_A; 
      hx711_tbl[i].gain = HX711_GAIN_128;
      hx711_tbl[i].coefficient = 1.0f;
      HAL_GPIO_WritePin(hx711_tbl[i].do_port, hx711_tbl[i].do_pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(hx711_tbl[i].sck_port, hx711_tbl[i].sck_pin, GPIO_PIN_RESET);
  }  
  return  true;
}

bool hx711_IsAvailable(uint8_t ch)
{
  if (ch >= HX711_MAX_CH)
    return false;
  return (HAL_GPIO_ReadPin(hx711_tbl[ch].do_port, hx711_tbl[ch].do_pin) == GPIO_PIN_RESET);
}

bool hx711_IsInit(uint8_t ch)
{
  return hx711_tbl[ch].is_init;
}

void hx711_SetCoefficient(uint8_t ch, float coefficient)
{
  
  if (ch >= HX711_MAX_CH)
    return ;

  hx711_tbl[ch].coefficient = coefficient;
}

float hx11_ReadWeight(uint8_t ch, uint8_t sample_feq)
{
  if (ch >= HX711_MAX_CH)
    return 0.0f;
  int64_t average = 0;
  for (uint8_t i = 0; i < sample_feq; i++)
  {
    average += hx711_Read(ch);
  }

  int32_t data = (int32_t)(average / sample_feq);
  float weight = (float)(data - hx711_tbl[ch].offset) / hx711_tbl[ch].coefficient;

  return weight;
}

 int32_t hx711_GetData(uint8_t ch)
 {
   if (ch >= HX711_MAX_CH)
    return 0;

  const uint8_t delay_micro_sec = 1;

  delay_us(delay_micro_sec);

  uint32_t val_data = 0;
  const uint8_t adc_resolution_bit_cnt = 24;
  for (uint8_t i = 0; i < adc_resolution_bit_cnt; i++)
  {
    //gerate clock
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_SET);
    delay_us(delay_micro_sec);
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_RESET);
    delay_us(delay_micro_sec);

    val_data = val_data << 1; //shift << 1
    if (HAL_GPIO_ReadPin(hx711_tbl[ch].do_port, hx711_tbl[ch].do_pin) == GPIO_PIN_SET)
      val_data++;
  }

  /* gain pulse*/
  for (uint8_t i = 0; i < (uint8_t)hx711_tbl[ch].gain; i++)
  {
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_SET);
    delay_us(delay_micro_sec);
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_RESET);
    delay_us(delay_micro_sec);
}

  val_data = val_data ^ 0x800000; //2's complement;

  return (int32_t)val_data;

 }

int32_t hx711_Read(uint8_t ch)
{
  if (ch >= HX711_MAX_CH)
    return 0;

  uint32_t  begin_ms= millis();
  while(HAL_GPIO_ReadPin(hx711_tbl[ch].do_port, hx711_tbl[ch].do_pin) == GPIO_PIN_SET)
  {
    delay(1);
    if(millis() - begin_ms > 150)
      return 0;
  }

  const uint8_t delay_micro_sec = 10;

  delay_us(delay_micro_sec);

  uint32_t val_data = 0;
  const uint8_t adc_resolution_bit_cnt = 24;
  for (uint8_t i = 0; i < adc_resolution_bit_cnt; i++)
  {
    //gerate clock
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_SET);
    delay_us(delay_micro_sec);
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_RESET);
    delay_us(delay_micro_sec);

    val_data = val_data << 1; //shift << 1
    if (HAL_GPIO_ReadPin(hx711_tbl[ch].do_port, hx711_tbl[ch].do_pin) == GPIO_PIN_SET)
      val_data++;
  }

  /* gain pulse*/
  for (uint8_t i = 0; i < (uint8_t)hx711_tbl[ch].gain; i++)
  {
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_SET);
    delay_us(delay_micro_sec);
    HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_RESET);
    delay_us(delay_micro_sec);
}

  val_data = val_data ^ 0x800000; //2's complement;

  return (int32_t)val_data;
}


bool hx711_SetGain(uint8_t ch, hx_711_gain_t gain)
{
  if ( ch >= HX711_MAX_CH ) return false;
  hx711_tbl[ch].gain = gain;
  return true;
}

void hx711_PowerOn(uint8_t ch)
{
  HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_RESET);
}
void hx711_PowerDown(uint8_t ch)
{
  HAL_GPIO_WritePin(hx711_tbl[ch].sck_port, hx711_tbl[ch].sck_pin, GPIO_PIN_SET);
}

