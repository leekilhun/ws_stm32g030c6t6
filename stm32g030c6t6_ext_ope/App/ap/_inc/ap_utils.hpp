/*
 * ap_utils.hpp
 *
 *  Created on: 2023. 11. 15.
 *      Author: gns2l
 *
 *
 *  machine_step_t 추가
 *  2024.01.08 : machine_step_t 수정 : operator 함수 반영
 */

#ifndef AP__INC_AP_UTILS_HPP_
#define AP__INC_AP_UTILS_HPP_

namespace UTL
{
  inline void crc16_modbus_update(uint16_t *crc_in, uint8_t data)
  {
    constexpr uint16_t poly = 0xA001;
    uint16_t           crc  = *crc_in;
    uint8_t            i;
    /* Exclusive-OR the byte with the CRC */
    crc ^= data; //*(pDataBuffer + iByte);

    /* Loop through all 8 data bits */
    for (i = 0; i <= 7; i++)
    {
      /* If the LSB is 1, shift the CRC and XOR the polynomial mask with the CRC */
      // Note - the bit test is performed before the rotation, so can't move the << here
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc  ^= poly;
      }
      else
      {
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

    T       m_Buff[AP_UTIL_QUE_BUFF_SIZE];
    uint8_t m_Head, m_Tail;

    _que()
    {
      m_Head = 0;
      m_Tail = 0;
    }
    ~_que() {}

    inline void Init()
    {
      m_Head = m_Tail = 0;
    }

    inline bool Write(T *p_data, uint8_t length)
    {
      bool     ret = true;
      uint32_t next_in;
      for (uint8_t i = 0; i < length; i++)
      {
        next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
        if (next_in != m_Tail)
        {
          m_Buff[m_Head] = p_data[i];
          m_Head         = next_in;
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
      bool     ret = true;
      uint32_t next_in;
      next_in = (m_Head + 1) % AP_UTIL_QUE_BUFF_SIZE;
      if (next_in != m_Tail)
      {
        m_Buff[m_Head] = data;
        m_Head         = next_in;
      }
      else
        ret = false;

      return ret;
    }

    inline bool Get(T *p_data)
    {
      bool ret  = true;
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
      return Pop[addr % AP_UTIL_QUE_BUFF_SIZE];
    }

    inline void Flush()
    {
      m_Head = m_Tail = 0;
    }
  };

  // template <typename Type, size_t Size>
  // struct tiny_Q{};

  /**
   * @brief tiny que buffer
   *
   * @tparam Type
   * @tparam Size
   */
  template <typename Type, size_t Size = 32>
  struct tiny_Q
  {

    uint8_t                head{}, tail{};
    std::array<Type, Size> buffer{};

    tiny_Q() = default;
    // copy constructor
    tiny_Q(const tiny_Q &rhs) = default;
    // copy assignment operator
    tiny_Q &operator=(const tiny_Q &rhs) = default;
    // move constructor
    tiny_Q(tiny_Q &&rhs) = default;
    // move assignment operator
    tiny_Q &operator=(tiny_Q &&rhs) = default;

    ~tiny_Q() = default;

    inline uint8_t available()
    {
      return (head - tail) % Size;
    }

    inline bool write(Type data)
    {
      uint8_t next_in = (head + 1) % Size;
      if (next_in != tail)
      {
        buffer[head] = data;
        head         = next_in;
        return true;
      }
      return false;
    };

    inline bool read(Type &data)
    {
      if (tail != head)
      {
        data = buffer[tail];
        tail = (tail + 1) % Size;
        return true;
      }
      return false;
    }

    inline Type &pop(uint8_t index)
    {
      if (index > (Size - 1))
        return buffer[tail];
      return buffer[index];
    }

    inline void flush()
    {
      head = tail = 0;
      buffer.fill(0);
    }

    inline Type &operator[](uint8_t index)
    {
      return pop(index);
    }
  };
  // end of class  tiny_Q

  /**
   * @brief tiny fifo buffer
   *
   * @tparam Type
   * @tparam Size
   * @tparam opt  true인 경우 index를 sheet하여 맨 처음 들어간 데이터는 버린다.
   *              false인 경우 buffer full을 반환하여 입력 받지 않는다.
   */
  template <typename Type, size_t Size = 32, bool opt = false>
  struct tiny_fifo
  {
    uint8_t                forward_cnt{};
    uint8_t                empty_cnt{};
    bool                   is_overflow{};
    std::array<Type, Size> buffer{};

    tiny_fifo()  = default;
    ~tiny_fifo() = default;

    inline bool is_full()
    {
      uint8_t check_no = is_overflow ? ((forward_cnt + Size) - empty_cnt) : forward_cnt - empty_cnt;
      return check_no == Size;
    }

    inline uint8_t available()
    {
      return is_overflow ? ((forward_cnt + Size) - empty_cnt) : forward_cnt - empty_cnt;
    }

    inline bool in(const Type &data)
    {
      if (is_full())
      {
        if (opt)
        {
          if (++empty_cnt == Size)
          {
            empty_cnt   = 0;
            is_overflow = false;
          }
        }
        else
          return false;
      }

      buffer[forward_cnt % Size] = data;

      if (++forward_cnt == Size)
      {
        forward_cnt = 0;
        is_overflow = true;
      }

      return true;
    };

    inline bool out(Type &data)
    {
      if (available() == 0)
        return false;

      data = buffer[empty_cnt % Size];
      if (++empty_cnt == Size)
      {
        empty_cnt   = 0;
        is_overflow = false;
      }

      return true;
    }

    inline Type &pop(uint8_t index)
    {
      return buffer[(empty_cnt + index) % Size];
    }

    inline Type &operator[](uint8_t index)
    {
      return pop(index);
    }
  };
  // end of class  tiny_fifo

} // end of namespace UTL

namespace trans
{
  inline uint8_t SplitArgs(char *arg_str, char **argv, const char *delim_chars, int _max)
  {
    uint8_t argc     = 0;
    char   *tok      = nullptr;
    char   *next_ptr = nullptr;

    for (tok = strtok_r(arg_str, delim_chars, &next_ptr); tok; tok = strtok_r(NULL, delim_chars, &next_ptr))
    {
      argv[argc++] = tok;
      if (argc >= _max)
        return argc - 1;
    }
    return argc;
  }
}

struct machine_step_t
{
  volatile uint8_t         curr_step{};
  volatile uint8_t         pre_step{};
  volatile uint8_t         wait_step{};
  volatile uint32_t        prev_ms{};
  volatile uint32_t        elap_ms{};
  volatile uint8_t         retry_cnt{};
  UTL::tiny_fifo<uint8_t, 32> steps{};

  inline void SetStep(uint8_t step)
  {
    elap_ms   = millis() - prev_ms;
    prev_ms   = millis();
    pre_step  = curr_step;
    curr_step = step;
    steps.in(step);
  }

  inline uint8_t GetStep() const
  {
    return curr_step;
  }

  inline uint32_t ElapTime() const
  {
    return elap_ms;
  }

  inline bool LessThan(uint32_t msec)
  {
    elap_ms = millis() - prev_ms;
    if (elap_ms < msec)
      return true;
    else
      return false;
  }

  inline bool MoreThan(uint32_t msec)
  {
    return !LessThan(msec);
  }

  inline uint8_t operator-(uint8_t dec_index)
  {
    if (dec_index > steps.available())
    {
      LOG_PRINT("dec_index is out of range");
      return 0;
    }
    else
    {
      return steps.pop(dec_index);
    }
  }
};
// end of struct machine_step_t

struct prc_step_t
{
  volatile uint8_t   curr_step{};
  volatile uint8_t   pre_step{};
  volatile uint8_t   wait_step{};
  volatile uint32_t  prev_ms{};
  volatile uint32_t  elap_ms{};
  volatile uint8_t   retry_cnt{};
  UTL::_que<uint8_t> msgQ;
  volatile bool      wait_resp{}; // true - wait step complete, false - completed step

  inline void SetStep(uint8_t step)
  {
    elap_ms   = millis() - prev_ms;
    prev_ms   = millis();
    pre_step  = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const
  {
    return curr_step;
  }

  inline uint32_t ElapTime() const
  {
    return elap_ms;
  }

  inline bool LessThan(uint32_t msec)
  {
    elap_ms = millis() - prev_ms;
    if (elap_ms < msec)
      return true;
    else
      return false;
  }

  inline bool MoreThan(uint32_t msec)
  {
    return !LessThan(msec);
  }

  inline bool Available() const
  {
    return !wait_resp;
  }

  inline bool IsInStep(uint8_t step)
  {
    if (msgQ.m_Head)
    {
      if (msgQ.m_Buff[msgQ.m_Head - 1] != step || msgQ.m_Buff[msgQ.m_Head - 1] != step + 1 || msgQ.m_Buff[msgQ.m_Head - 1] != step + 2)
      {
        return true;
      }
    }
    return false;
  }
};

#endif /* AP__INC_AP_UTILS_HPP_ */
