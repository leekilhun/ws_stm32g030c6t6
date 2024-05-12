/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"

void apInit(void)
{

}

void apMain(void)
{

  while (1)
  {

  }

}

#ifdef _USE_HW_CLI
void cliApp(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "info") == true)
    {

      ret = true;
    }
  }
  else if (args->argc == 2)
  {

    // int result = 0;
  }

  if (ret == false)
  {
    cliPrintf("app info \n");
  }
}
#endif
