/*****************************************************************//**
 * \file   error_def.hpp
 * \brief  
 * 
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/
#pragma once
#ifndef SRC_COMMON_ERROR_DEF_HPP
#define SRC_COMMON_ERROR_DEF_HPP    


constexpr int ERROR_NO_ERROR = 0;      /* no error */

constexpr int ERROR_PORT_OPEN = -11; /* serial port open fail */
constexpr int ERROR_NOT_DEFINED = -1; /* */



constexpr int BOOT_ERR_NOT_OPEN = 1;
constexpr int BOOT_ERR_FAIL_OPEN = 2;
constexpr int BOOT_ERR_NACK = 3;
constexpr int BOOT_ERR_NO_RESP = 4;
constexpr int BOOT_ERR_TIMEOUT = 5;
constexpr int BOOT_ERR_WRITE_ADDR = 6;
constexpr int BOOT_ERR_WRITE_ADDR_ACK = 7;
constexpr int BOOT_ERR_WRITE_LEN = 8;
constexpr int BOOT_ERR_WRITE_LEN_ACK = 9;
constexpr int BOOT_ERR_WRITE_READ_DATA = 10;
constexpr int BOOT_ERR_CMD_READ_MEMORY_RDP = 11;
constexpr int BOOT_ERR_CMD_GET = 12;
constexpr int BOOT_ERR_CMD_GET_ID = 13;
constexpr int BOOT_ERR_CMD_GET_OPTION = 14;
constexpr int BOOT_ERR_CMD_PING = 15;
constexpr int BOOT_ERR_NOT_EXTENDED_ERASE = 16;
constexpr int BOOT_ERR_NOT_SUPPORT = 17;
constexpr int BOOT_ERR_CMD_GO = 18;
constexpr int BOOT_ERR_CMD_EX_ERASE = 19;
constexpr int BOOT_ERR_INVAILD_ADDR = 20;
constexpr int BOOT_ERR_ERASE_TIMEOUT = 21;
constexpr int BOOT_ERR_CMD_ERASE = 22;
constexpr int BOOT_ERR_NOT_ERASE_CMD = 23;
constexpr int BOOT_ERR_CMD_WRITE_UNPROTECT = 24;
constexpr int BOOT_ERR_CMD_READ_UNPROTECT = 25;

#endif /* SRC_COMMON_ERROR_DEF_HPP */