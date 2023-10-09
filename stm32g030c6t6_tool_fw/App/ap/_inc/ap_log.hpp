/*
 * ap_log.hpp
 *
 *  Created on: 2023. 7. 15.
 *      Author: gns2.lee
 */

#ifndef SRC_AP_AP_LOG_HPP_
#define SRC_AP_AP_LOG_HPP_



#include "ap_def.h"

#ifdef _USE_HW_RTOS
#define AP_LOG_LOCK_BEGIN osMutexWait(ap_log_mutex_id, osWaitForever)
#define AP_LOG_LOCK_END   osMutexRelease(ap_log_mutex_id)
#else
#define AP_LOG_LOCK_BEGIN
#define AP_LOG_LOCK_END
#endif

#define AP_LOG_QUE_BUFF_SIZE        32

struct ap_log
{
  /****************************************************
   *  data
   ****************************************************/
public:
  log_dat m_log{};

  uint8_t m_Head{}, m_Tail{};
#ifdef _USE_HW_RTOS
  osMutexId ap_log_mutex_id;
#endif

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  ap_log (){
#ifdef _USE_HW_RTOS
    osMutexDef(ap_log_mutex_id);
    ap_log_mutex_id = osMutexCreate (osMutex(ap_log_mutex_id));
#endif

    m_Head = m_Tail = 0;
  }
  ~ap_log (){}

  /****************************************************
   *  func
   ****************************************************/

  inline void Init() {
    m_Head = m_Tail = 0;
  }

  inline uint8_t Available() {
    return (m_Head - m_Tail) % AP_LOG_QUE_BUFF_SIZE;
  }

  inline uint8_t GetAddrNo() {
    return (m_Head - (m_Head - m_Tail)) % AP_LOG_QUE_BUFF_SIZE;
  }

  inline bool apLogWrite(log_dat::head_t *p_head_info, const char *fmt, ...) {
    bool ret = true;
    AP_LOG_LOCK_BEGIN;

    va_list args;
    int len;

    va_start(args, fmt);
    len = vsnprintf(m_log.GetBuffer(), m_log.GetBufferLen(), fmt, args);
    UNUSED(len);
    m_log.log_buff.head = *(p_head_info);

    uint8_t next_in;
    next_in = (m_Head + 1) % AP_LOG_QUE_BUFF_SIZE;
    if (next_in != m_Tail) {
      // write
      m_log.WriteData(m_Head,m_log.log_buff);
      m_Head = next_in;
    }
    else
      ret = false;

    va_end(args);
    AP_LOG_LOCK_END;
    return ret;
  }

   inline bool Get(log_dat::dat_t* p_data) {
    bool ret = true;
    //AP_LOG_LOCK_BEGIN;

    uint8_t addr_no = GetAddrNo();
    p_data = m_log.GetData(addr_no);
    if (m_Tail != m_Head) {
      m_Tail = (m_Tail + 1) % AP_LOG_QUE_BUFF_SIZE;
    }
    else
      ret = false;

    //AP_LOG_LOCK_END;
    return ret;
  }

 /*
  inline uint8_t* Get() {
    uint8_t* ret = 0;
    //read *(p_data) = m_Buff[m_Tail];
    uint8_t addr_no = GetAddrNo();
    ret = m_log.Get(static_cast<log_dat::addr_e>(addr_no));

    if (m_Tail != m_Head) {
      m_Tail = (m_Tail + 1) % AP_LOG_QUE_BUFF_SIZE;
    }
    else
      ret = 0;

    return ret;
  }
  */

  inline log_dat::dat_t *Pop(uint8_t addr)
  {
    if (addr < AP_LOG_QUE_BUFF_SIZE)
      return m_log.GetData(addr);
    return nullptr;
  }
};

#endif /* SRC_AP_AP_LOG_HPP_ */
