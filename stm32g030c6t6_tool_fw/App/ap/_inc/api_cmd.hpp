/*
 * api_cmd.hpp
 *
 *  Created on: 2023. 8. 17.
 *      Author: gns2l
 *
 *  edit : 24.01.30  CMD -> RX
 */

#pragma once
#ifndef AP__INC_API_CMD_HPP_
#define AP__INC_API_CMD_HPP_

#include "ap_def.hpp"

struct api_cmd
{
  /****************************************************
   *  data
   ****************************************************/

public:
  struct cfg_t
  {
    CMD::uart_cmd            *ptr_comm{};
    ap_reg                   *ptr_mcu_reg{};
    ap_io                    *ptr_io{};
    mcu_tool_data_align_48_t *ptr_mcu_data{};
    char                      tool_info[32]{};
#ifndef CMD_PART_MAIN
    cnMotors *ptr_motors{};
#endif

    cfg_t() = default;
    // destructor
    ~cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t &other) = default;
    // copy assignment
    cfg_t &operator=(const cfg_t &other) = default;
    // move constructor
    cfg_t(cfg_t &&other) = default;
    // move assignment
    cfg_t &operator=(cfg_t &&other) = default;

    char *set_info(const char *info)
    {
      memset(&tool_info[0], 0, sizeof(tool_info));
      strlcpy(&tool_info[0], info, sizeof(tool_info));
      return &tool_info[0];
    }
  };

  bool                     m_IsInit;
  cfg_t                    m_cfg;
  CMD::uart_cmd::packet_st m_receiveData;

  prc_step_t m_step;
  uint32_t   m_elaps;

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  api_cmd() : m_IsInit{}, m_cfg{}, m_receiveData{}, m_step{}, m_elaps{} {

                                                              };

  ~api_cmd(){};

  /****************************************************
   *  func
   ****************************************************/
private:
  void doRunStep();

  // callback function
  inline static void receiveDataFunc(void *obj, void *w_parm, void *l_parm)
  {
    api_cmd *ptr_this = (api_cmd *)obj;

    ptr_this->m_step.wait_resp = false;
    if (w_parm == nullptr && obj == nullptr && l_parm == nullptr)
      return;
    ptr_this->ProcessCmd(*(CMD::uart_cmd::packet_st *)l_parm);
  }

