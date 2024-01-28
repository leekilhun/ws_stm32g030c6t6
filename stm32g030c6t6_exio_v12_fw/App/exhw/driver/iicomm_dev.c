/*
 * iicomm_slave.c
 *
 *  Created on: 2023. 11. 15.
 *      Author: gns2.lee
 */

#include "iicomm_dev.h"

#ifdef _USE_EXHW_IICOMM_DEV

//128 = 48(mcu_tool_data_align_48_t) + 12(out0-4, out1-4, out2-4) + 2 (command order 2(l,h)) + 8 (cmd_data0-4,cmd_data1-4) + 58(reserved)

//32    = 12(mcu_io_align_12_t) + 4(out0-4) + 2 (command order 2(l,h)) + 4 (cmd_data0-4) + 10(reserved)
//                                1  IICS_REG_OUT_IDX_MAX                1 IICS_REG_ORD_DATA_IDX_MAX  
                           
uint8_t IICS_REG[IICS_REG_MAX];


bool iicomm_devInit(void)
{
  memset(IICS_REG, 0, sizeof(IICS_REG));
  if (iicsBegin(_IIC_SLAVE, IICS_FREQ_400KHz) == false)
  {
    LOG_PRINT("[NG] iicsBegin  Fail!");
    return false;
  }

  LOG_PRINT("[OK] iicomm_devInit  Success!");
  return true;
}

void iicomm_devUpdate(void)
{

}

bool iicomm_devWriteReg(uint8_t addr, uint8_t *p_data)
{
  // uint8_t test_ptr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  // if (iicsIsReady(_IIC_SLAVE) == false)
  //   return false;

  switch (addr)
  {
  case IICOMM_CMD_WR_MCD_DATA:
    memcpy(&IICS_REG[IICOMM_CMD_WR_MCD_DATA], p_data, IICS_REG_MCU_DATA_LENGTH);
    break;

  case IICOMM_CMD_WR_OUT:
    memcpy(&IICS_REG[IICOMM_CMD_WR_OUT], p_data, IICS_REG_OUT_DATA_LENGTH);
    break;

  case IICOMM_CMD_WR_ORDER:
    memcpy(&IICS_REG[IICOMM_CMD_WR_ORDER], p_data, IICS_REG_CMD_ORDR_LENGTH);
    break;
    
  case IICOMM_CMD_WR_ORDER_DATA:
    memcpy(&IICS_REG[IICOMM_CMD_WR_ORDER_DATA], p_data, IICS_REG_CMD_DATA_LENGTH);
    break;

  default:
    *p_data = 0x00;
    break;
  }
  return true;
}

bool iicomm_devReadReg(uint8_t addr, uint8_t *p_data)
{
  switch (addr)
  {
  case IICOMM_CMD_WR_OUT:
    memcpy(p_data, &IICS_REG[addr], IICS_REG_OUT_DATA_LENGTH);
    break;
  case IICOMM_CMD_WR_ORDER:
    memcpy(p_data, &IICS_REG[addr], IICS_REG_CMD_ORDR_LENGTH);
    break;
  case IICOMM_CMD_WR_ORDER_DATA:
    memcpy(p_data, &IICS_REG[addr], IICS_REG_CMD_DATA_LENGTH);
    break;

  default:
    break;
  }

  return true;
}

void i2cReadCallback(uint8_t addr, uint8_t *p_data)
{
  //  *p_data = *(IICS_REG + (addr % 80));
  //  return;
  //LOG_PRINT("addr [0x%02X]", addr);
  if (addr < IICS_REG_MAX)
  {
    *p_data = *(IICS_REG + addr);
  }
  else
  {
    *p_data = 0x00;
  }
}

void i2cWriteCallback(uint8_t addr, uint8_t *p_data)
{
  // IICS_REG[addr % 10] = *p_data;
  // return;
  //LOG_PRINT("addr [0x%02X], data [0x%02X]", addr, *p_data);
  if ((addr >= IICOMM_CMD_WR_OUT) && addr < (IICOMM_CMD_WR_OUT + IICS_REG_OUT_DATA_LENGTH))
  {
    // act ?
    IICS_REG [addr] = *p_data ;
  }
  else if ((addr >= IICOMM_CMD_WR_ORDER) && addr < (IICOMM_CMD_WR_ORDER + IICS_REG_CMD_ORDR_LENGTH))
  {
    // act ?
    IICS_REG [addr]  = *p_data ;
  }
  else if ((addr >= IICOMM_CMD_WR_ORDER_DATA) && addr < (IICOMM_CMD_WR_ORDER_DATA + IICS_REG_CMD_DATA_LENGTH))
  {
    // act ?
    IICS_REG [addr]  = *p_data;
  }
  else
  {
    *p_data = 0x00;
  }
  //LOG_PRINT("writint Reg [0x%02X]", IICS_REG [addr]);
}

#endif