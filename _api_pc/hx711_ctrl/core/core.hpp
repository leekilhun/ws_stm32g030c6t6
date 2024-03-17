/*****************************************************************//**
 * \file   core.hpp
 * \brief  
 * 
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef CORE_CORE_HPP
#define CORE_CORE_HPP


#include "mcc_conversion.hpp"
#include "mcc_utils.hpp"

#include "common/util/mcc_cli.hpp"
#include "common/util/mcc_log.hpp"
#include "common/util/mcc_json.hpp"



#define SYSLOGFILE_INFO "./Log/system.log"
#define SYSLOGFILE_ERR "./Log/sysError.log"

#define __LOGFUNC__ __FUNCTION__
#define SYSLOG_INFO(obj, fmt, ...) TinyC::mcc_log::PutLog(TinyC::mcc_log::lvl_info, obj, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_PRINT(fmt, ...)TinyC::mcc_log::WriteLog(TinyC::mcc_log::lvl_info, 0, SYSLOGFILE_INFO, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define ERR_PRINT(fmt, ...) TinyC::mcc_log::WriteLog(TinyC::mcc_log::lvl_err, 0, SYSLOGFILE_ERR, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_OUT(fmt, ...) TinyC::mcc_log::Log_Print((__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define TRACE_OUT(fmt, ...) TinyC::mcc_log::trace_prc((__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)


inline constexpr unsigned char operator""_byte(unsigned long long value)
{
  return static_cast<uint8_t>(value);
}


inline constexpr unsigned int operator""_ms(unsigned long long value)
{
  return static_cast<unsigned int>(value);
}

#endif // CORE_CORE_HPP