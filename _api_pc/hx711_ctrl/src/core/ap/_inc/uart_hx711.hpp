/*****************************************************************//**
 * \file   uart_hx711.hpp
 * \brief  
 * 
 * \author gns2l
 * \date   March 2024
 *********************************************************************/

#pragma	once
#ifndef AP__INC_UART_HX711_HPP_
#define AP__INC_UART_HX711_HPP_

#include "../ap_def.hpp"

namespace ap_core
{

  /*

    main(pc), secondary(tool system)

    TX(main->secondary) request information or action
    obj_id[option] 0 is all or ignore
    | STX0 | STX1 | Type | objId | Data Length | Data | Checksum |
    | :---- - | : ---- - | : ------ | : ---- - | : ------------ | : -------- - | : ------  |
    | 0x4A | 0x4C | 2byte | 2byte | 2 byte | Data 0��n | 1byte |


    RX(secondary->main) provide information
    obj_id[option] 0 is all or ignore
    | STX0 | STX1 | Type | objId | Data Length | Data | Checksum |
    | :--- | : ---- - | : ------ | : ---- - | : ------------ | : -------- - | : ------  |
    | 0x4A | 0x4C | 2byte | 2byte | 2 byte | Data 0��n | 1byte |


    uint8_t check_sum = 0;
  for (int i = 0; i < index; i++)
  {
    check_sum += p_cmd->packet.buffer[i];
  }
  check_sum = (~check_sum) + 1;
  p_cmd->packet.buffer[index++] = check_sum;

  */

#ifdef _USE_AP_UART_HX711

	template<typename DataType, std::size_t BufferSize = 4096>
	class uart_hx711 :public ap_core::IComm
	{
  public:
    static constexpr uint8_t CMD_STX0 = 0x4A;
    static constexpr uint8_t CMD_STX1 = 0x4C;

    static constexpr int CMD_MAX_DATA_LENGTH = 120;
    static constexpr int CMD_MAX_PACKET_LENGTH = (CMD_MAX_DATA_LENGTH + 8);

	public:
		using data_type = DataType;
		using uart_t = serial_comm<data_type>;
    struct state_reg_st
    {
      enum state_e : size_t
      {
        STATE_00,
        STATE_01,
        STATE_02,
        STATE_03,
        STATE_04,
        STATE_05,
        STATE_06,
        STATE_07,

        STATE_10,
        STATE_11,
        STATE_12,
        STATE_13,
        STATE_14,
        STATE_15,
        STATE_16,
        STATE_17,
        STATE_MAX
      };

      std::bitset<state_e::STATE_MAX> ap_state;

      bool state_00() const { return ap_state.test(state_e::STATE_00); }
      bool state_01() const { return ap_state.test(state_e::STATE_01); }
      bool state_02() const { return ap_state.test(state_e::STATE_02); }
      bool state_03() const { return ap_state.test(state_e::STATE_03); }
      bool state_04() const { return ap_state.test(state_e::STATE_04); }
      bool state_05() const { return ap_state.test(state_e::STATE_05); }
      bool state_06() const { return ap_state.test(state_e::STATE_06); }
      bool state_07() const { return ap_state.test(state_e::STATE_07); }

      bool state_10() const { return ap_state.test(state_e::STATE_10); }
      bool state_11() const { return ap_state.test(state_e::STATE_11); }
      bool state_12() const { return ap_state.test(state_e::STATE_12); }
      bool state_13() const { return ap_state.test(state_e::STATE_13); }
      bool state_14() const { return ap_state.test(state_e::STATE_14); }
      bool state_15() const { return ap_state.test(state_e::STATE_15); }
      bool state_16() const { return ap_state.test(state_e::STATE_16); }
      bool state_17() const { return ap_state.test(state_e::STATE_17); }
    };

    struct otp_reg_st
    {

      enum option_e : size_t
      {
        OPTION_00,
        OPTION_01,
        OPTION_02,
        OPTION_03,
        OPTION_04,
        OPTION_05,
        OPTION_06,
        OPTION_07,

