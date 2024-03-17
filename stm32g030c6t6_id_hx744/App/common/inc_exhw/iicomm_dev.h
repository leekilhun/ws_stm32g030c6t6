/*
 * iicomm_slave.h
 *
 *  Created on: 2023. 11. 1.
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_EXHW_IICOMM_DEV_H_
#define COMMON_INC_EXHW_IICOMM_DEV_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "exhw_def.h"

typedef enum
{
  IICOMM_CMD_READ_MCD_DATA  = 0x00,
  IICOMM_CMD_WR_MCD_DATA    = 0x00,
  IICOMM_CMD_WR_OUT         = 0x0C,
  IICOMM_CMD_WR_ORDER       = 0x0E,
  IICOMM_CMD_WR_ORDER_DATA  = 0x10,
} iicommDev_cmd_t;


#ifdef  _USE_EXHW_IICOMM_DEV

#define IICS_REG_MAX              32
#define IICS_REG_MCU_DATA_LENGTH  12
#define IICS_REG_OUT_DATA_LENGTH  4
#define IICS_REG_CMD_ORDR_LENGTH  2
#define IICS_REG_CMD_DATA_LENGTH  4
//reserved                        10

#define IICS_REG_OUT_IDX_MAX      1
#define IICS_REG_ORD_DATA_IDX_MAX 1




bool iicomm_devInit(void);

void iicomm_devUpdate(void);
bool iicomm_devWriteReg(uint8_t addr, uint8_t *p_data);
bool iicomm_devReadReg(uint8_t addr, uint8_t *p_data);


#endif



#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_EXHW_IICOMM_DEV_H_ */
