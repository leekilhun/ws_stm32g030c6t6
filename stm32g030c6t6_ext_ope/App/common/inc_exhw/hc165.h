/*
 * hc165.h
 *
 *  Created on: Oct 4, 2023
 *      Author: gns2l
 */

#ifndef COMMON_INC_EXHW_HC165_H_
#define COMMON_INC_EXHW_HC165_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"

#ifdef _USE_EXHW_HC165


  bool hc165_Init(void);
  bool hc165_IsInit(uint8_t ch);
  uint8_t hc165_ReadInputs(uint8_t ch);


#endif

#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_EXHW_HC165_H_ */