        OPTION_10,
        OPTION_11,
        OPTION_12,
        OPTION_13,
        OPTION_14,
        OPTION_15,
        OPTION_16,
        OPTION_17,

        OPTION_20,
        OPTION_21,
        OPTION_22,
        OPTION_23,
        OPTION_24,
        OPTION_25,
        OPTION_26,
        OPTION_27,

        OPTION_30,
        OPTION_31,
        OPTION_32,
        OPTION_33,
        OPTION_34,
        OPTION_35,
        OPTION_36,
        OPTION_37, 

        OPTION_MAX
      };

      std::bitset<option_e::OPTION_MAX> ap_option;

      bool option_00() const { return ap_option.test(option_e::OPTION_00); }
      bool option_01() const { return ap_option.test(option_e::OPTION_01); }
      bool option_02() const { return ap_option.test(option_e::OPTION_02); }
      bool option_03() const { return ap_option.test(option_e::OPTION_03); }
      bool option_04() const { return ap_option.test(option_e::OPTION_04); }
      bool option_05() const { return ap_option.test(option_e::OPTION_05); }
      bool option_06() const { return ap_option.test(option_e::OPTION_06); }
      bool option_07() const { return ap_option.test(option_e::OPTION_07); }

      bool option_10() const { return ap_option.test(option_e::OPTION_10); }
      bool option_11() const { return ap_option.test(option_e::OPTION_11); }
      bool option_12() const { return ap_option.test(option_e::OPTION_12); }
      bool option_13() const { return ap_option.test(option_e::OPTION_13); }
      bool option_14() const { return ap_option.test(option_e::OPTION_14); }
      bool option_15() const { return ap_option.test(option_e::OPTION_15); }
      bool option_16() const { return ap_option.test(option_e::OPTION_16); }
      bool option_17() const { return ap_option.test(option_e::OPTION_17); }

      bool option_20() const { return ap_option.test(option_e::OPTION_20); }
      bool option_21() const { return ap_option.test(option_e::OPTION_21); }
      bool option_22() const { return ap_option.test(option_e::OPTION_22); }
      bool option_23() const { return ap_option.test(option_e::OPTION_23); }
      bool option_24() const { return ap_option.test(option_e::OPTION_24); }
      bool option_25() const { return ap_option.test(option_e::OPTION_25); }
      bool option_26() const { return ap_option.test(option_e::OPTION_26); }
      bool option_27() const { return ap_option.test(option_e::OPTION_27); }

      bool option_30() const { return ap_option.test(option_e::OPTION_30); }
      bool option_31() const { return ap_option.test(option_e::OPTION_31); }
      bool option_32() const { return ap_option.test(option_e::OPTION_32); }
      bool option_33() const { return ap_option.test(option_e::OPTION_33); }
      bool option_34() const { return ap_option.test(option_e::OPTION_34); }
      bool option_35() const { return ap_option.test(option_e::OPTION_35); }
      bool option_36() const { return ap_option.test(option_e::OPTION_36); }
      bool option_37() const { return ap_option.test(option_e::OPTION_37); }
    };

    struct err_reg_st
    {
      enum err_e : size_t
      {
        NO_ERR_STATE,
        ERR_01,
        ERR_02,
        ERR_03,
        ERR_04,
        ERR_05,
        ERR_06,
        ERR_07,

        ERR_10,
        ERR_11,
        ERR_12,
        ERR_13,
        ERR_14,
        ERR_15,
        ERR_16,
        ERR_17,

        ERR_20,
        ERR_21,
        ERR_22,
        ERR_23,
        ERR_24,
        ERR_25,
        ERR_26,
        ERR_27,

        ERR_30,
        ERR_31,
        ERR_32,
        ERR_33,
        ERR_34,
        ERR_35,
        ERR_36,
        ERR_37,
        ERR_MAX
      };

      std::bitset<err_e::ERR_MAX> ap_error;

