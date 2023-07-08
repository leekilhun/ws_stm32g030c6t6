/*
 * utils.c
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */



#include "utils.h"


uint8_t utilParseArgs(char *argStr, char **argv, const char* delim_char, int max)
{
  uint8_t argc = 0;
  char *tok = NULL;
  char *next_ptr= NULL;
  for (tok = strtok_r(argStr, delim_char, &next_ptr); tok; tok = strtok_r(NULL, delim_char, &next_ptr))
  {
    argv[argc++] = tok;
    if (argc >= max)
           return argc - 1;
  }

  return argc;
}

