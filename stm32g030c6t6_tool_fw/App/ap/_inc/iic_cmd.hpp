/*
 * iic_cmd.hpp
 *
 *  Created on: Nov 1, 2023
 *      Author: gns2.lee
 */

#pragma once
#ifndef AP__INC_IIC_CMD_HPP_
#define AP__INC_IIC_CMD_HPP_

#include "ap_def.hpp"

struct iic_cmd
{
  enum cmd_order_e : int16_t
  {
    CMD_ERR_RET = -1,
    CMD_NONE = 0,
    CMD_ORD_MOTOR_ENABLE = 1,
    CMD_ORD_MOTOR_DISABLE ,
    CMD_ORD_MOTOR_RUN,
    CMD_ORD_MOTOR_ORG,
    CMD_ORD_4,
  };
  /****************************************************
   *  data
   ****************************************************/
  struct cfg_t
  {
    bool (*WriteReg)(uint8_t, uint8_t *){};
    bool (*ReadReg)(uint8_t, uint8_t *){};

    cfg_t() = default;
    ~cfg_t() = default;
    // copy constructor
    cfg_t(const cfg_t &other) = default;
    // copy assignment
    cfg_t &operator=(const cfg_t &other) = default;
    // move constructor
    cfg_t(cfg_t &&other) = default;
    // move assignment
    cfg_t &operator=(cfg_t &&other) = default;
  } m_cfg{};

  int16_t cmdOrderReg{};
  std::array<uint32_t, IICS_REG_OUT_IDX_MAX> outRegs{};
  std::array<data_t, IICS_REG_ORD_DATA_IDX_MAX> orderDatas{};

  /****************************************************
   *  Constructor
   ****************************************************/
public:
  iic_cmd() = default;
  ~iic_cmd() {}

  /****************************************************
   *  func
   ****************************************************/

  inline void Init(cfg_t &cfg)
  {
    m_cfg = cfg;
    LOG_PRINT("[OK] Init Success!");
  }

  inline errno_t ResetOrderRegister()
  {
    cmdOrderReg = 0;
    orderDatas.fill(data_t{});
    if (m_cfg.WriteReg((uint8_t)IICOMM_CMD_WR_ORDER, (uint8_t *)&cmdOrderReg) == false)
    {
      // LOG_PRINT("WriteReg [%d]", m_cfg.WriteReg);
      LOG_PRINT("WriteReg IICOMM_CMD_WR_ORDER_DATA Err");
      return -1;
    }

    if (m_cfg.WriteReg((uint8_t)IICOMM_CMD_WR_ORDER_DATA, (uint8_t *)orderDatas.data()) == false)
    {
      // LOG_PRINT("WriteReg [%d]", m_cfg.WriteReg);
      LOG_PRINT("WriteReg IICOMM_CMD_WR_ORDER_DATA Err");
      return -1;
    }

    // LOG_PRINT("cmdOrderReg[0x%04X], orderData[%d][%d] ",cmdOrderReg,orderDatas[0], orderDatas[1]);
    return ERROR_SUCCESS;
  }

  inline int16_t Process()
  {
    if (this->GetOrderReg() != ERROR_SUCCESS)
    {
      LOG_PRINT("cmd_comm.GetOrderReg Error");
      return CMD_ERR_RET;
    }
    else
    {
      if (this->GetOrderData() != ERROR_SUCCESS)
      {
        LOG_PRINT("cmd_comm.GetOrderData Error");
        return CMD_ERR_RET;
      }
      else
      {
        return cmdOrderReg;
      }
    }
  };

  inline errno_t SetMcuData(mcu_tool_data_align_48_t *ptr_data)
  {
    // LOG_PRINT("WriteReg [%d]",m_cfg.WriteReg);
    if (m_cfg.WriteReg != nullptr)
    {
      if (m_cfg.WriteReg((uint8_t)IICOMM_CMD_WR_MCD_DATA, (uint8_t *)ptr_data) == false)
      {
        // LOG_PRINT("WriteReg [%d]", m_cfg.WriteReg);
        LOG_PRINT("WriteReg[0x%08X] Err", ptr_data);
        return -1;
      }
    }
    else
      return -1;
    return ERROR_SUCCESS;
  }

  inline errno_t GetOrderReg()
  {
    if (m_cfg.ReadReg != nullptr)
    {
      if (m_cfg.ReadReg((uint8_t)IICOMM_CMD_WR_ORDER, (uint8_t *)&cmdOrderReg) == false)
      {
        LOG_PRINT("ReadReg[0x%08X] Err", IICOMM_CMD_WR_ORDER);
        return -1;
      }
    }
    else
      return -1;
    return ERROR_SUCCESS;
  }

  inline errno_t GetOrderData()
  {
    if (m_cfg.ReadReg != nullptr)
    {
      if (m_cfg.ReadReg((uint8_t)IICOMM_CMD_WR_ORDER_DATA, (uint8_t *)orderDatas.data()) == false)
      {
        LOG_PRINT("ReadReg[0x%08X] Err", IICOMM_CMD_WR_ORDER_DATA);
        return -1;
      }
    }
    else
      return -1;
    return ERROR_SUCCESS;
  }

  inline errno_t GetOutReg()
  {
    if (m_cfg.ReadReg != nullptr)
    {
      if (m_cfg.ReadReg((uint8_t)IICOMM_CMD_WR_OUT, (uint8_t *)outRegs.data()) == false)
      {
        LOG_PRINT("ReadReg[0x%08X] Err", IICOMM_CMD_WR_OUT);
        return -1;
      }
    }
    else
      return -1;
    return ERROR_SUCCESS;
  }
};

#endif /* AP__INC_IIC_CMD_HPP_ */