      bool err_01() const { return ap_error.test(err_e::ERR_01); }
      bool err_02() const { return ap_error.test(err_e::ERR_02); }
      bool err_03() const { return ap_error.test(err_e::ERR_03); }
      bool err_04() const { return ap_error.test(err_e::ERR_04); }
      bool err_05() const { return ap_error.test(err_e::ERR_05); }
      bool err_06() const { return ap_error.test(err_e::ERR_06); }
      bool err_07() const { return ap_error.test(err_e::ERR_07); }

      bool err_10() const { return ap_error.test(err_e::ERR_10); }
      bool err_11() const { return ap_error.test(err_e::ERR_11); }
      bool err_12() const { return ap_error.test(err_e::ERR_12); }
      bool err_13() const { return ap_error.test(err_e::ERR_13); }
      bool err_14() const { return ap_error.test(err_e::ERR_14); }
      bool err_15() const { return ap_error.test(err_e::ERR_15); }
      bool err_16() const { return ap_error.test(err_e::ERR_16); }
      bool err_17() const { return ap_error.test(err_e::ERR_17); }

      bool err_20() const { return ap_error.test(err_e::ERR_20); }
      bool err_21() const { return ap_error.test(err_e::ERR_21); }
      bool err_22() const { return ap_error.test(err_e::ERR_22); }
      bool err_23() const { return ap_error.test(err_e::ERR_23); }
      bool err_24() const { return ap_error.test(err_e::ERR_24); }
      bool err_25() const { return ap_error.test(err_e::ERR_25); }
      bool err_26() const { return ap_error.test(err_e::ERR_26); }
      bool err_27() const { return ap_error.test(err_e::ERR_27); }

      bool err_30() const { return ap_error.test(err_e::ERR_30); }
      bool err_31() const { return ap_error.test(err_e::ERR_31); }
      bool err_32() const { return ap_error.test(err_e::ERR_32); }
      bool err_33() const { return ap_error.test(err_e::ERR_33); }
      bool err_34() const { return ap_error.test(err_e::ERR_34); }
      bool err_35() const { return ap_error.test(err_e::ERR_35); }
      bool err_36() const { return ap_error.test(err_e::ERR_36); }
      bool err_37() const { return ap_error.test(err_e::ERR_37); }
    };

    struct io_in_reg_st
    {
      enum in_e : size_t
      {
        IN_00,
        IN_01,
        IN_02,
        IN_03,
        IN_04,
        IN_05,
        IN_06,
        IN_07, //

        IN_10, // RESERVED
        IN_11,
        IN_12,
        IN_13,
        IN_14,
        IN_15,
        IN_16,
        IN_17,

        IN_20, // EXT_IO 1
        IN_21,
        IN_22,
        IN_23,
        IN_24,
        IN_25,
        IN_26,
        IN_27,

        IN_30,
        IN_31,
        IN_32,
        IN_33,
        IN_34,
        IN_35,
        IN_36,
        IN_37,
        IN_MAX

      };

      std::bitset<in_e::IN_MAX> in_reg;

      bool in_00() const { return in_reg.test(in_e::IN_00); }
      bool in_01() const { return in_reg.test(in_e::IN_01); }
      bool in_02() const { return in_reg.test(in_e::IN_02); }
      bool in_03() const { return in_reg.test(in_e::IN_03); }
      bool in_04() const { return in_reg.test(in_e::IN_04); }
      bool in_05() const { return in_reg.test(in_e::IN_05); }
      bool in_06() const { return in_reg.test(in_e::IN_06); }
      bool in_07() const { return in_reg.test(in_e::IN_07); }

      bool in_10() const { return in_reg.test(in_e::IN_10); }
      bool in_11() const { return in_reg.test(in_e::IN_11); }
      bool in_12() const { return in_reg.test(in_e::IN_12); }
      bool in_13() const { return in_reg.test(in_e::IN_13); }
      bool in_14() const { return in_reg.test(in_e::IN_14); }
      bool in_15() const { return in_reg.test(in_e::IN_15); }
      bool in_16() const { return in_reg.test(in_e::IN_16); }
      bool in_17() const { return in_reg.test(in_e::IN_17); }

