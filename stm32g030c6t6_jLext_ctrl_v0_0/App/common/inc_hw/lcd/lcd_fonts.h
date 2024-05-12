/*
 * lcd_fonts.h
 *
 *  Created on: 2020. 12. 30.
 *      Author: baram
 */

#ifndef SRC_COMMON_HW_INCLUDE_LCD_LCD_FONTS_H_
#define SRC_COMMON_HW_INCLUDE_LCD_LCD_FONTS_H_


#include "hw_def.h"

// typedef struct
// {
//   uint16_t Code;
//   //uint16_t Size_Char;  // 글자 1개의 바이트수(한글:2 영문:1)
//   //uint16_t Code_Type;  // 한/영/특수 문자인지 감별

//   uint8_t  FontBuffer[32];
// } font_buff_t;

typedef struct
{
  uint8_t   width;
  uint8_t   height;
  const uint16_t *data;
} lcd_font_t;


#ifdef _USE_LCD_FONT_7x10
extern lcd_font_t font_07x10;
#endif
#ifdef _USE_LCD_FONT_11x18
extern lcd_font_t font_11x18;
#endif
#ifdef _USE_LCD_FONT_16x26
extern lcd_font_t font_16x26;
#endif
#ifdef _USE_LCD_FONT_HANGUL_16x16
extern lcd_font_t font_hangul;
#endif

#endif /* SRC_COMMON_HW_INCLUDE_LCD_LCD_FONTS_H_ */
