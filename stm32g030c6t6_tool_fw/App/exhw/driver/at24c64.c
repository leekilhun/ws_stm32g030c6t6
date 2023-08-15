/*
 * at24c64.c
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */





#include "at24c64.h"
#include "cli.h"

// at24c64  8192*8bit   32byte 256 pages
#define AT24C64_PAGE_SIZE 32
#define AT24C64_PAGE_MAX  256

#ifdef _USE_EXHW_AT24C64



#ifdef _USE_HW_CLI
void cliAt24c64eeprom(cli_args_t *args);
#endif

#define EEPROM_MAX_SIZE       8192


#define AT23C64_I2C           _DEF_I2C2


static bool is_init = false;
static uint8_t i2c_addr = (0xa4>>1);
static uint8_t last_error = 0;



bool at24c64Init(void)
{
  bool ret;

  ret = i2cBegin(AT23C64_I2C, I2C_FREQ_400KHz);

  if (at24c64Valid(0x00) == false)
  {
    return false;
  }

#ifdef _USE_HW_CLI
  cliAdd("at24c64eeprom", cliAt24c64eeprom);
#endif

  is_init = ret;

  return ret;

}


bool at24c64IsInit(void)
{
  return is_init;
}

bool at24c64Valid(uint32_t addr)
{
  uint8_t data;
  bool ret;

  if (addr >= EEPROM_MAX_SIZE)
  {
    return false;
  }

  ret = i2cRead16Byte(AT23C64_I2C, i2c_addr, addr/* & 0xFF*/, &data, 100);

  return ret;
}

uint8_t at24c64ReadByte(uint32_t addr)
{
  uint8_t data;
  bool ret;

  if (addr >= EEPROM_MAX_SIZE)
  {
    return false;
  }

  ret = i2cRead16Byte(AT23C64_I2C, i2c_addr, addr /*& 0xFF*/, &data, 100);

  last_error = 0;
  if (ret != true) last_error = 1;

  return data;
}

bool at24c64WriteByte(uint32_t addr, uint8_t data_in)
{
  uint32_t pre_time;
  bool ret;

  if (addr >= EEPROM_MAX_SIZE)
  {
    return false;
  }

  ret = i2cWrite16Byte(AT23C64_I2C, i2c_addr, addr /*& 0xFF*/, data_in, 10);


  pre_time = millis();
  while(millis()-pre_time < 100)
  {

    ret = i2cRead16Byte(AT23C64_I2C, i2c_addr, addr, &data_in, 1);
    if (ret == true)
    {
      break;
    }
    delay(1);
  }

  return ret;
}

uint32_t at24c64GetLength(void)
{
  return EEPROM_MAX_SIZE;
}

bool  at24c64Read(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t i;

  if( is_init == false )
  {
    return false;
  }


  for (i=0; i<length; i++)
  {
    p_data[i] = at24c64ReadByte(addr+i);
  }

  return ret;
}

bool  at24c64Write(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t i;

  if( is_init == false )
  {
    return false;
  }


  for (i=0; i<length; i++)
  {
    ret = at24c64WriteByte(addr+i, p_data[i]);
    if (ret == false)
    {
      break;
    }
  }

  return ret;
}

bool at24c64Format(void)
{
  bool ret = true;

  for (uint16_t addr = 0; addr < EEPROM_MAX_SIZE; addr += AT24C64_PAGE_SIZE)
  {
    uint8_t val[AT24C64_PAGE_SIZE] = { 0 };
    memset(val,0xff,AT24C64_PAGE_SIZE);

    ret = i2cWrite16Bytes(AT23C64_I2C, i2c_addr, addr, val, AT24C64_PAGE_SIZE, 1000);
    delay(5);
  }

  /*
  for (uint16_t addr = 0; addr < EEPROM_MAX_SIZE; addr += AT24C32_PAGE_SIZE)
  {
    uint8_t val2[AT24C32_DATA_LENTH] = { 0 };
    val2[0] = (addr >> 8) & 0xFF;
    val2[1] = (addr & 0xFF);
    for (uint8_t j = 2; j <AT24C32_DATA_LENTH; j++)
    {
      val2[j] = 0xff;
    }
    ret = i2cWriteData(AT23C32_I2C, i2c_addr, val2, AT24C32_DATA_LENTH, 1000);
    delay(10);
  }
   */
  return ret;
}

bool at24c64pageErase(uint8_t page)
{
  bool ret = true;
  if (page < 1 || page > AT24C64_PAGE_MAX) return false;

  uint16_t addr = (page-1) * AT24C64_PAGE_SIZE;
  uint8_t val[AT24C64_PAGE_SIZE] = { 0 };
  memset(val, 0xff, AT24C64_PAGE_SIZE);

  ret = i2cWrite16Bytes(AT23C64_I2C, i2c_addr, addr, val, AT24C64_PAGE_SIZE, 1000);
  if (ret == true)
    delay(1);
  else
    logPrintf("at24c64_pageErase Failed \n");

  return ret;
}