      bool in_20() const { return in_reg.test(in_e::IN_20); }
      bool in_21() const { return in_reg.test(in_e::IN_21); }
      bool in_22() const { return in_reg.test(in_e::IN_22); }
      bool in_23() const { return in_reg.test(in_e::IN_23); }
      bool in_24() const { return in_reg.test(in_e::IN_24); }
      bool in_25() const { return in_reg.test(in_e::IN_25); }
      bool in_26() const { return in_reg.test(in_e::IN_26); }
      bool in_27() const { return in_reg.test(in_e::IN_27); }

      bool in_30() const { return in_reg.test(in_e::IN_30); }
      bool in_31() const { return in_reg.test(in_e::IN_31); }
      bool in_32() const { return in_reg.test(in_e::IN_32); }
      bool in_33() const { return in_reg.test(in_e::IN_33); }
      bool in_34() const { return in_reg.test(in_e::IN_34); }
      bool in_35() const { return in_reg.test(in_e::IN_35); }
      bool in_36() const { return in_reg.test(in_e::IN_36); }
      bool in_37() const { return in_reg.test(in_e::IN_37); }
    };

    struct io_out_reg_st
    {

      enum out_e : size_t
      {
        OUT_00,
        OUT_01,
        OUT_02,
        OUT_03,
        OUT_04,
        OUT_05,
        OUT_06,
        OUT_07,

        OUT_10, // RESERVED
        OUT_11,
        OUT_12,
        OUT_13,
        OUT_14,
        OUT_15,
        OUT_16,
        OUT_17,

        OUT_20, // EXT_IO 1
        OUT_21,
        OUT_22,
        OUT_23,
        OUT_24,
        OUT_25,
        OUT_26,
        OUT_27,

        OUT_30,
        OUT_31,
        OUT_32,
        OUT_33,
        OUT_34,
        OUT_35,
        OUT_36,
        OUT_37,
        OUT_MAX
      };

      std::bitset<out_e::OUT_MAX> out_reg;

      bool out_00() const { return out_reg.test(out_e::OUT_00); }
      bool out_01() const { return out_reg.test(out_e::OUT_01); }
      bool out_02() const { return out_reg.test(out_e::OUT_02); }
      bool out_03() const { return out_reg.test(out_e::OUT_03); }
      bool out_04() const { return out_reg.test(out_e::OUT_04); }
      bool out_05() const { return out_reg.test(out_e::OUT_05); }
      bool out_06() const { return out_reg.test(out_e::OUT_06); }
      bool out_07() const { return out_reg.test(out_e::OUT_07); }

      bool out_10() const { return out_reg.test(out_e::OUT_10); }
      bool out_11() const { return out_reg.test(out_e::OUT_11); }
      bool out_12() const { return out_reg.test(out_e::OUT_12); }
      bool out_13() const { return out_reg.test(out_e::OUT_13); }
      bool out_14() const { return out_reg.test(out_e::OUT_14); }
      bool out_15() const { return out_reg.test(out_e::OUT_15); }
      bool out_16() const { return out_reg.test(out_e::OUT_16); }
      bool out_17() const { return out_reg.test(out_e::OUT_17); }

      bool out_20() const { return out_reg.test(out_e::OUT_20); }
      bool out_21() const { return out_reg.test(out_e::OUT_21); }
      bool out_22() const { return out_reg.test(out_e::OUT_22); }
      bool out_23() const { return out_reg.test(out_e::OUT_23); }
      bool out_24() const { return out_reg.test(out_e::OUT_24); }
      bool out_25() const { return out_reg.test(out_e::OUT_25); }
      bool out_26() const { return out_reg.test(out_e::OUT_26); }
      bool out_27() const { return out_reg.test(out_e::OUT_27); }

