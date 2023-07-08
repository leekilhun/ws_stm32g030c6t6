/*
 * exhw.c
 *
 *  Created on: Jun 9, 2023
 *      Author: gns2l
 */



#include "exhw.h"


bool exhwInit(void)
{
  bool ret = true;


#ifdef _USE_EXHW_AT24C64
	ret = at24c64Init();
#endif


#ifdef _USE_EXHW_PCA8575PW_EX_IO
	ret &= pca8575pw_Init();
#endif

  return ret;
}