bool at24c32pageWrite(uint8_t page, uint8_t* p_data)
{

  bool ret = true;
  if (page < 1 || page > AT24C64_PAGE_MAX) return false;

  uint16_t addr = (page-1) * AT24C64_PAGE_SIZE;

  ret = i2cWrite16Bytes(AT23C64_I2C, i2c_addr, addr, p_data, AT24C64_PAGE_SIZE, 1000);
  if (ret == true)
    delay(1);
  else
    logPrintf("at24c64_pageWrite Failed \n");

  return ret;
}

bool at24c64pageRead(uint8_t page, uint8_t* p_data)
{
  bool ret = true;

  if (page < 1 || page > AT24C64_PAGE_MAX) return false;

  uint16_t addr = (page-1) * AT24C64_PAGE_SIZE;

  ret = i2cRead16Bytes(AT23C64_I2C, i2c_addr, addr, p_data, AT24C64_PAGE_SIZE, 1000);
  if (ret == true)
    delay(1);
  else
    logPrintf("at24c64_pageRead Failed \n");

  return ret;

#if 0
  uint8_t dev_addr  = ((page*16) >>8) | 0x50;
  uint8_t word_addr = (page *16) & 0xff;

  i2cReadBytes(AT23C32_I2C, dev_addr, word_addr, p_data, length, 1);
#endif


}



#ifdef _USE_HW_CLI
void cliAt24c64eeprom(cli_args_t *args)
{
  bool ret = true;
  uint32_t i;
  uint32_t addr;
  uint32_t length;
  uint8_t  data;
  uint32_t pre_time;
  bool eep_ret;


  if (args->argc == 1)
  {
    if(args->isStr(0, "info") == true)
    {
      cliPrintf("at24c64eeprom init   : %d\n", at24c64IsInit());
      cliPrintf("at24c64eeprom length : %d bytes\n", at24c64GetLength());
    }
    else if(args->isStr(0, "format") == true)
    {
      if (at24c64Format() == true)
      {
        cliPrintf("format OK\n");
      }
      else
      {
        cliPrintf("format Fail\n");
      }
    }
    else
    {
      ret = false;
    }
  }
  else if (args->argc == 2)
  {
    if(args->isStr(0, "pageErase") == true)
    {
      addr = (uint32_t)args->getData(1);

      pre_time = millis();
      eep_ret = at24c64pageErase(addr);

      cliPrintf( "page : %d\t  %dms\n", addr,  millis()-pre_time);
      if (eep_ret)
      {
        cliPrintf("OK\n");
        for (i=0; i<AT24C64_PAGE_SIZE; i++)
        {
          data = at24c64ReadByte(((addr-1)*AT24C64_PAGE_SIZE)+i);
          cliPrintf( "addr : %d\t 0x%02X\n", ((addr-1)*AT24C64_PAGE_SIZE)+i, data);
        }

      }
      else
      {
        cliPrintf("FAIL\n");
      }
    }
    else if(args->isStr(0, "pageRead") == true)
    {
      addr = (uint32_t)args->getData(1);

      pre_time = millis();
      uint8_t val[AT24C64_PAGE_SIZE] = { 0 };
      eep_ret = at24c64pageRead(addr, val);

      cliPrintf( "page : %d\t  %dms\n", addr,  millis()-pre_time);
      if (eep_ret)
      {
        cliPrintf("OK\n");
        for (i=0; i<AT24C64_PAGE_SIZE; i++)
        {
          //data = at24c32ReadByte((addr*AT24C32_PAGE_SIZE)+i);
          cliPrintf( "addr : %d\t 0x%02X\n", ((addr-1)*AT24C64_PAGE_SIZE)+i, val[i]);
        }

      }
      else
      {
        cliPrintf("FAIL\n");
      }
    }
  }
  else if (args->argc == 3)
  {

    if(args->isStr(0, "read") == true)
    {
      addr   = (uint32_t)args->getData(1);
      length = (uint32_t)args->getData(2);

      if (length > at24c64GetLength())
      {
        cliPrintf( "length error\n");
      }
      for (i=0; i<length; i++)
      {
        data = at24c64ReadByte(addr+i);
        cliPrintf( "addr : %d\t 0x%02X\n", addr+i, data);
      }
    }
    else if(args->isStr(0, "write") == true)
    {
      addr = (uint32_t)args->getData(1);
      data = (uint8_t )args->getData(2);

      pre_time = millis();
      eep_ret = at24c64WriteByte(addr, data);

      cliPrintf( "addr : %d\t 0x%02X %dms\n", addr, data, millis()-pre_time);
      if (eep_ret)
      {
        cliPrintf("OK\n");
      }
      else
      {
        cliPrintf("FAIL\n");
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "at24c64eeprom info\n");
    cliPrintf( "at24c64eeprom format\n");
    cliPrintf( "at24c64eeprom pageErase  [page]\n");
    cliPrintf( "at24c64eeprom pageRead   [page]\n");
    cliPrintf( "at24c64eeprom read  [addr] [length]\n");
    cliPrintf( "at24c64eeprom write [addr] [data]\n");
  }

}
#endif /* _USE_HW_CLI */





#endif
