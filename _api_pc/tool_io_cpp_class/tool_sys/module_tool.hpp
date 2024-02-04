#pragma once
#ifndef _MODULE_TOOL_HPP
#define _MODULE_TOOL_HPP

#include "../def.hpp"

class serial_comm;

namespace toolsys
{

  /*

  main(pc), secondary(tool system)

  TX (main -> secondary) request information or action
  obj_id [option] 0 is all or ignore
  | STX0 | STX1 | Type   | objId | Data Length |Data      | Checksum |
  | :----- | :----- | :------ | :----- | :------------   |:---------| :------  |
  | 0x4A | 0x4C | 2byte | 2byte | 2 byte      |Data 0～n | 1byte    |


  RX  (secondary -> main) provide information
  obj_id [option] 0 is all or ignore
  | STX0 | STX1 | Type  | objId | Data Length |Data      | Checksum |
  | :--- |:-----|:------|:----- |:------------|:---------| :------  |
  | 0x4A | 0x4C | 2byte | 2byte | 2 byte      |Data 0～n | 1byte    |


  uint8_t check_sum = 0;
  for (int i=0; i<index; i++)
  {
    check_sum += p_cmd->packet.buffer[i];
  }
  check_sum = (~check_sum) + 1;
  p_cmd->packet.buffer[index++] = check_sum;

  */

  constexpr uint8_t CMD_STX0 = 0x4A;
  constexpr uint8_t CMD_STX1 = 0x4C;

  constexpr int CMD_MAX_DATA_LENGTH   = 120;
  constexpr int CMD_MAX_PACKET_LENGTH = (CMD_MAX_DATA_LENGTH + 8);
  constexpr int PACKET_BUFF_LENGTH    = CMD_MAX_PACKET_LENGTH;

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

  struct mcu_tool_data_align_48_t
  {
  private:
    static constexpr auto io_dword_max    = 1;
    static constexpr auto motor_dword_max = 3;
    static constexpr auto data_dword_max  = 2;

  public:
    enum dat_addr : uint8_t
    {
      addr_system          = 0,
      addr_state           = 2,
      addr_option          = 4,
      addr_err             = 8,
      addr_io_in_0         = 12,
      addr_io_out_0        = 16,
      addr_motor_cnt       = 20,
      addr_motor_reserved  = 22,
      addr_motor_pulse_0   = 24,
      addr_motor_pulse_1   = 28,
      addr_motor_pulse_2   = 32,
      addr_motor_state_0   = 36,
      addr_motor_state_1   = 38,
      addr_motor_state_2   = 40,
      addr_motor_reserved2 = 42,
      addr_datas_0         = 44,
      addr_datas_1         = 46,
    };
    uint16_t                              reg_sys{};         //
    uint16_t                              reg_state{};       //
    uint32_t                              reg_option{};      //
    uint32_t                              reg_err{};         //
    std::array<uint32_t, io_dword_max>    io_in{};           //
    std::array<uint32_t, io_dword_max>    io_out{};          //
    uint16_t                              motor_cnt{};       //
    uint16_t                              motor_reserved{};  // 24
    std::array<uint32_t, motor_dword_max> motor_pulse{};     // 36
    std::array<uint16_t, motor_dword_max> motor_status{};    //
    uint16_t                              motor_reserved2{}; // 44
    std::array<uint16_t, data_dword_max>  datas{};           // 48

    mcu_tool_data_align_48_t()  = default;
    ~mcu_tool_data_align_48_t() = default;
    // copy constructor
    mcu_tool_data_align_48_t(const mcu_tool_data_align_48_t &rhs) = default;
    // copy assignment operator
    mcu_tool_data_align_48_t &operator=(const mcu_tool_data_align_48_t &rhs) = default;
    // move constructor
    mcu_tool_data_align_48_t(mcu_tool_data_align_48_t &&rhs) = default;
    // move assignment operator
    mcu_tool_data_align_48_t &operator=(mcu_tool_data_align_48_t &&rhs) = default;
  };

  class ModuleTool
  {
  public:
    struct cfg_t
    {
      serial_comm *ptr_uart_comm{nullptr};

      cfg_t()  = default;
      ~cfg_t() = default;

      // copy constructor
      cfg_t(const cfg_t &other) = default;
      // copy assignment
      cfg_t &operator=(const cfg_t &other) = default;
      // move constructor
      cfg_t(cfg_t &&other) = default;
      // move assignment
      cfg_t &operator=(cfg_t &&other) = default;
    };

    // RX  (mcu -> pc) provide information
    enum RX_TYPE : uint16_t
    {
      RX_OK_RESPONSE = 0x0000,
      RX_TOOL_DATA   = 0x0001,
      RX_TOOL_INFO   = 0x0002,
    };

