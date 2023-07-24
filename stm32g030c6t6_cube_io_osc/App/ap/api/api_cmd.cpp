/*
 * api_cmd.cpp
 *
 *  Created on: 2023. 7. 23.
 *      Author: gns2l
 */





#include "ap.hpp"
#include "api_cmd.hpp"

using namespace CMD;

errno_t api_cmd::SendData(TX_TYPE type, uint16_t obj_id )
{
  uint16_t length = 0;
  enum :uint8_t {
    idx_stx0, idx_stx1,idx_type_l,idx_type_h,idx_objid_l,
    idx_objid_h, idx_length_l,idx_length_h, idx_data
  };
  std::array<uint8_t, CMD_MAX_PACKET_LENGTH> value = { };
  //using tx_t = TX_TYPE;
  //uint8_t* ptr = nullptr;
  value[idx_stx0] = CMD_STX0;
  value[idx_stx1] = CMD_STX1;
  value[idx_type_l] = (uint8_t)(type >> 0);
  value[idx_objid_h] =  (uint8_t)(type >> 8);
  value[idx_objid_l] = (uint8_t)(obj_id >> 0);
  value[idx_type_h] =  (uint8_t)(obj_id >> 8);
#if 0
  switch (type)
  {
    case tx_t::TX_MCU_DATA:
      length = sizeof(*(m_cfg.ptr_mcu_data));
      std::memcpy(&value[idx_data],(uint8_t*)m_cfg.ptr_mcu_data, length);
      value[idx_length_l] = (uint8_t)(length >> 0);
      value[idx_length_h] = (uint8_t)(length >> 8);
      break;

    case tx_t::TX_FIRM_INFO:
      length = sizeof(firm_version_t{});
      ptr = &value[idx_data];
      std::memcpy(ptr,(firm_version_t *)(FLASH_ADDR_FW_VER), length);
      /* ptr = ptr + length;
      std::memcpy(ptr,(firm_version_t *)(FLASH_ADDR_FW_VER), length);*/
      value[idx_length_l] = (uint8_t)((length) >> 0);
      value[idx_length_h] = (uint8_t)((length) >> 8);
      break;

    case tx_t::TX_OK_RESPONSE:
    default:
      break;
  }
  //end of switch
#endif


  uint8_t check_sum = 0;
  uint32_t idx = 1;
  for (uint8_t i = 1; i < (length + idx_data); i++)
  {
    idx++;
    check_sum += value[i];
  }
  value[idx++] = (~check_sum) + 1;

  /*
  std::string s{};
  for (uint8_t i = 0; i < value.size(); i++)
  {
    //s += std::to_string(static_cast<int>(m_receiveData.data[i])) + " ";
    char hex[5];
    std::snprintf(hex, sizeof(hex), "%02X", value[i]);
    s += hex;
    s += " ";
  }
  LOG_PRINT("packet! [%s]",s.c_str());
   */
  return m_cfg.ptr_comm->SendCmd(value.data(), idx/*(uint32_t)value.size()*/);
}



void api_cmd::ThreadJob()
{
  doRunStep();
#ifndef _USE_HW_RTOS
  m_cfg.ptr_comm->ReceiveProcess();
#endif
}


void api_cmd::doRunStep()
{
  using tx_t = TX_TYPE;

  enum : uint8_t {
    STEP_INIT,STEP_TODO,STEP_TIMEOUT,STEP_RESET_COMM_ALARM,
    STEP_STATE_UPDATE,STEP_STATE_UPDATE_START,STEP_STATE_UPDATE_WAIT,STEP_STATE_UPDATE_END,
  };
  constexpr uint8_t step_retry_max = 3;
  constexpr uint32_t step_wait_delay = 50;


  switch(m_step.GetStep())
  {
    case STEP_INIT:
    {
      m_step.SetStep(STEP_TODO);
    }
    break;

    /*######################################################
      to do
    ######################################################*/
    case STEP_TODO:
    {
      if(m_step.msgQ.Available())
      {
        volatile uint8_t step{};
        m_step.msgQ.Get((uint8_t*)&step);
        m_step.SetStep(step);
      }
    }
    break;
    /*######################################################
      timeout
    ######################################################*/
    case STEP_TIMEOUT:
    {
      LOG_PRINT("STEP_TIMEOUT recovery result[%d]",  m_cfg.ptr_comm->Recovery());

      m_step.SetStep(STEP_TODO);
    }
    break;
    /*######################################################
      STEP_STATE_UPDATE
    ######################################################*/
    case STEP_STATE_UPDATE:
    {
      m_step.wait_resp = false;
      m_step.wait_step = 0;
      m_step.retry_cnt = 0;

      m_step.SetStep(STEP_STATE_UPDATE_START);
    }
    break;

    case STEP_STATE_UPDATE_START:
    {
      if (SendData(tx_t::TX_MCU_DATA) == ERROR_SUCCESS)
        m_step.SetStep(STEP_STATE_UPDATE_WAIT);
      else
      {
        LOG_PRINT("STEP_STATE_UPDATE_START send data failed!");
        m_step.SetStep(STEP_TIMEOUT);
      }
    }
    break;

    case STEP_STATE_UPDATE_WAIT:
    {
      if(m_step.LessThan(step_wait_delay))
        break;

      if (m_cfg.ptr_comm->IsAvailableComm())
        m_step.SetStep(STEP_STATE_UPDATE_END);
      else
      {
        if (m_step.retry_cnt++ < step_retry_max)
        {
          m_step.SetStep(STEP_STATE_UPDATE_WAIT); // timer reset
          break;
        }
        m_step.SetStep(STEP_TIMEOUT);
        LOG_PRINT("STEP_TIMEOUT retry[%d]",  m_step.retry_cnt);
      }
    }
    break;

    case STEP_STATE_UPDATE_END:
    {

      m_step.SetStep(STEP_TODO);
    }
    break;

    default:
      break;
  }
  // end of switch


}


void api_cmd::ProcessCmd(uart_cmd::packet_st& data)
{
  this->m_receiveData = data;
  using cmd_t = CMD_TYPE;
  using tx_t = TX_TYPE;
  enum : uint8_t {
    STEP_INIT,STEP_TODO,STEP_TIMEOUT,STEP_RESET_COMM_ALARM,
    STEP_STATE_UPDATE,STEP_STATE_UPDATE_START,STEP_STATE_UPDATE_WAIT,STEP_STATE_UPDATE_END,
  };

  /*

   */

  cmd_t rx_cmd = (cmd_t)m_receiveData.type;
  LOG_PRINT("ProcessCmd cmd type[%d]",  rx_cmd);
  switch (rx_cmd)
  {
    case cmd_t::CMD_READ_MCU_DATA:
      m_step.msgQ.Put(STEP_STATE_UPDATE);
      break;

    case cmd_t::CMD_READ_FIRM_INFO:
      if (SendData(tx_t::TX_FIRM_INFO) != ERROR_SUCCESS)
        LOG_PRINT("CMD_READ_FIRM_INFO send data failed!");
      break;
    case cmd_t::CMD_OK_RESPONSE:
      break;
    default:
      break;
  }


}
