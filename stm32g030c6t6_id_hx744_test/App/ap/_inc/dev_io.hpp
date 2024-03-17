/*
 * dev_io.hpp
 *
 *  Created on: 2023. 11. 17.
 *      Author: gns2.lee
 */

#ifndef AP__INC_DEV_IO_HPP_
#define AP__INC_DEV_IO_HPP_

#include "ap_def.hpp"

struct dev_io
{
  /****************************************************
   *  data
   ****************************************************/
  struct cfg_t
  {
    ap_reg *ptr_mcu_reg{};
    ap_io *ptr_io{};

    CMD::uart_cmd *ptr_comm{};
    mcu_io_align_12_t *ptr_mcu_data{};

    cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t &rhs) = default;
    // copy assignment operator
    cfg_t &operator=(const cfg_t &rhs) = default;
    // move constructor
    cfg_t(cfg_t &&rhs) = default;
    // move assignment operator
    cfg_t &operator=(cfg_t &&rhs) = default;

  } m_cfg{};
  CMD::uart_cmd::packet_st m_receiveData{};

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  dev_io() = default;
  ~dev_io() = default;
  /****************************************************
   *  func
   ****************************************************/
private:
  // callback function
  inline static void receiveDataFunc(void *obj, void *w_parm, void *l_parm)
  {
    dev_io *ptr_this = (dev_io *)obj;

    if (w_parm == nullptr && obj == nullptr && l_parm == nullptr)
      return;
    ptr_this->ProcessCmd(*(CMD::uart_cmd::packet_st *)l_parm);
  }