    enum class TX_TYPE : uint16_t
    {
      TX_OK_RESPONSE    = 0x0000,
      TX_READ_TOOL_DATA = 0x0001,
      TX_READ_TOOL_INFO = 0x0002,
      //
      TX_CTRL_IO_OUT     = 0x0010,
      TX_CTRL_CYL        = 0x0011,
      TX_CTRL_VAC        = 0x0012,
      TX_CTRL_REG_OPTION = 0x0013,
      //
      TX_CTRL_INITIALIZE = 0x001A,
      TX_CTRL_VIRTUAL_SW = 0x001B,
      //
      TX_CTRL_MOT_ORIGIN      = 0x0020,
      TX_CTRL_MOT_ONOFF       = 0x0021,
      TX_CTRL_MOT_MOVE        = 0x0022,
      TX_CTRL_MOT_STOP        = 0x0023,
      TX_CTRL_MOT_JOG         = 0x0024,
      TX_CTRL_MOT_LIMIT       = 0x0025,
      TX_CTRL_MOT_ZEROSET     = 0x0026,
      TX_CTRL_MOT_RELMOVE     = 0x0027,
      TX_CTRL_MOT_CLEAR_ALARM = 0x0028,
      TX_CTRL_MOT_CHANGE_VEL  = 0x0029,
      TX_CTRL_MOT_MOVE_VEL    = 0x002A,
      TX_CTRL_MOT_RELMOVE_VEL = 0x002B,
      TX_CTRL_MOT_VEL_JOG     = 0x002C,

      TX_CTRL_TOOL_PnP    = 0x0040,
      TX_MODE_FW_DOWNLOAD = 0xF000,
    };

    enum tool_PnP_order_e : int16_t
    {
      TOOL_CMD_ERR_RET          = -1,
      TOOL_CMD_NONE             = 0,
      TOOL_CMD_ORD_MOTOR_ENABLE = 1,
      TOOL_CMD_ORD_MOTOR_DISABLE,
      TOOL_CMD_ORD_MOTOR_RUN,
      TOOL_CMD_ORD_MOTOR_ORG,
      TOOL_CMD_ORD_4,
    };

    struct packet_st
    {
      uint16_t                                   type{};
      uint16_t                                   obj_id{};
      uint16_t                                   data_length{};
      uint8_t                                   *data{};
      uint8_t                                    checksum{};
      uint8_t                                    checksum_recv{};
      std::array<uint8_t, CMD_MAX_PACKET_LENGTH> buffer{};
      uint8_t                                    buffer_idx{};
      uint16_t                                   data_cnt{};
      uint32_t                                   resp_ms{};
      bool                                       wait_resp{};
      step::state_st<>                           state{};
      packet_st() {}

      // copy constructor
      packet_st(const packet_st &other) = default;

      // copy assignment
      packet_st &operator=(const packet_st &other) = default;

      // move constructor
      packet_st(packet_st &&other) = default;

      // move assignment

      packet_st &operator=(packet_st &&other) = default;
      // destructor
      ~packet_st() = default;

      uint8_t BufferAdd(uint8_t rx_data)
      {
        checksum           += rx_data;
        buffer[buffer_idx]  = rx_data;
        buffer_idx          = (buffer_idx + 1) % PACKET_BUFF_LENGTH;
        return buffer_idx;
      }

      void BufferClear()
      {
        buffer.fill(0);
        buffer_idx = 0;
        data_cnt   = 0;
        checksum   = 0;
        state.SetStep(0);
      }
    };

    struct tools_info_st
    {
      // name
      std::string name{};
    };

    bool  m_is_init;
    cfg_t m_cfg;

    err_reg_st    m_errorReg;
    otp_reg_st    m_optionReg;
    state_reg_st  m_stateReg;
    io_in_reg_st  m_inReg;
    io_out_reg_st m_outReg;

    packet_st m_packetData;

    std::thread                        m_trd;
    std::atomic<bool>                  m_stopThread;
    std::function<int(void *, void *)> m_cb;
    std::mutex                         m_mutex;
    uint32_t                           m_msgTime;
    tools_info_st                      m_toolsInfo;
    mcu_tool_data_align_48_t           m_toolData{};

  public:
    ModuleTool() : m_is_init{}, m_cfg{}, m_errorReg{}, m_optionReg{}, m_stateReg{}, m_inReg{}, m_outReg{}, m_packetData{}, m_trd{}, m_stopThread{}, m_cb{}, m_mutex{}, m_msgTime{}, m_toolsInfo{}, m_toolData{}
    {
    }

    ~ModuleTool()
    {
    }