public:
  inline int Init(api_cmd::cfg_t &cfg)
  {
    m_cfg = cfg;
    cfg.ptr_comm->AttCallbackFunc(this, receiveDataFunc);
    m_IsInit = true;
    LOG_PRINT("[OK] Init Success! ");
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

  // It is processed as nonblock through machine step.
  inline void ThreadJob()
  {
    m_cfg.ptr_comm->ReceiveProcess();
  }


#ifdef CMD_PART_MAIN

  template <typename Type, typename... ElementType>
  errno_t SendOrder(Type type, ElementType... elements)
  {
    using namespace CMD;
    if constexpr (std::is_same_v<Type, TX_TYPE>)
    {
      std::array<uint8_t, uart_cmd::CMD_MAX_PACKET_LENGTH> datas = {};
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
      datas[idx_stx0]    = uart_cmd::CMD_STX0;
      datas[idx_stx1]    = uart_cmd::CMD_STX1;
      datas[idx_type_l]  = LoBYTE(cmd_type);
      datas[idx_type_h]  = HiBYTE(cmd_type);
      datas[idx_objid_l] = 0;
      datas[idx_objid_h] = 0;
      if constexpr (sizeof...(elements) == 0)
      {
        if (type != TX_TYPE::TX_OK_RESPONSE 
          ||type != TX_TYPE::TX_READ_TOOL_DATA 
          ||type != TX_TYPE::TX_READ_TOOL_INFO
          ||type != TX_TYPE::TX_MODE_FW_DOWNLOAD
          ||type != TX_TYPE::TX_CTRL_INITIALIZE)
        {
          LOG_PRINT("Error! Wrong type.");
          return ERROR_FAIL;
        }
      }
      else
      {
        using ele_t = std::common_type_t<ElementType...>;
        std::array<ele_t, sizeof...(elements)> values = {elements...};

        auto set_data = [&packet = datas](auto offset, auto src) 
          {
            std::memcpy(&packet[idx_data + offset], &src , sizeof(src));
            return  sizeof(src);
          };

        if constexpr (sizeof...(elements) == 1)
        {
          if (type == TX_TYPE::TX_CTRL_IO_OUT
            ||type == TX_TYPE::TX_CTRL_VIRTUAL_SW
            ||type == TX_TYPE::TX_CTRL_MOT_STOP 
            ||type == TX_TYPE::TX_CTRL_MOT_ZEROSET
            ||type == TX_TYPE::TX_CTRL_MOT_CLEAR_ALARM        
            ||type == TX_TYPE::TX_CTRL_MOT_ORIGIN
            ||type == TX_TYPE::TX_CTRL_MOT_LIMIT    
            )
          {
            // length = sizeof(int);
            // std::memcpy(&datas[idx_data], (uint8_t *)&arr[0], length);
            length = set_data(0, (uint32_t)values[0]);
          }  
          else if (type == TX_TYPE::TX_CTRL_REG_OPTION)
          {
            length = set_data(0, (uint16_t)values[0]);
          }       
          else
          {
            LOG_PRINT("Error! Wrong type.");
            return ERROR_FAIL;
          }
        }
        else if constexpr (sizeof...(elements) == 2)
        {
          if (type == TX_TYPE::TX_CTRL_MOT_MOVE             
            ||type == TX_TYPE::TX_CTRL_MOT_RELMOVE
            ||type == TX_TYPE::TX_CTRL_MOT_ONOFF
            ||type == TX_TYPE::TX_CTRL_MOT_JOG
          )
          {
            enum : uint8_t {_objid, _data1};
            length = set_data(length, (uint16_t)values[_objid]);
            length += set_data(length, (int)values[_data1]);
          }
          else
          {
            LOG_PRINT("Error! Wrong type.");
            return ERROR_FAIL;
          }
        }
        else if constexpr (sizeof...(elements) == 3)
        {
        }
        else if constexpr (sizeof...(elements) == 4)
        {
          if (type == TX_TYPE::TX_CTRL_TOOL_PnP)
          {
            enum : uint8_t {_order, _objid, _data1, _datat2};
            length = set_data(0, (uint16_t)values[_order]);
            length += set_data(length, (uint16_t)values[_objid]);
            length += set_data(length, (int)values[_data1]);
            length += set_data(length, (int)values[_datat2]);
          }
          else
          {
            LOG_PRINT("Error! Wrong type.");
            return ERROR_FAIL;
          }
        }
        else
        {
          LOG_PRINT("Error! Too many arguments.");
          return ERROR_FAIL;
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

      return m_cfg.ptr_comm->SendCmd(datas.data(), length);
    }
    else
    {
      LOG_PRINT("Error! Wrong type.");
      return ERROR_FAIL;
    }


    return ERROR_FAIL;
  }

  //  template <typename Type, typename... ElementType>
  //  errno_t SendOrder(Type data, Type data, ElementType... elements) -> SendOrder<Type, ElementType...>;

  inline errno_t Ret_OkResponse()
  {
    return SendOrder(CMD::TX_TYPE::TX_OK_RESPONSE);
  }



  // The PC processes command reception and returns the received ack signal.
  inline void ProcessCmd(CMD::uart_cmd::packet_st &data)
  {
    this->m_receiveData = data;
    using cmd_t         = CMD::RX_TYPE;

    // auto ret_data = [&rx = m_receiveData](auto offset, auto &dest) -> uint8_t
    // {
    //   memcpy(&dest, &rx.data[offset], sizeof(dest));
    //   return (uint8_t)(offset + sizeof(dest));
    // };

    cmd_t rx_cmd = (cmd_t)m_receiveData.type;
    switch (rx_cmd)
    {
      case cmd_t::RX_TOOL_DATA:
      {
        Ret_OkResponse();
       *(m_cfg.ptr_mcu_data) = *((mcu_tool_data_align_48_t *)m_receiveData.data);
      }
      break;

      case cmd_t::RX_TOOL_INFO:
      {
        Ret_OkResponse();
        m_cfg.set_info((const char *)m_receiveData.data);
        LOG_PRINT("TOOL_INFO [%s]", m_cfg.tool_info);
      }
      break;

      default: break;
    }
  }

#else

  inline errno_t Ret_OkResponse()
  {
    return SendData(CMD::TX_TYPE::TX_OK_RESPONSE);
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

    constexpr uint16_t obj_id        = 0x0000;
    uint16_t           length        = 0;
    datas[idx_stx0]                  = CMD::uart_cmd::CMD_STX0;
    datas[idx_stx1]                  = CMD::uart_cmd::CMD_STX1;
    datas[idx_type_l]                = (uint8_t)(type >> 0);
    datas[idx_type_h]                = (uint8_t)(type >> 8);
    datas[idx_objid_l]               = (uint8_t)(obj_id >> 0);
    datas[idx_objid_h]               = (uint8_t)(obj_id >> 8);
    constexpr const char *tools_info = "TOOLS_SYS_1.0.0";
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

    uint32_t idx      = (uint32_t)idx_data + length;
    uint8_t  checksum = 0;
    for (const auto &elm : datas)
      checksum += elm;
    checksum   = (~checksum) + 1;
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

  // The PC processes command reception and returns the received ack signal.
  inline void ProcessCmd(CMD::uart_cmd::packet_st &data)
  {
    this->m_receiveData = data;
    using cmd_t         = CMD::RX_TYPE;

    /*

     */
    auto ret_data = [&rx = m_receiveData](auto offset, auto &dest) -> uint8_t
    {
      memcpy(&dest, &rx.data[offset], sizeof(dest));
      return (uint8_t)(offset + sizeof(dest));
    };
    cmd_t rx_cmd = (cmd_t)m_receiveData.type;
    // LOG_PRINT("ProcessCmd cmd type[%d], resp_ms[%d]", rx_cmd, m_receiveData.resp_ms);

    switch (rx_cmd)
    {
    case cmd_t::RX_READ_TOOL_DATA:
      if (SendData(CMD::TX_TOOL_DATA) != ERROR_SUCCESS)
        LOG_PRINT("SendData Fail!");
      break;

    case cmd_t::RX_READ_TOOL_INFO:
      if (SendData(CMD::TX_TOOL_INFO) != ERROR_SUCCESS)
        LOG_PRINT("SendData Fail!");
      break;

    case cmd_t::RX_CTRL_IO_OUT:
    {
      SendData(CMD::TX_OK_RESPONSE);
      uint32_t io_reg_data = 0;
      uint8_t  idx         = 0;
      idx                  = ret_data(idx, io_reg_data);
      m_cfg.ptr_io->SetOutputReg(io_reg_data);
      // LOG_PRINT("RX_CTRL_IO_OUT value[%d]", io_reg_data);
    }
    break;

    case cmd_t::RX_CTRL_CYL:
      break;

    case cmd_t::RX_CTRL_VAC:
      break;

    case cmd_t::RX_CTRL_REG_OPTION:
      break;

    case cmd_t::RX_CTRL_INITIALIZE:
      break;

    case cmd_t::RX_CTRL_MOT_ORIGIN:
      break;

    case cmd_t::RX_CTRL_MOT_ONOFF:
      break;

    case cmd_t::RX_CTRL_MOT_MOVE:
      break;

    case cmd_t::RX_CTRL_MOT_STOP:
      break;

    case cmd_t::RX_CTRL_MOT_JOG:
      break;

    case cmd_t::RX_CTRL_MOT_LIMIT:
      break;

    case cmd_t::RX_CTRL_MOT_ZEROSET:
      break;

    case cmd_t::RX_CTRL_MOT_RELMOVE:
      break;

    case cmd_t::RX_CTRL_MOT_CLEAR_ALARM:
      break;

    case cmd_t::RX_CTRL_MOT_CHANGE_VEL:
      break;

    case cmd_t::RX_CTRL_MOT_MOVE_VEL:
      break;

    case cmd_t::RX_CTRL_MOT_RELMOVE_VEL:
      break;

    case cmd_t::RX_CTRL_MOT_VEL_JOG:
      break;

    case cmd_t::RX_CTRL_TOOL_PnP:
    {
      Ret_OkResponse();

      uint16_t order{};
      int      data_1{}, data_2{};
      uint8_t  idx         = 0;
      idx                  = ret_data(idx, order);
      idx                  = ret_data(idx, data_1);
      idx                  = ret_data(idx, data_2);
      AP_OBJ::MOTOR motor_idx = constrain((AP_OBJ::MOTOR)m_receiveData.obj_id,AP_OBJ::MOTOR_A, AP_OBJ::MOTOR_MAX);
 
      switch ((CMD::tool_PnP_order_e)order)
      {
      case CMD::tool_PnP_order_e::TOOL_CMD_ORD_MOTOR_ENABLE:
        if (m_cfg.ptr_motors->MotorOnOff(motor_idx) != ERROR_SUCCESS)
          LOG_PRINT("DoOrigin Error");
        break;

      case CMD::tool_PnP_order_e::TOOL_CMD_ORD_MOTOR_DISABLE:
        if (m_cfg.ptr_motors->MotorOnOff(motor_idx, false) != ERROR_SUCCESS)
          LOG_PRINT("DoOrigin Error");
        break;

      case CMD::tool_PnP_order_e::TOOL_CMD_ORD_MOTOR_RUN:
        if (m_cfg.ptr_motors->Move(motor_idx, data_1) != ERROR_SUCCESS)
          LOG_PRINT("DoOrigin Error");
        // m_cfg.ptr_tool->MotorRun(tool_obj_id, data_1);
        break;

      case CMD::tool_PnP_order_e::TOOL_CMD_ORD_MOTOR_ORG:
        if (m_cfg.ptr_motors->DoOrigin(motor_idx) != ERROR_SUCCESS)
          LOG_PRINT("DoOrigin Error");
        break;

      default:
        break;
      }
    }
    break;

    case cmd_t::RX_MODE_FW_DOWNLOAD:
    {
      SendData(CMD::TX_OK_RESPONSE);
      uint32_t magic_data = 0;
      uint8_t  idx        = 0;
      idx                 = ret_data(idx, magic_data);
      if (magic_data == FLASH_MAGIC_NUMBER)
      {
        LOG_PRINT("CMD_MODE_FW_DOWNLOAD");
        const uint32_t stmf030c_addr_sysmem     = 0x1FFF0000;
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
  }

#endif
};
// end of struct api_cmd

#endif
