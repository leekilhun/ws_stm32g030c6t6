/*
 * iic_s.h
 *
 *  Created on: 2023. 10. 18.
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_IIC_S_H_
#define COMMON_INC_HW_IIC_S_H_




#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_IIC_S

#define IIC_S_MAX_CH       HW_IIC_S_MAX_CH
  typedef enum
  {
    IICS_FREQ_100KHz,
    IICS_FREQ_400KHz,
    IICS_FREQ_1MHz,
  } iics_freq_t;



bool iicsInit(void);
bool iicsIsInit(void);
bool iicsBegin(uint8_t ch, iics_freq_t freq_khz);
bool iicsIsBegin(uint8_t ch);
void iicsReset(uint8_t ch);
bool iicsRecovery(uint8_t ch);
// bool iicsMain(void);

void     iicsSetTimeout(uint8_t ch, uint32_t timeout);
uint32_t iicsGetTimeout(uint8_t ch);

void     iicsClearErrCount(uint8_t ch);
uint32_t iicsGetErrCount(uint8_t ch);

// uint32_t iicsSlaveAvailable(uint8_t ch);
// uint8_t  iicsSlaveRead(uint8_t ch);

#endif

#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_HW_IIC_S_H_ */