  private:
    /// @brief  parsing process
    inline void processCplt()
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::cout << "RxData.Type (resp time): " << std::hex << (int)m_packetData.type << std::dec << "(" << m_packetData.resp_ms << ")" << std::endl;
      RX_TYPE rx_type = (RX_TYPE)m_packetData.type;

      auto putData = [&](const mcu_tool_data_align_48_t &data)
      {
        m_errorReg.ap_error   = data.reg_err;
        m_optionReg.ap_option = data.reg_option;
        m_stateReg.ap_state   = data.reg_state;
        m_inReg.in_reg        = data.io_in[0];
        m_outReg.out_reg      = data.io_out[0];
      };

      std::cout << std::dec;
      switch (rx_type)
      {
      case RX_TYPE::RX_OK_RESPONSE:
        break;
      case RX_TYPE::RX_TOOL_DATA:
        m_toolData = *((mcu_tool_data_align_48_t *)m_packetData.data);
        std::cout << "tool data (word ) reg_sys : " << m_toolData.reg_sys << std::endl;
        std::cout << "tool data (word ) reg_state : " << m_toolData.reg_state << std::endl;
        std::cout << "tool data (dword) reg_option : " << m_toolData.reg_option << std::endl;
        std::cout << "tool data (dword) reg_err : " << m_toolData.reg_err << std::endl;
        std::cout << "tool data (dword) io_in[0] : " << m_toolData.io_in[0] << std::endl;
        std::cout << "tool data (dword) io_out[0] : " << m_toolData.io_out[0] << std::endl;
        std::cout << "tool data (word ) motor_cnt : " << m_toolData.motor_cnt << std::endl;
        std::cout << "tool data (dword) motor_pos[0] : " << m_toolData.motor_pulse[0] << std::endl;
        std::cout << "tool data (word ) motor_status[0] : " << m_toolData.motor_status[0] << std::endl;
        std::cout << "tool data (dword) motor_pos[1] : " << m_toolData.motor_pulse[1] << std::endl;
        std::cout << "tool data (word ) motor_status[1] : " << m_toolData.motor_status[1] << std::endl;
        std::cout << "tool data (dword) motor_pos[2] : " << m_toolData.motor_pulse[2] << std::endl;
        std::cout << "tool data (word ) motor_status[2] : " << m_toolData.motor_status[2] << std::endl;
        std::cout << "tool data (word) datas[0] : " << m_toolData.datas[0] << std::endl;
        std::cout << "tool data (word) datas[1] : " << m_toolData.datas[1] << std::endl;

        putData(m_toolData);
        break;
      case RX_TYPE::RX_TOOL_INFO:
      {
        std::string name{m_packetData.data, m_packetData.data + m_packetData.data_length};
        std::cout << "name : " << name << std::endl;
        m_toolsInfo.name = name;
      }
      break;

      default:
        break;
      }
    }

    inline int UartCallback(void *w_parm, void *l_parm)
    {
      if (w_parm == nullptr)
        return -1;
      int length = *((int *)w_parm);
      int index  = 0;

      for (index = 0; index < length; index++)
      {
        uint8_t data = *((uint8_t *)l_parm + index);
        if (receivePacket(data))
        {
          m_packetData.wait_resp = false;
          m_packetData.resp_ms   = tim::millis() - m_msgTime;
          processCplt();
        }
      }

      return 0;
    }

