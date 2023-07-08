/*
 * at24c64.h
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef COMMON_INC_EXHW_AT24C64_H_
#define COMMON_INC_EXHW_AT24C64_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_AT24C64

  bool at24c64Init(void);
  bool at24c64IsInit(void);
  bool at24c64Valid(uint32_t addr);

  uint8_t at24c64ReadByte(uint32_t addr);
  bool at24c64WriteByte(uint32_t addr, uint8_t data_in);
  uint32_t at24c64GetLength(void);

  bool  at24c64Read(uint32_t addr, uint8_t *p_data, uint32_t length);
  bool  at24c64Write(uint32_t addr, uint8_t *p_data, uint32_t length);

  bool at24c64Format(void);
  bool at24c64pageErase(uint8_t page);
  bool at24c64pageWrite(uint8_t page, uint8_t* p_data);
  bool at24c64pageRead(uint8_t page, uint8_t* p_data);


#endif

#ifdef __cplusplus
}
#endif



#endif /* COMMON_INC_EXHW_AT24C64_H_ */