      bool out_30() const { return out_reg.test(out_e::OUT_30); }
      bool out_31() const { return out_reg.test(out_e::OUT_31); }
      bool out_32() const { return out_reg.test(out_e::OUT_32); }
      bool out_33() const { return out_reg.test(out_e::OUT_33); }
      bool out_34() const { return out_reg.test(out_e::OUT_34); }
      bool out_35() const { return out_reg.test(out_e::OUT_35); }
      bool out_36() const { return out_reg.test(out_e::OUT_36); }
      bool out_37() const { return out_reg.test(out_e::OUT_37); }
    };

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
    // RX  (mcu -> pc) provide information
    enum RX_TYPE : uint16_t
    {
      RX_OK_RESPONSE = 0x0000,
      RX_TOOL_DATA = 0x0001,
      RX_TOOL_INFO = 0x0002,
    };

    enum class TX_TYPE : uint16_t
    {
      TX_OK_RESPONSE = 0x0000,
      TX_READ_TOOL_DATA = 0x0001,
      TX_READ_TOOL_INFO = 0x0002,
      //
      TX_CTRL_IO_OUT = 0x0010,
      TX_CTRL_CYL = 0x0011,
      TX_CTRL_VAC = 0x0012,
      TX_CTRL_REG_OPTION = 0x0013,
      //
      TX_CTRL_INITIALIZE = 0x001A,
      TX_CTRL_VIRTUAL_SW = 0x001B,
      //
      TX_CTRL_MOT_ORIGIN = 0x0020,
      TX_CTRL_MOT_ONOFF = 0x0021,
      TX_CTRL_MOT_MOVE = 0x0022,
      TX_CTRL_MOT_STOP = 0x0023,
      TX_CTRL_MOT_JOG = 0x0024,
      TX_CTRL_MOT_LIMIT = 0x0025,
      TX_CTRL_MOT_ZEROSET = 0x0026,
      TX_CTRL_MOT_RELMOVE = 0x0027,
      TX_CTRL_MOT_CLEAR_ALARM = 0x0028,
      TX_CTRL_MOT_CHANGE_VEL = 0x0029,
      TX_CTRL_MOT_MOVE_VEL = 0x002A,
      TX_CTRL_MOT_RELMOVE_VEL = 0x002B,
      TX_CTRL_MOT_VEL_JOG = 0x002C,

      TX_CTRL_TOOL_PnP = 0x0040,
      TX_MODE_FW_DOWNLOAD = 0xF000,
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
    uart_hx711(const ap_core::attribute_st& f) : ap_core::IObj(f) {}
    uart_hx711() = default;
    ~uart_hx711() = default;


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
          LOG_PRINT("va_list args error");
          va_end(args);
          return false;
        }

      
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
        //LOG_PRINT("[OK] uart_hx711 Init Success! [%s] baud[%d]", m_cfg.port_name, m_cfg.baudrate);
        //this->log->WriteLog(TinyC::Log::lvl_info, 123, this->attribute.logFile_name.c_str(), __FILE__
        //  , __FUNCTION__, __LINE__, "init success : [%d]\n", 10);


        //write_info_log(__FUNCTION__, __FILE__, __LINE__, "init success : [%d]\n", 10);

        ap_core::log_info_t data{ __FUNCTION__, __FILE__, __LINE__ };
        write_log(TinyC::Log::level::lvl_info, data, "init success : [%d]\n", 10);