    inline bool receivePacket(uint8_t rx_data)
    {

      // std::cout << "rx_data [0x" << std::hex << (int)rx_data << "]"<< std::endl;
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

      if (m_packetData.state.MoreThan(100))
      {
        m_packetData.BufferClear();
      }

      switch (m_packetData.state.GetStep())
      {
      case STATE_WAIT_STX0:
        if (rx_data == CMD_STX0)
        {
          m_packetData.BufferClear();
          m_packetData.BufferAdd(rx_data);
          m_packetData.state.SetStep(STATE_WAIT_STX1);
        }
        break;

      case STATE_WAIT_STX1:
        if (rx_data == CMD_STX1)
        {
          m_packetData.BufferAdd(rx_data);
          m_packetData.state.SetStep(STATE_WAIT_TYPE_L);
        }
        break;

      case STATE_WAIT_TYPE_L:
        m_packetData.type = rx_data;
        m_packetData.BufferAdd(rx_data);
        m_packetData.state.SetStep(STATE_WAIT_TYPE_H);
        break;

      case STATE_WAIT_TYPE_H:
        m_packetData.type |= (rx_data << 8);
        m_packetData.BufferAdd(rx_data);
        m_packetData.state.SetStep(STATE_WAIT_OBJ_ID_L);
        break;

      case STATE_WAIT_OBJ_ID_L:
        m_packetData.obj_id = rx_data;
        m_packetData.BufferAdd(rx_data);
        m_packetData.state.SetStep(STATE_WAIT_OBJ_ID_H);
        break;

      case STATE_WAIT_OBJ_ID_H:
        m_packetData.obj_id |= (rx_data << 8);
        m_packetData.BufferAdd(rx_data);
        m_packetData.state.SetStep(STATE_WAIT_LENGTH_L);
        break;

      case STATE_WAIT_LENGTH_L:
        m_packetData.data_length = rx_data;
        m_packetData.BufferAdd(rx_data);
        m_packetData.state.SetStep(STATE_WAIT_LENGTH_H);
        break;

      case STATE_WAIT_LENGTH_H:
        m_packetData.data_length |= (rx_data << 8);
        m_packetData.BufferAdd(rx_data);

        if (m_packetData.data_length == 0)
        {
          m_packetData.state.SetStep(STATE_WAIT_CHECKSUM);
        }
        else if (m_packetData.data_length < (CMD_MAX_DATA_LENGTH + 1))
        {
          m_packetData.state.SetStep(STATE_WAIT_DATA);
        }
        else
          m_packetData.state.SetStep(STATE_WAIT_STX0);
        break;

      case STATE_WAIT_DATA:
        // assign data address
        if (m_packetData.data_cnt++ == 0)
          m_packetData.data = &m_packetData.buffer[m_packetData.buffer_idx];

        // check length
        if (m_packetData.data_cnt == m_packetData.data_length)
          m_packetData.state.SetStep(STATE_WAIT_CHECKSUM);

        m_packetData.BufferAdd(rx_data);
        break;

      case STATE_WAIT_CHECKSUM:
        m_packetData.checksum_recv = rx_data;
        m_packetData.checksum      = (~m_packetData.checksum) + 1;
        m_packetData.state.SetStep(STATE_WAIT_STX0);
        // std::cout << "STATE_WAIT_CHECKSUM  checksum_recv [0x" << std::hex << (int)m_packetData.checksum_recv
        //<<"] ,  cal checksum [0x"<< int(m_packetData.checksum) << "] "<<std::endl;
        if (m_packetData.checksum == m_packetData.checksum_recv)
          return true;
        break;

      default:
        return false;
      }
      // end of  switch

      return false;
    }

  public:
    inline errno_t Init(const cfg_t &cfg)
    {
      m_cfg = cfg;
      // serial_comm::cfg_t comm_cfg{};
      m_cfg.ptr_uart_comm->registerCallback(std::bind(&ModuleTool::UartCallback, this, std::placeholders::_1, std::placeholders::_2));
      if (m_cfg.ptr_uart_comm->IsOpen())
      {
        m_is_init = true;
        return ERROR_SUCCESS;
      }
      else
        return m_cfg.ptr_uart_comm->OpenPort();
    }

    inline errno_t SendCmd(uint8_t *p_data, uint32_t length)
    {
      if (m_packetData.wait_resp)
        return -1;

      /*
       | STX0 | STX1 | Type  | objId | Data Length |Data      | Checksum |
       | :--- |:-----|:------|:----- |:------------|:---------| :------  |
       | 0x4A | 0x4C | 2byte | 2byte | 2 byte      |Data 0～n | 1byte    |
      */

      std::vector<uint8_t> datas{};
      for (uint32_t i = 0; i < length; i++)
        datas.emplace_back(p_data[i]);

      uint8_t checksum = 0;
      for (const auto &elm : datas)
        checksum += elm;
      checksum = (~checksum) + 1;
      datas.emplace_back(checksum);

      std::cout << "TxData.Data: " << std::hex;
      for (const auto &elm : datas)
        std::cout << (int)elm << " ";
      std::cout << std::endl;

      m_msgTime = tim::millis();
      if (m_cfg.ptr_uart_comm->SendData((char *)datas.data(), (uint32_t)datas.size()) > 0)
        return ERROR_SUCCESS;
      else
        return -1;
    }

    inline bool SendCmdRxResp(uint8_t *p_data, uint32_t length, uint32_t timeout = 200)
    {
      if (SendCmd(p_data, length) == ERROR_SUCCESS)
      {
        uint32_t pre_ms        = tim::millis();
        m_packetData.wait_resp = true;
        while (m_packetData.wait_resp)
        {
          if ((tim::millis() - pre_ms) > timeout)
          {
            ERR_PRINT("SendCmdRxResp timeout!");
            m_packetData.wait_resp = false;
            return false;
          }

          // Sleep(50);
          tim::delay(1);
        }
        // m_commRespTime = tim::millis() - pre_ms;
        // std::cout << "Response ms : " << std::to_string(m_commRespTime) << std::endl;
        return true;
      }
      return false;
    }