public:
  inline error_t Init(cfg_t &cfg)
  {
    cfg.ptr_comm->AttCallbackFunc(this, receiveDataFunc);
    LOG_PRINT("[OK] Init Success! ");
    m_cfg = cfg;
    return ERROR_SUCCESS;
  }

  // Retry the communication connection
  inline void CommRecovery()
  {
    m_cfg.ptr_comm->Recovery();
  }

  inline bool IsConnected()
  {
    return m_cfg.ptr_comm->IsConnected();
  }

  inline errno_t SendData(CMD::TX_TYPE type)
  {
    std::array<uint8_t, CMD::uart_cmd::CMD_MAX_PACKET_LENGTH> datas = {};
    /*
     | STX0 | STX1 | Type  | objId | Data Length |Data      | Checksum |
     | :--- |:-----|:------|:----- |:------------|:---------| :------  |
     | 0x4A | 0x4C | 2byte | 2byte | 2 byte      |Data 0～n | 1byte    |
    */

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

    constexpr uint16_t obj_id = 0x0000;
    uint16_t length = 0;
    datas[idx_stx0] = CMD::uart_cmd::CMD_STX0;
    datas[idx_stx1] = CMD::uart_cmd::CMD_STX1;
    datas[idx_type_l] = (uint8_t)(type >> 0);
    datas[idx_type_h] = (uint8_t)(type >> 8);
    datas[idx_objid_l] = (uint8_t)(obj_id >> 0);
    datas[idx_objid_h] = (uint8_t)(obj_id >> 8);
    constexpr const char *tools_info = _DEF_FW_NAME;
    switch (type)
    {
    case CMD::TX_OK_RESPONSE:
      break;

    case CMD::TX_TOOL_INFO:
      length = (uint16_t)strlen(tools_info);
      // LOG_PRINT("TYPE_TOOL_INFO length[%d]", length);
      std::memcpy(&datas[idx_data], (uint8_t *)tools_info, length);
      break;

    case CMD::TX_TOOL_DATA:
      length = sizeof(*(m_cfg.ptr_mcu_data));
      std::memcpy(&datas[idx_data], (uint8_t *)m_cfg.ptr_mcu_data, length);
      break;

    default:
      break;
    }

    datas[idx_length_l] = (uint8_t)(length >> 0);
    datas[idx_length_h] = (uint8_t)(length >> 8);

    uint32_t idx = (uint32_t)idx_data + length;
    uint8_t checksum = 0;
    for (const auto &elm : datas)
      checksum += elm;
    checksum = (~checksum) + 1;
    datas[idx] = checksum;

    length = idx + 1;

    /*
      std::string s{};
      for (uint8_t i = 0; i < length; i++)
      {
        //s += std::to_string(static_cast<int>(m_receiveData.data[i])) + " ";
        char hex[5];
        std::snprintf(hex, sizeof(hex), "%02X", datas[i]);
        s += hex;
        s += " ";
      }
      LOG_PRINT("packet! [%s]",s.c_str());
      */
    return m_cfg.ptr_comm->SendCmd(datas.data(), length);
  }

  // It is processed as nonblock through machine step.
  inline void ThreadJob()
  {
    m_cfg.ptr_comm->ReceiveProcess();
  }

  inline void ProcessCmd(CMD::uart_cmd::packet_st &data)
  {
    this->m_receiveData = data;
    // LOG_PRINT("SendData type[%04X]!",data.type);
    using cmd_t = CMD::CMD_TYPE;

    auto ret_data = [&rx = m_receiveData](auto offset, auto &dest) -> uint8_t
    {
      memcpy(&dest, &rx.data[offset], sizeof(dest));
      return (uint8_t)(offset + sizeof(dest));
    };
    cmd_t rx_cmd = (cmd_t)m_receiveData.type;

    switch (rx_cmd)
    {
    case cmd_t::CMD_READ_TOOL_DATA:
      if (SendData(CMD::TX_TOOL_DATA) != ERROR_SUCCESS)
        LOG_PRINT("SendData Fail!");
      break;

    case cmd_t::CMD_READ_TOOL_INFO:
      if (SendData(CMD::TX_TOOL_INFO) != ERROR_SUCCESS)
        LOG_PRINT("SendData Fail!");
      break;

    case cmd_t::CMD_CTRL_IO_OUT:
    {
      SendData(CMD::TX_OK_RESPONSE);
      uint32_t io_reg_data = 0;
      uint8_t idx = 0;
      idx = ret_data(idx, io_reg_data);
      //LOG_PRINT("SetOutputReg data[0x%08X] - bank 1[0x%04X], bank 2[0x%04X]",io_reg_data, LoWORD(io_reg_data), HiWORD(io_reg_data));
      if (m_cfg.ptr_io->SetOutputReg(LoWORD(io_reg_data), ap_io::bank_main_1616) != ERROR_SUCCESS)
        LOG_PRINT("SetOutputReg [%d][%d] fail", LoWORD(io_reg_data), ap_io::bank_main_1616);

      if (m_cfg.ptr_io->SetOutputReg(HiWORD(io_reg_data), ap_io::bank_ext_1616) != ERROR_SUCCESS)
        LOG_PRINT("SetOutputReg [%d][%d] fail", HiWORD(io_reg_data), ap_io::bank_ext_1616);
      // LOG_PRINT("CMD_CTRL_IO_OUT value[%d]", io_reg_data);
    }
    break;
    case cmd_t::CMD_CTRL_READ_EXT_IO:
    {
      SendData(CMD::TX_OK_RESPONSE);
      mcu_io_align_12_t receive_data{};
      if (i2cReadBytes(_DEF_I2C1, HW_SLAVE_ADDRESS, IICOMM_CMD_READ_MCD_DATA, (uint8_t *)&receive_data, sizeof(receive_data)  , 100) == false) //datas.data(), datas.size()
        LOG_PRINT("[NG] i2cReadData fail! ch[%d], addr[0x%02X], reg[0x%04X] , size[%d]",_DEF_I2C1, HW_SLAVE_ADDRESS,IICOMM_CMD_READ_MCD_DATA ,sizeof(receive_data));
      else
      {
        LOG_PRINT("[OK] ext in[0x%04X], out[0x%04X]", (uint16_t)receive_data.io_in[0], (uint16_t)receive_data.io_out[0]);
      }
    }
    break;
    case cmd_t::CMD_MODE_FW_DOWNLOAD:
    {
      SendData(CMD::TX_OK_RESPONSE);
      uint32_t magic_data = 0;
      uint8_t idx = 0;
      idx = ret_data(idx, magic_data);
      if (magic_data == FLASH_MAGIC_NUMBER)
      {
        LOG_PRINT("CMD_MODE_FW_DOWNLOAD");
        const uint32_t stmf030c_addr_sysmem = 0x1FFF0000;
        const uint32_t wait_download_timeout_ms = 1000 * 2;
        wdgBegin(wait_download_timeout_ms);
        bspDeInit();
        __set_MSP(*(uint32_t *)stmf030c_addr_sysmem);

        void (**jump_addr)(void) = (void (**)(void))(stmf030c_addr_sysmem + 4);
        (*jump_addr)();
      }
    }
    break;
    default:
      break;
    }
    // end of switch
  }
};

#endif /* AP__INC_DEV_IO_HPP_ */