        m_is_open = true;
        m_cfg.ptr_uart->registerCallback(std::bind(&uart_hx711::UartCallback, this, std::placeholders::_1, std::placeholders::_2));
        return ERROR_SUCCESS;
      }
      else
      {
        LOG_PRINT("[ERR] uart_hx711 Init Fail! [%s] baud[%d]", m_cfg.port_name, m_cfg.baudrate);
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

    inline bool  recievePacket(uint8_t rx_data)
    {
      std::lock_guard<std::mutex> lock(m_mutex);

      enum : uint8_t
      {
        STATE_WAIT_STX0,
        STATE_WAIT_STX1,
        STATE_WAIT_TYPE_L,
        STATE_WAIT_TYPE_H,
        STATE_WAIT_OBJ_ID_L,
        STATE_WAIT_OBJ_ID_H,
        STATE_WAIT_LENGTH_L,
        STATE_WAIT_LENGTH_H,
        STATE_WAIT_DATA,
        STATE_WAIT_CHECKSUM
      };

      constexpr int timeout_ms = 100;

      if (m_packet.state.MoreThan(100))
      {
        m_packet.BufferClear();
      }

      switch (m_packet.state.GetStep())
      {
      case STATE_WAIT_STX0:
        if (rx_data == CMD_STX0)
        {
          m_packet.BufferClear();
          m_packet.BufferAdd(rx_data);
          m_packet.state.SetStep(STATE_WAIT_STX1);
        }
        break;

      case STATE_WAIT_STX1:
        if (rx_data == CMD_STX1)
        {
          m_packet.BufferAdd(rx_data);
          m_packet.state.SetStep(STATE_WAIT_TYPE_L);
        }
        break;

      case STATE_WAIT_TYPE_L:
        m_packet.type = rx_data;
        m_packet.BufferAdd(rx_data);
        m_packet.state.SetStep(STATE_WAIT_TYPE_H);
        break;

      case STATE_WAIT_TYPE_H:
        m_packet.type |= (rx_data << 8);
        m_packet.BufferAdd(rx_data);
        m_packet.state.SetStep(STATE_WAIT_OBJ_ID_L);
        break;

      case STATE_WAIT_OBJ_ID_L:
        m_packet.obj_id = rx_data;
        m_packet.BufferAdd(rx_data);
        m_packet.state.SetStep(STATE_WAIT_OBJ_ID_H);
        break;

      case STATE_WAIT_OBJ_ID_H:
        m_packet.obj_id |= (rx_data << 8);
        m_packet.BufferAdd(rx_data);
        m_packet.state.SetStep(STATE_WAIT_LENGTH_L);
        break;

      case STATE_WAIT_LENGTH_L:
        m_packet.data_length = rx_data;
        m_packet.BufferAdd(rx_data);
        m_packet.state.SetStep(STATE_WAIT_LENGTH_H);
        break;

      case STATE_WAIT_LENGTH_H:
        m_packet.data_length |= (rx_data << 8);
        m_packet.BufferAdd(rx_data);

        if (m_packet.data_length == 0)
        {
          m_packet.state.SetStep(STATE_WAIT_CHECKSUM);
        }
        else if (m_packet.data_length < (CMD_MAX_DATA_LENGTH + 1))
        {
          m_packet.state.SetStep(STATE_WAIT_DATA);
        }
        else
          m_packet.state.SetStep(STATE_WAIT_STX0);
        break;

      case STATE_WAIT_DATA:
        // assign data address
        if (m_packet.data_cnt++ == 0)
          m_packet.data = &m_packet.buffer[m_packet.buffer_idx];

        // check length
        if (m_packet.data_cnt == m_packet.data_length)
          m_packet.state.SetStep(STATE_WAIT_CHECKSUM);

        m_packet.BufferAdd(rx_data);
        break;

      case STATE_WAIT_CHECKSUM:
        m_packet.checksum_recv = rx_data;
        m_packet.checksum = (~m_packet.checksum) + 1;
        m_packet.state.SetStep(STATE_WAIT_STX0);
        // std::cout << "STATE_WAIT_CHECKSUM  checksum_recv [0x" << std::hex << (int)m_packet.checksum_recv
        //<<"] ,  cal checksum [0x"<< int(m_packet.checksum) << "] "<<std::endl;
        if (m_packet.checksum == m_packet.checksum_recv)
          return true;
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
            std::cout << "hx711 info [" << this << "] success " << std::endl;
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

      std::cout << "hx711 info" << std::endl;

      return -1;
    }

	};
#endif

} 
// end of namespace ap_core

#endif // AP__INC_UART_HX711_HPP_