    inline errno_t SendData(TX_TYPE type)
    {
      return 0;
    }

    template <typename Type, typename... ElementType>
    errno_t SendOrder(Type type, ElementType... elements)
    {
      if constexpr (std::is_same_v<Type, TX_TYPE>)
      {
        std::array<uint8_t, CMD_MAX_PACKET_LENGTH> datas = {};
        enum : uint8_t
        {
          idx_stx0,
          idx_stx1,
          idx_type_l,
          idx_type_h,
          idx_objid_l,
          idx_objid_h,
          idx_length_l,
          idx_length_h,
          idx_data
        };

        // constexpr uint16_t obj_id   = 0x0000;
        uint16_t length    = 0;
        uint16_t cmd_type  = static_cast<uint16_t>(type);
        datas[idx_stx0]    = CMD_STX0;
        datas[idx_stx1]    = CMD_STX1;
        datas[idx_type_l]  = LoBYTE(cmd_type);
        datas[idx_type_h]  = HiBYTE(cmd_type);
        datas[idx_objid_l] = 0;
        datas[idx_objid_h] = 0;
        if constexpr (sizeof...(elements) == 0)
        {
          switch (type)
          {
          case TX_TYPE::TX_OK_RESPONSE:      __fallthrough;
          case TX_TYPE::TX_READ_TOOL_DATA:   __fallthrough;
          case TX_TYPE::TX_READ_TOOL_INFO:   __fallthrough;
          case TX_TYPE::TX_CTRL_INITIALIZE:
            __fallthrough;
            break;

          default:
            LOG_PRINT("Error! Wrong type.");
            return -1;
            break;
          }
        }
        else
        {
          using ele_t                                   = std::common_type_t<ElementType...>;
          std::array<ele_t, sizeof...(elements)> values = {elements...};

          auto set_data = [&packet = datas](auto offset, auto src)
          {
            std::memcpy(&packet[idx_data + offset], &src, sizeof(src));
            return sizeof(src);
          };

          if constexpr (sizeof...(elements) == 1)
          {

            switch (type)
            {
            case TX_TYPE::TX_CTRL_IO_OUT:          __fallthrough;
            case TX_TYPE::TX_CTRL_VIRTUAL_SW:      __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_STOP:        __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_ZEROSET:     __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_CLEAR_ALARM: __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_ORIGIN:      __fallthrough;
            case TX_TYPE::TX_MODE_FW_DOWNLOAD:     __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_LIMIT:       
              // length = sizeof(int);
              // std::memcpy(&datas[idx_data], (uint8_t *)&arr[0], length);
              length = set_data(0, (uint32_t)values[0]);
              break;

            case TX_TYPE::TX_CTRL_REG_OPTION:
              length = set_data(0, (uint16_t)values[0]);
              break;

            default:
              LOG_PRINT("Error! Wrong type.");
              return -1;
              break;
            }
          }
          else if constexpr (sizeof...(elements) == 2)
          {
            switch (type)
            {
            case TX_TYPE::TX_CTRL_MOT_MOVE:    __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_RELMOVE: __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_ONOFF:   __fallthrough;
            case TX_TYPE::TX_CTRL_MOT_JOG:
            {
              enum : uint8_t
              {
                _objid,
                _data1
              };
              length  = set_data(length, (uint16_t)values[_objid]);
              length += set_data(length, (int)values[_data1]);
            }
            break;
            default:
              LOG_PRINT("Error! Wrong type.");
              return -1;
            }            
          }
          else if constexpr (sizeof...(elements) == 3)
          {
          }
          else if constexpr (sizeof...(elements) == 4)
          {
            if (type == TX_TYPE::TX_CTRL_TOOL_PnP)
            {
              enum : uint8_t
              {
                _order,
                _objid,
                _data1,
                _datat2
              };
              length  = set_data(0, (uint16_t)values[_order]);
              length += set_data(length, (uint16_t)values[_objid]);
              length += set_data(length, (int)values[_data1]);
              length += set_data(length, (int)values[_datat2]);
            }
            else
            {
              LOG_PRINT("Error! Wrong type.");
              return -1;
            }
          }
          else
          {
            LOG_PRINT("Error! Too many arguments.");
            return -1;
          }
        }

        datas[idx_length_l] = (uint8_t)(length >> 0);
        datas[idx_length_h] = (uint8_t)(length >> 8);

        uint32_t idx      = (uint32_t)idx_data + length;
        uint8_t  checksum = 0;
        for (const auto &elm : datas)
          checksum += elm;
        checksum   = (~checksum) + 1;
        datas[idx] = checksum;

        length = idx + 1;

        return (SendCmdRxResp(datas.data(), length) ? ERROR_SUCCESS : -1);
      }
      else
      {
        LOG_PRINT("Error! Wrong type.");
        return -1;
      }

      return -1;
    }

