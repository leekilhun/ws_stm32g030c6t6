/*
 * ap_utils.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_UTILS_HPP_
#define AP__INC_AP_UTILS_HPP_




namespace UTL
{
  inline void crc16_modbus_update(uint16_t* crc_in, uint8_t data) {
    constexpr uint16_t poly = 0xA001;
    uint16_t crc = *crc_in;
    uint8_t i;
    /* Exclusive-OR the byte with the CRC */
    crc ^= data; //*(pDataBuffer + iByte);

    /* Loop through all 8 data bits */
    for (i = 0; i <= 7; i++) {
      /* If the LSB is 1, shift the CRC and XOR the polynomial mask with the CRC */
      // Note - the bit test is performed before the rotation, so can't move the << here
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= poly;
      }
      else {
        // Just rotate it
        crc >>= 1;
      }
    }
    *crc_in = crc;
  }


#define AP_UTIL_QUE_BUFF_SIZE 32

  /// <summary>
  /// 데이터 버퍼 클래스
  /// </summary>
  template <typename T>
  struct _que
  {

    T m_Buff[AP_UTIL_QUE_BUFF_SIZE];
    uint8_t m_Head, m_Tail;

    _que()
    {
      m_Head = 0;
      m_Tail = 0;
    }
    virtual ~_que() {}

    inline void Init()
    {
      m_Head = m_Tail = 0;
    }

    inline bool Write(T *p_data, uint8_t length)
    {
      bool ret = true;
      uint32_t next_in;
      for (uint8_t i = 0; i < length; i++)
      {
        next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
        if (next_in != m_Tail)
        {
          m_Buff[m_Head] = p_data[i];
          m_Head = next_in;
        }
        else
        {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline bool Read(T *p_data, uint8_t length)
    {
      bool ret = true;
      for (uint8_t i = 0; i < length; i++)
      {
        p_data[i] = m_Buff[m_Tail];
        if (m_Tail != m_Head)
        {
          m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
        }
        else
        {
          ret = false;
          break;
        }
      }
      return ret;
    }

    inline uint8_t Available()
    {
      return (m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE;
    }

    inline bool Put(T data)
    {
      bool ret = true;
      uint32_t next_in;
      next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
      if (next_in != m_Tail)
      {
        m_Buff[m_Head] = data;
        m_Head = next_in;
      }
      else
        ret = false;

      return ret;
    }

    inline bool Get(T *p_data)
    {
      bool ret = true;
      *(p_data) = m_Buff[m_Tail];
      if (m_Tail != m_Head)
      {
        m_Tail = (m_Tail + 1) % AP_UTIL_QUE_BUFF_SIZE;
      }
      else
        ret = false;

      return ret;
    }

    inline T Pop(uint32_t addr)
    {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return m_Buff[m_Tail];
      return m_Buff[addr];
    }

    inline void Pop(uint32_t addr, T *p_data)
    {
      if (addr > (AP_UTIL_QUE_BUFF_SIZE - 1))
        return;
      *(p_data) = m_Buff[addr];
    }

    inline uint32_t GetSize()
    {
      return ((m_Head - m_Tail) % AP_UTIL_QUE_BUFF_SIZE);
    }

    inline T operator[](uint32_t addr)
    {
      return Pop[addr];
    }

    inline void Flush()
    {
      m_Head = m_Tail = 0;
    }
  };

}// end of namespace UTL



namespace trans
{
  inline uint8_t SplitArgs(char *arg_str, char **argv, const char *delim_chars, int _max)
  {
    uint8_t argc = 0;
    char *tok = nullptr;
    char *next_ptr = nullptr;

    for (tok = strtok_r(arg_str, delim_chars, &next_ptr); tok; tok = strtok_r(NULL, delim_chars, &next_ptr))
    {
      argv[argc++] = tok;
      if (argc >= _max)
        return argc - 1;
    }
    return argc;
  }
}




struct prc_step_t
{
  volatile uint8_t curr_step{};
  volatile uint8_t pre_step{};
  volatile uint8_t wait_step{};
  volatile uint32_t prev_ms{};
  volatile uint32_t elap_ms{};
  volatile uint8_t retry_cnt{};
  UTL::_que<uint8_t> msgQ;
  volatile bool wait_resp{};//true - wait step complete, false - completed step

  inline void SetStep(uint8_t step){
    elap_ms = millis() - prev_ms;
    prev_ms = millis();
    pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const{
    return curr_step;
  }

  inline uint32_t ElapTime() const {
    return elap_ms;
  }

  inline bool LessThan(uint32_t msec){
    elap_ms = millis() - prev_ms;
    if (elap_ms < msec)
      return true;
    else
      return false;
  }

  inline bool MoreThan(uint32_t msec){
      return !LessThan(msec);
  }


  inline bool Available() const {
    return !wait_resp;
  }


  inline bool IsInStep (uint8_t step) {
    if (msgQ.m_Head)
    {
      if (msgQ.m_Buff[msgQ.m_Head - 1] != step
          || msgQ.m_Buff[msgQ.m_Head - 1] != step + 1
          || msgQ.m_Buff[msgQ.m_Head - 1] != step + 2)
      {
        return true;
      }
    }
    return false;
  }

} ;



#endif /* AP__INC_AP_UTILS_HPP_ */
