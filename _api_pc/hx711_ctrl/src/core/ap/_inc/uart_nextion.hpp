/*****************************************************************//**
 * \file   uart_nextion.hpp
 * \brief
 *
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef AP__INC_UART_NEXTION_HPP_
#define AP__INC_UART_NEXTION_HPP_

#include "../ap_def.hpp"

namespace ap_core
{
  /*

  ap - client, lcd - server

  TX  (ap -> lcd) provide information
  obj_id [option] 0 is all or ignore

  RX (lcd -> ap) request information or action
  obj_id [option] 0 is all or ignore
  | SOF  | page | type |Obj Id| length |Data(max 120) |   Checksum   | EOF  |
  | :--- |:-----|:-----|:---- | :- --- |:-------------|:-------------| :--  |
  | 0x4A |1 byte|1 byte|1 byte| 1 byte |Data 0～Data n|2 byte(crc 16)| 0x4C |


  | SOF  | page | type | request beep sound - return true
  | :--- |:-----|:-----|
  | 0x4A |1 byte|   0  |

  | reply  | reply page |
  | :---   |:-----------|
  | 0x66   |   1 byte   |



   */


#ifdef _USE_AP_UART_NEXTION

  template<typename DataType, std::size_t BufferSize = 4096>
  class uart_nextion :public ap_core::IComm
  {

  public:
    static constexpr uint8_t CMD_STX = 0x4A;
    static constexpr uint8_t CMD_ETX = 0x4C;

    static constexpr int CMD_MAX_DATA_LENGTH = 120;
    static constexpr int CMD_MAX_PACKET_LENGTH = (CMD_MAX_DATA_LENGTH + 8);
    static constexpr int CMD_STATE_REPLY_PAGE_NO = 11;

    /*
  – Level 0 is Off – no pass/fail will be returned
  – Level 1 is OnSuccess, only when last serial command successful.
  – Level 2 is OnFailure, only when last serial command failed
  – Level 3 is Always, returns 0x00 to 0x23 result of serial command.
   */
    enum bkCMdLevel
    {
      NO_RETURN,
      ONLY_SUCCESS,
      ONLY_FAIL,
      ALWAYS_RETURN,
    };

    enum page_e : uint8_t
    {
      INIT = 0,
      MAIN = 1,
      MANUAL = 2,
      TEACH = 3,
      IO = 4,
      LOG = 5,
      SETUP = 6,
      TASK = 7,
      SELTTEST = 8,
      ALARM = 10,
      CONFIRM = 11,
      page_max
    };

    //TX  (ap -> lcd) provide information
    enum class type_t : uint8_t
    {
      TX_LCD_BKCMDLEVEL = 0x11,
      TX_LCD_START_REPARSEMODE = 0x12,
      TX_LCD_END_REPARSEMODE = 0x13,
      TX_LCD_CHANGE_PAGE = 0x14,
      TX_LCD_REQUSET_PAGE = 0x15,
      TX_LCD_RESET_RECEIVE_CNT = 0x16,

      TX_OK_RESPONSE = 0xAA
    };


    //RX (lcd -> ap) request information or action
    enum cmd_e :uint8_t
    {
      CMD_OK_RESPONSE = 0x00,
      CMD_READ_MCU_DATA = 0x01,
      CMD_READ_FIRM_INFO = 0x02,

      CMD_CTRL_OUT_REG = 0x10,
      CMD_CTRL_OPT_REG = 0x11,
      CMD_CTRL_OPE_BTN = 0x12,
      CMD_CTRL_CYL = 0x15,
      CMD_CTRL_VAC = 0x16,


      CMD_CTRL_MOT_ORIGIN = 0x20,
      CMD_CTRL_MOT_ONOFF = 0x21,
      CMD_CTRL_MOT_MOVE = 0x22,
      CMD_CTRL_MOT_STOP = 0x23,
      CMD_CTRL_MOT_JOG = 0x24,
      CMD_CTRL_MOT_LIMIT = 0x25,
      CMD_CTRL_MOT_ZEROSET = 0x26,
      CMD_CTRL_MOT_RELMOVE = 0x27,
      CMD_CTRL_MOT_CLEAR_ALARM = 0x28,
      CMD_CTRL_MOT_CHANGE_VEL = 0x29,
      CMD_CTRL_MOT_MOVE_VEL = 0x2A,
      CMD_CTRL_MOT_RELMOVE_VEL = 0x2B,
      CMD_CTRL_MOT_VEL_JOG = 0x2C,

      CMD_CTRL_STPMOTOR_ORIGIN = 0x30,
      CMD_CTRL_STPMOTOR_ONOFF = 0x31,
      CMD_CTRL_STPMOTOR_MOVE = 0x32,
      CMD_CTRL_STPMOTOR_RELMOVE = 0x33,

      CMD_CTRL_INITIALIZE = 0x50,

      CMD_RPY_BKCMD_OK = 0x66,
      CMD_CTRL_BKCMD = 0xAA,
      CMD_RET_CURR_PAGE_NO = 0xB0,
      CMD_CTRL_REQ_BEEP = 0xBB,

      CMD_MODE_FW_DOWNLOAD = 0xF0,
    };


  public:
    using data_type = DataType;
    using uart_t = serial_comm<data_type>;

    struct cfg_t
    {
      uart_t* ptr_uart{};
      std::string port_name{};
      uint32_t baudrate{};

      cfg_t() = default;
      ~cfg_t() = default;

      // copy constructor
      cfg_t(const cfg_t& rhs) = default;
      // copy assignment operator
      cfg_t& operator=(const cfg_t& rhs) = default;
      // move constructor
      cfg_t(cfg_t&& rhs) = default;
      // move assignment operator
      cfg_t& operator=(cfg_t&& rhs) = default;

      cfg_t& operator()(std::string port_name, uint32_t baudrate)
      {
        this->port_name = port_name;
        this->baudrate = baudrate;
        return *this;
      }
    };

    struct packet_st
    {
      uint8_t         type{};
      uint8_t         page{};
      uint8_t   retun_flag{};
      uint8_t         obj_id{};
      uint16_t        data_length{};
      uint8_t* data{};
      uint16_t        checksum{};
      uint16_t        checksum_recv{};
      std::array <data_type, CMD_MAX_PACKET_LENGTH> buffer{};
      uint8_t         buffer_idx{};
      uint16_t        data_cnt{};
      uint32_t        resp_ms{};
      machine_step_t    state{};

      packet_st() = default;
      // copy constructor
      packet_st(const packet_st& other) = default;
      // copy assignment
      packet_st& operator=(const packet_st& other) = default;
      // move constructor
      packet_st(packet_st&& other) = default;
      // move assignment
      packet_st& operator=(packet_st&& other) = default;
      ~packet_st() = default;

      uint8_t BufferAdd(uint8_t rx_data)
      {
        buffer[buffer_idx] = rx_data;
        buffer_idx = (buffer_idx + 1) % CMD_MAX_PACKET_LENGTH;
        return buffer_idx;
      }

      void BufferClear()
      {
        buffer.fill(0);
        buffer_idx = 0;
        retun_flag = 0;
        data_cnt = 0;
        checksum = 0xffff;
        state.SetStep(0);
      }


    };


  public:
    bool m_is_open{};
    cfg_t m_cfg{};
    packet_st m_packet{};
    std::function<int(void*, void*)> m_cb{};
    uint8_t m_reqFlag{};
    uint32_t m_packet_sending_ms{};
    std::mutex m_mutex{};


    /****************************************************
    *  Constructor
    ****************************************************/
  public:
    uart_nextion(const ap_core::attribute_st& f) : ap_core::IObj(f) {}
    uart_nextion() = default;
    ~uart_nextion() = default;

    // copy constructor
    uart_nextion(const uart_nextion& other) = default;
    // copy assignment
    uart_nextion& operator=(const uart_nextion& other) = default;
    // move constructor
    uart_nextion(uart_nextion&& other) = default;
    // move assignment
    uart_nextion& operator=(uart_nextion&& other) = default;


    /****************************************************
   *  overriding
   ****************************************************/
    bool write_log(TinyC::Log::level level, ap_core::log_info_t& log, const char* fmt, ...)
    {
      if (fmt)
      {
        va_list args;
        va_start(args, fmt);

        std::string str(1024, '\0');
        int needed = vsnprintf(&str[0], str.size(), fmt, args);
        if (needed < 0)
        {
          // 오류 처리
          LOG_PRINT("va_list is invailed.");
          va_end(args);
          return false;
        }

        //
        if (needed >= str.size())
        {
          str.resize(needed + 1);
          va_start(args, fmt);  
          vsnprintf(&str[0], str.size(), fmt, args);
        }
        else
        {
          str.resize(needed);
        }
        va_end(args);
        writeLog(level, log.file.c_str(), log.func_name.c_str(), log.line_no, str.c_str());
        return true;
      }

      return false;
    }


    /****************************************************
     *  func
     ****************************************************/
    inline errno_t Init(cfg_t& cfg) {
      m_cfg = cfg;

      using uart_t = serial_comm<uint8_t>;
      uart_t::cfg_t serial_cfg{};
      serial_cfg.port_name = m_cfg.port_name;
      serial_cfg.baudrate = m_cfg.baudrate;
      if (m_cfg.ptr_uart->Init(serial_cfg) == ERROR_SUCCESS)
      {

        std::cout << "success open uart" << std::endl;
        //LOG_PRINT("[OK] uart_nextion Init Success! [%s] baud[%d]", m_cfg.port_name, m_cfg.baudrate);
        //this->log->WriteLog(TinyC::Log::lvl_info, 123, this->attribute.logFile_name.c_str(), __FILE__
        //  , __FUNCTION__, __LINE__, "init success : [%d]\n", 10);


        //write_info_log(__FUNCTION__, __FILE__, __LINE__, "init success : [%d]\n", 10);

        ap_core::log_info_t data{ __FUNCTION__, __FILE__, __LINE__ };
        write_log(TinyC::Log::level::lvl_info, data, "init success : [%d]\n", 10);

        m_is_open = true;
        m_cfg.ptr_uart->registerCallback(std::bind(&uart_nextion::UartCallback, this, std::placeholders::_1, std::placeholders::_2));
        return ERROR_SUCCESS;
      }
      else
      {
        LOG_PRINT("[ERR] uart_nextion Init Fail! [%s] baud[%d]", m_cfg.port_name, m_cfg.baudrate);
        m_is_open = false;
      }

      return -1;
    }

    inline bool IsConnected() const {
      return m_is_open;
    }



    inline bool Recovery()
    {
      if (m_cfg.ptr_uart->Recovery())
      {
        LOG_PRINT("recovery success");
        m_is_open = true;
        return true;
      }
      LOG_PRINT("recovery fail");
      m_is_open = false;
      return m_is_open;

    }

    inline bool IsAvailableComm()
    {
      return (m_reqFlag == 0);
    }

    inline bool ReceiveProcess()
    {
      bool ret = false;
      /*
      if (receivePacket())
      {
        receiveCplt();
        ret = true;
      }*/
      return ret;
    }

    inline void registerCallback(std::function<int(void*, void*)> cb)
    {
      m_cb = cb;
    }

    // tx_t별 templete 함수 send_data
    template<typename TxType, typename... Args >
      requires (std::is_same<TxType, type_t>::value
    && types::same_args_c<Args...>)
      inline bool send_data(TxType tx_t, Args... args)
    {

      constexpr int data_length_max = 32;
      std::array<uint8_t, data_length_max> temp = { };
      m_reqFlag = 1;
      bool ret = false;
      auto datas[] = { args... };
      if constexpr (sizeof...(args) == 0)
      {
        switch (tx_t)
        {
        case type_t::TX_LCD_END_REPARSEMODE:
        {
          sprintf((char*)temp.data(), "DRAKJHSUYDGBNCJHGJKSHBDN");
          ret = true;
        }
        break;
        case type_t::TX_LCD_REQUSET_PAGE:
        {
          sprintf((char*)temp.data(), "sendme");
          ret = true;
        }
        break;
        case type_t::TX_LCD_RESET_RECEIVE_CNT:
        {
          sprintf((char*)temp.data(), "nDataID.val=1");
          ret = true;
        }
        break;
        case type_t::TX_OK_RESPONSE:
        {
          sprintf((char*)temp.data(), "isResp.val=1");
          ret = true;
        }
        break;
        default:
          break;
        }
        // end of switch

        return true;
      }
      else if constexpr (sizeof...(args) == 1)
      {
        switch (tx_t)
        {
        case type_t::TX_LCD_BKCMDLEVEL:
          sprintf((char*)temp.data(), "bkcmd=%d", (int)(datas[0]));
          ret = true;
          break;

        case type_t::TX_LCD_START_REPARSEMODE:
          sprintf((char*)temp.data(), "recmod=%d", (int)(datas[0]));
          ret = true;
          break;

        case type_t::TX_LCD_CHANGE_PAGE:
          sprintf((char*)temp.data(), "page %d", (int)(datas[0]));
          ret = true;
          break;
        default:
          break;
        }
        // end of switch
      }
      else if constexpr (sizeof...(args) == 2)
      {

      }
      else if constexpr (sizeof...(args) == 3)
      {
      }
      else
      {
      }

      if (ret)
      {
        uint32_t len = strnlen((const char*)temp.data(), data_length_max);
        temp[len++] = 0xff;
        temp[len++] = 0xff;
        temp[len++] = 0xff;
        m_packet_sending_ms = tim::millis();
        ret = (m_cfg.ptr_uart->Write(temp.data(), len) > 0) ? true : false;
      }

      return ret;
    }

    inline bool  recievePacket(uint8_t rx_data)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      /*

   RX (lcd -> ap) request information or action
   obj_id [option] 0 is all or ignore
   | SOF  | page | type |Obj Id| length |Data(max 120) |   Checksum   | EOF  |
   | :--- |:-----|:-----|:---- | :- --- |:-------------|:-------------| :--  |
   | 0x4A |1 byte|1 byte|1 byte| 1 byte |Data 0～Data n|2 byte(crc 16)| 0x4C |


      */

      enum : uint8_t
      {
        STATE_WAIT_STX, STATE_WAIT_REPLY_PAGE_NO, STATE_WAIT_PAGE_NO,
        STATE_WAIT_TYPE, STATE_WAIT_OBJ_ID, STATE_WAIT_LENGTH,
        STATE_WAIT_DATA, STATE_WAIT_CHECKSUM_L, STATE_WAIT_CHECKSUM_H,
        STATE_WAIT_ETX
      };

      constexpr int timeout_ms = 100;

      if (m_packet.state.MoreThan(timeout_ms))
      {
        m_packet.BufferClear();
      }

      switch (m_packet.state.GetStep())
      {
      case STATE_WAIT_STX:
        /* reply packet*/
        if (rx_data == CMD_RPY_BKCMD_OK)
        {
          m_packet.type = rx_data;
          m_packet.state.SetStep(STATE_WAIT_REPLY_PAGE_NO);
          break;
        }

        if (rx_data == CMD_STX)
        {
          m_packet.BufferClear();
          m_packet.BufferAdd(rx_data);
          m_packet.state.SetStep(STATE_WAIT_PAGE_NO);
        }
        break;

      case STATE_WAIT_REPLY_PAGE_NO:
        m_packet.page = (page_e)(rx_data & 0x0f);
        m_packet.retun_flag = rx_data >> 4;
        m_packet.state.SetStep(STATE_WAIT_STX);
        return true;
        break;

      case STATE_WAIT_PAGE_NO:
        m_packet.page = rx_data;
        m_packet.BufferAdd(rx_data);
        crc::crc16_update(&m_packet.checksum, rx_data);
        m_packet.state.SetStep(STATE_WAIT_TYPE);
        break;

      case STATE_WAIT_TYPE:
        m_packet.type = rx_data;
        m_packet.BufferAdd(rx_data);
        crc::crc16_update(&m_packet.checksum, rx_data);
        m_packet.state.SetStep(STATE_WAIT_OBJ_ID);

        if (rx_data == CMD_RET_CURR_PAGE_NO)
          m_packet.state.SetStep(STATE_WAIT_CHECKSUM_L);
        else if (rx_data == CMD_CTRL_REQ_BEEP)
          return true;
        break;

      case STATE_WAIT_OBJ_ID:
        m_packet.obj_id = rx_data;
        m_packet.BufferAdd(rx_data);
        crc::crc16_update(&m_packet.checksum, rx_data);
        m_packet.state.SetStep(STATE_WAIT_LENGTH);
        break;

      case STATE_WAIT_LENGTH:
        m_packet.data_length = rx_data;
        m_packet.BufferAdd(rx_data);
        crc::crc16_update(&m_packet.checksum, rx_data);
        if (m_packet.data_length == 0)
          m_packet.state.SetStep(STATE_WAIT_CHECKSUM_L);
        else if (m_packet.data_length < (CMD_MAX_DATA_LENGTH + 1))
          m_packet.state.SetStep(STATE_WAIT_DATA);
        else
          m_packet.state.SetStep(STATE_WAIT_STX);
        break;

      case STATE_WAIT_DATA:
        // assign data address
        if (m_packet.data_cnt++ == 0)
          m_packet.data = &m_packet.buffer[m_packet.buffer_idx];

        // check length
        if (m_packet.data_cnt == m_packet.data_length)
          m_packet.state.SetStep(STATE_WAIT_CHECKSUM_L);

        crc::crc16_update(&m_packet.checksum, rx_data);
        m_packet.BufferAdd(rx_data);
        break;

      case STATE_WAIT_CHECKSUM_L:
        m_packet.checksum_recv = rx_data;
        m_packet.state.SetStep(STATE_WAIT_CHECKSUM_H);
        m_packet.BufferAdd(rx_data);
        break;

      case STATE_WAIT_CHECKSUM_H:
        m_packet.BufferAdd(rx_data);
        m_packet.checksum_recv |= (rx_data << 8);
        m_packet.state.SetStep(STATE_WAIT_ETX);
        break;

      case STATE_WAIT_ETX:
        m_packet.state.SetStep(STATE_WAIT_STX);
        if (rx_data == CMD_ETX)
        {
          m_packet.BufferAdd(rx_data);

          if (m_packet.checksum == m_packet.checksum_recv)
            return true;
        }
        break;

      default:
        return false;
      }
      // end of  switch

      return false;
    }


    inline int UartCallback(void* w_parm, void* l_parm)
    {
      if (w_parm == nullptr)
        return -1;
      int length = *((int*)w_parm);
      int index = 0;

      for (index = 0; index < length; index++)
      {
        uint8_t data = *((uint8_t*)l_parm + index);

        {
          if (recievePacket(data))
          {
            receiveCplt();
          }
        }
      }

      return 0;
    }

  private:
    inline void receiveCplt()
    {
      m_reqFlag = 0;
      m_packet.resp_ms = tim::millis() - m_packet_sending_ms;
      if (m_cb)
        m_cb(&m_packet, nullptr);

      /* if (m_func && m_obj)
       {
         m_func(m_obj, &m_packet.retun_flag, &m_packet);
       }*/
    }


  public:
    /****************************************************
     *	test code
     ****************************************************/
    inline int testCB_func(int argc, char* argv[])
    {
      auto get_data = [](char* int_ptr) -> int
        {
          int ret = 0;
          ret = (int)strtoul((const char*)int_ptr, (char**)NULL, (int)0);
          return ret;
        };

      bool ret{};
      enum : int
      {
        arg_cnt_0,
        arg_cnt_1,
        arg_cnt_2,
        arg_cnt_3
      };
      switch ((argc - 1))
      {
      case arg_cnt_1:
      {
        if (argv[1])
        {
          if (std::string(argv[1]).compare("info") == 0)
          {
            std::cout << "lcd info [" << this << "] success " << std::endl;
            ret = true;
          }
        }
      }
      break;
      case arg_cnt_2:
      {
      }
      break;
      case arg_cnt_3:
      {
        if (argv[1])
        {

        }
      }
      break;
      default:
        break;
      }
      // end of switch

      if (ret)
        return 0;

      std::cout << "lcd info" << std::endl;

      return -1;
    }

  };
  // end of class uart_nextion

#endif //_USE_AP_UART_NEXTION
}
// end of namespace ap_core
#endif // AP__INC_UART_NEXTION_HPP_