    inline errno_t Ret_OkResponse()
    {
      return SendOrder(TX_TYPE::TX_OK_RESPONSE);
    }

#if 0

    inline errno_t GetToolsInfo()
    {
      std::vector<uint8_t> datas{};
      datas.emplace_back((uint8_t)(TX_READ_TOOL_INFO >> 0));
      datas.emplace_back((uint8_t)(TX_READ_TOOL_INFO >> 8));
      datas.emplace_back(0x00); // obj id l
      datas.emplace_back(0x00); // obj id h
      datas.emplace_back(0x00); // data length l
      datas.emplace_back(0x00); // data length h

      return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
    }

    inline errno_t GetToolsData()
    {
      std::vector<uint8_t> datas{};
      datas.emplace_back((uint8_t)(TX_READ_TOOL_DATA >> 0));
      datas.emplace_back((uint8_t)(TX_READ_TOOL_DATA >> 8));
      datas.emplace_back(0x00); // obj id l
      datas.emplace_back(0x00); // obj id h
      datas.emplace_back(0x00); // data length l
      datas.emplace_back(0x00); // data length h

      return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
    }

    inline errno_t SetIO_Output(uint32_t out_reg)
    {
      std::vector<uint8_t> datas{};
      datas.emplace_back((uint8_t)(TX_CTRL_IO_OUT >> 0));
      datas.emplace_back((uint8_t)(TX_CTRL_IO_OUT >> 8));
      datas.emplace_back(0x00);                            // obj id l
      datas.emplace_back(0x00);                            // obj id h
      datas.emplace_back((uint8_t)(sizeof(out_reg) >> 0)); // data length l
      datas.emplace_back((uint8_t)(sizeof(out_reg) >> 8)); // data length h

      datas.emplace_back((uint8_t)(out_reg >> 0)); // data
      datas.emplace_back((uint8_t)(out_reg >> 8));
      datas.emplace_back((uint8_t)(out_reg >> 16));
      datas.emplace_back((uint8_t)(out_reg >> 24));

      if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
        return ERROR_SUCCESS;
      else
        return -1;
    }

    inline errno_t SetOptionReg(uint16_t option_reg)
    {
      std::vector<uint8_t> datas{};
      datas.emplace_back((uint8_t)(TX_CTRL_REG_OPTION >> 0));
      datas.emplace_back((uint8_t)(TX_CTRL_REG_OPTION >> 8));
      datas.emplace_back(0x00);                               // obj id l
      datas.emplace_back(0x00);                               // obj id h
      datas.emplace_back((uint8_t)(sizeof(option_reg) >> 0)); // data length l
      datas.emplace_back((uint8_t)(sizeof(option_reg) >> 8)); // data length h

      datas.emplace_back((uint8_t)(option_reg >> 0)); // data
      datas.emplace_back((uint8_t)(option_reg >> 8));

      if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
        return ERROR_SUCCESS;
      else
        return -1;
    }

    
    inline errno_t SetModeFwDownloadr()
    {
      std::vector<uint8_t> datas{};

      uint32_t magic_number = 0x5555AAAA;
      datas.emplace_back(LoBYTE(TX_MODE_FW_DOWNLOAD));
      datas.emplace_back(HiBYTE(TX_MODE_FW_DOWNLOAD));

      datas.emplace_back(0);                                    // obj id l
      datas.emplace_back(0);                                    // obj id h
                                                                // length
      datas.emplace_back((uint8_t)(sizeof(magic_number) >> 0)); // data length l
      datas.emplace_back((uint8_t)(sizeof(magic_number) >> 8)); // data length h
      // data
      datas.emplace_back((uint8_t)(magic_number >> 0));
      datas.emplace_back((uint8_t)(magic_number >> 8));
      datas.emplace_back((uint8_t)(magic_number >> 16));
      datas.emplace_back((uint8_t)(magic_number >> 24));

      return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
    }

