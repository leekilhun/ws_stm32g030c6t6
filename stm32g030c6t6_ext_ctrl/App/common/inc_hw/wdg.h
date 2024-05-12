/*
 * wdg.h
 *
 *  Created on: Mar 13, 2023
 *      Author: baram
 */

#ifndef INCLUDE_WDG_H_
#define INCLUDE_WDG_H_

#include "hw_def.h"

#ifdef _USE_HW_WDG
#ifdef __cplusplus
extern "C"
{
#endif

  bool wdgInit(void);
  bool wdgBegin(uint32_t time_ms);
  bool wdgRefresh(void);

#ifdef __cplusplus
}
#endif
#endif

#endif /* INCLUDE_WDG_H_ */
