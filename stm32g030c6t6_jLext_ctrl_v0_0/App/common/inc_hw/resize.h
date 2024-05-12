/**
 * @file resize.h
 * @author lee kil hun (pba7189@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#ifndef COMMON_INC_HW_RESIZE_H_
#define COMMON_INC_HW_RESIZE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

typedef struct
{
  int32_t  w;
  int32_t  h;
  int32_t  x;
  int32_t  y;
  int32_t  stride;
  uint16_t *p_data;
} resize_image_t;


void resizeImage(resize_image_t *src, resize_image_t *dest);
void resizeImageNearest(resize_image_t *src, resize_image_t *dest);
void resizeImageFast(resize_image_t *src, resize_image_t *dest);
void resizeImageFastOffset(resize_image_t *src, resize_image_t *dest);
void resizeImageFastPxp(resize_image_t *src, resize_image_t *dest);

void resizeImageFastGray(resize_image_t *src, resize_image_t *dest);


#ifdef __cplusplus
}
#endif



#endif /* COMMON_INC_HW_RESIZE_H_ */