    inline errno_t SetPnPToolOrder(tool_PnP_order_e order, uint16_t obj_idx = 0, int data_1 = 0, int data_2 = 0)
    {
      std::vector<uint8_t> datas{};
      enum
      {
        ord1,
        ord2,
        data11,
        data12,
        data13,
        data14,
        data21,
        data22,
        data23,
        data24,
        _max
      };      

      datas.emplace_back(LoBYTE(TX_CTRL_TOOL_PnP));
      datas.emplace_back(HiBYTE(TX_CTRL_TOOL_PnP));

      datas.emplace_back(LoBYTE(obj_idx)); // obj id l
      datas.emplace_back(HiBYTE(obj_idx)); // obj id h
                                          // length
      datas.emplace_back(LoBYTE(_max));   // data length l
      datas.emplace_back(HiBYTE(_max));   // data length h
      // data
      datas.emplace_back(LoBYTE(order));
      datas.emplace_back(HiBYTE(order));
      datas.emplace_back(uint8_t(data_1 >> (8 * 0)));
      datas.emplace_back(uint8_t(data_1 >> (8 * 1)));
      datas.emplace_back(uint8_t(data_1 >> (8 * 2)));
      datas.emplace_back(uint8_t(data_1 >> (8 * 3)));
      datas.emplace_back(uint8_t(data_2 >> (8 * 0)));
      datas.emplace_back(uint8_t(data_2 >> (8 * 1)));
      datas.emplace_back(uint8_t(data_2 >> (8 * 2)));
      datas.emplace_back(uint8_t(data_2 >> (8 * 3)));

      return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
    }

#endif

    ///////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////
    inline int testCB_func(int argc, char *argv[])
    {
      auto get_data = [](char *int_ptr) -> int
      {
        int ret_idx = 0;
        ret_idx     = (int)strtoul((const char *)int_ptr, (char **)NULL, (int)0);
        return ret_idx;
      };

      bool ret{};
      enum : int
      {
        arg_cnt_0,
        arg_cnt_1,
        arg_cnt_2,
        arg_cnt_3,
        arg_cnt_4,
      };

      switch ((argc - 1))
      {
      case arg_cnt_1:
      {
        if (argv[1])
        {
          if (std::string(argv[1]).compare("info") == 0)
          {
            if (this->SendOrder(TX_TYPE::TX_READ_TOOL_INFO) == ERROR_SUCCESS)
            {
              std::cout << "tools info [" << this << "] success " << std::endl;
              ret = true;
            }
            else
              std::cout << "tools get info fail" << std::endl;
          }
          else if (std::string(argv[1]).compare("data") == 0)
          {
            if (this->SendOrder(TX_TYPE::TX_READ_TOOL_DATA) == ERROR_SUCCESS)
            {
              std::cout << "tools data [" << this << "] success " << std::endl;
              ret = true;
            }
            else
              std::cout << "tools get data fail" << std::endl;
          }
          else if (std::string(argv[1]).compare("mode_fw") == 0)
          {
            constexpr uint32_t FLASH_MAGIC_NUMBER  =     0x5555AAAA;
            if (this->SendOrder(TX_TYPE::TX_MODE_FW_DOWNLOAD, FLASH_MAGIC_NUMBER) == ERROR_SUCCESS)
            {
              std::cout << "tools mode fw [" << this << "] success " << std::endl;
              ret = true;
            }
            else
              std::cout << "tools mode fw fail" << std::endl;
          }
        }
      }
      break;
      case arg_cnt_2:
      {
        if (argv[1])
        {
          if (std::string(argv[1]).compare("on_io") == 0)
          {
            uint8_t out_idx = get_data(argv[2]);
            if (out_idx < 0 || out_idx > 15)
              break;
            std::bitset<32> output = this->m_outReg.out_reg;
            // std::cout << "on_io before [" << std::to_string((uint32_t)output.to_ulong()) << "] set" << std::endl;

            output.set(out_idx, true);
            // std::cout << "on_io set after [" << std::to_string((uint32_t)output.to_ulong()) << "] set" << std::endl;

            if (this->SendOrder(TX_TYPE::TX_CTRL_IO_OUT, (uint32_t)output.to_ulong()) == ERROR_SUCCESS)
            {
              std::cout << "on_io [" << std::to_string(out_idx) << "] success " << std::endl;
              this->SendOrder(TX_TYPE::TX_READ_TOOL_DATA);
              ret = true;
            }
            else
              std::cout << "on_io fail" << std::endl;
          }
        }
      }
      break;
      case arg_cnt_3:
      {
        if (argv[1])
        {
          if (std::string(argv[1]).compare("io_out") == 0)
          {
            uint8_t out_idx = get_data(argv[2]);
            if (out_idx < 0 || out_idx > 15)
              break;
            std::bitset<32> output = this->m_outReg.out_reg;
            if (std::string(argv[3]).compare("off") == 0)
            {
              output.set(out_idx, false);
              ret = true;
            }
            else if (std::string(argv[3]).compare("on") == 0)
            {
              output.set(out_idx, true);
              ret = true;
            }
            else
            {
              std::cout << "set output fail" << std::endl;
            }

            if (ret)
            {
              if (this->SendOrder(TX_TYPE::TX_CTRL_IO_OUT, (uint32_t)output.to_ulong()) == ERROR_SUCCESS)
              {
                std::cout << "set io output [" << std::to_string(out_idx) << "] success " << std::endl;
                this->SendOrder(TX_TYPE::TX_READ_TOOL_DATA);
              }
              else
                std::cout << "set output fail" << std::endl;
            }
          }
          else if (std::string(argv[1]).compare("option") == 0)
          {
            uint8_t reg_idx = get_data(argv[2]);

            if (reg_idx < 0 || reg_idx > 15)
              break;

            std::bitset<32> opt_bit = this->m_optionReg.ap_option;
            if (std::string(argv[3]).compare("off") == 0)
            {
              opt_bit.set(reg_idx, false);
              ret = true;
            }
            else if (std::string(argv[3]).compare("on") == 0)
            {
              opt_bit.set(reg_idx, true);
              ret = true;
            }
            else
            {
              std::cout << "set option fail" << std::endl;
            }

            if (ret)
            {
              if (this->SendOrder(TX_TYPE::TX_CTRL_REG_OPTION, (uint16_t)opt_bit.to_ulong()) == ERROR_SUCCESS)
              {
                std::cout << "set option [" << std::to_string(reg_idx) << "] success " << std::endl;
                this->SendOrder(TX_TYPE::TX_READ_TOOL_DATA);
              }
              else
                std::cout << "set option fail" << std::endl;
            }
          }
        }
      }
      break;
      case arg_cnt_4:
      {
        if (argv[1])
        {
          if (std::string(argv[1]).compare("pnp") == 0)
          {
          }
        }
      }
      break;
      default:
        break;
      }
      // end of switch

      if (argv[1])
      {
        if (std::string(argv[1]).compare("pnp") == 0)
        {
          if ((argc - 1) == arg_cnt_1)
          {
          }
          else if ((argc - 1) == arg_cnt_3)
          {
            uint8_t idx = get_data(argv[3]);
            if (std::string(argv[2]).compare("org") == 0)
            {
              if (this->SendOrder(TX_TYPE::TX_CTRL_TOOL_PnP, TOOL_CMD_ORD_MOTOR_ORG, idx) == ERROR_SUCCESS)
              {
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_ORG  success " << std::endl;
                ret = true;
              }
              else
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_ORG fail" << std::endl;
            }
            else if (std::string(argv[2]).compare("motor_on") == 0)
            {
              if (this->SendOrder(TX_TYPE::TX_CTRL_TOOL_PnP, TOOL_CMD_ORD_MOTOR_ENABLE, idx) == ERROR_SUCCESS)
              {
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_ENABLE  success " << std::endl;
                ret = true;
              }
              else
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_ENABLE fail" << std::endl;
            }
            else if (std::string(argv[2]).compare("motor_off") == 0)
            {
              if (this->SendOrder(TX_TYPE::TX_CTRL_TOOL_PnP, TOOL_CMD_ORD_MOTOR_DISABLE, idx) == ERROR_SUCCESS)
              {
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_DISABLE  success " << std::endl;
                ret = true;
              }
              else
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_DISABLE fail" << std::endl;
            }
          }
          else if ((argc - 1) == arg_cnt_4)
          {
            uint8_t idx = get_data(argv[3]);
            if (std::string(argv[2]).compare("run") == 0)
            {
              int cnt = get_data(argv[4]);
              if (this->SendOrder(TX_TYPE::TX_CTRL_TOOL_PnP, TOOL_CMD_ORD_MOTOR_RUN, idx, cnt) == ERROR_SUCCESS)
              {
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_RUN  success " << std::endl;
                ret = true;
              }
              else
                std::cout << "xyio TOOL_CMD_ORD_MOTOR_RUN fail" << std::endl;
            }
          }
        }
      }
      // end of pnp

      if (ret)
        return 0;

      std::cout << "tools info" << std::endl;
      std::cout << "tools data" << std::endl;
      std::cout << "tools mode_fw" << std::endl;
      std::cout << "tools on_io idx[0:15]" << std::endl;
      std::cout << "tools io_out idx[0:15] [on/off]" << std::endl;
      std::cout << "tools pnp org idx[0:2 3-all]" << std::endl;
      std::cout << "tools pnp motor_on idx[0:2 3-all]" << std::endl;
      std::cout << "tools pnp motor_off idx[0:2 3-all]" << std::endl;
      std::cout << "tools pnp run idx[0:2 3-all] [cnt]" << std::endl;

      return -1;
    }
  };
  // end of class ModuleTool
}
// end of namespace

#endif
// end of _MODULE_TOOL_HPP