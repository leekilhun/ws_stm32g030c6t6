/*
 * uart_cmd.hpp
 *
 *  Created on: 2023. 7. 23.
 *      Author: gns2l
 */

#ifndef AP__INC_UART_CMD_HPP_
#define AP__INC_UART_CMD_HPP_


/*

main(main controller), secondary(tool system)


TX  (main -> secondary) request information or action 
obj_id [option] 0 is all or ignore
| STX0 | STX1 | Type  | objId | Data Length |Data      | Checksum |
| :--- |:-----|:------|:----- |:------------|:---------| :------  |
| 0x4A | 0x4C | 2byte | 2byte | 2 byte      |Data 0～n | 1byte    |


Message
RX (secondary -> main) provide information
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

namespace CMD
{

  #if 0
  //TX (main -> secondary) request information or action
  enum TX_TYPE:uint16_t
  {
    TX_OK_RESPONSE                    = 0x0000,
    TX_READ_TOOL_DATA                 = 0x0001,

    TX_CTRL_IO_OUT                    = 0x0010,
    TX_CTRL_CYL                       = 0x0011,
    TX_CTRL_VAC                       = 0x0012,

    TX_CTRL_INITIALIZE                = 0x001A,
    TX_CTRL_VIRTUAL_SW                = 0x001B,

    TX_CTRL_MOT_ORIGIN                = 0x0020,
    TX_CTRL_MOT_ONOFF                 = 0x0021,
    TX_CTRL_MOT_MOVE                  = 0x0022,
    TX_CTRL_MOT_STOP                  = 0x0023,
    TX_CTRL_MOT_JOG                   = 0x0024,
    TX_CTRL_MOT_LIMIT                 = 0x0025,
    TX_CTRL_MOT_ZEROSET               = 0x0026,
    TX_CTRL_MOT_RELMOVE               = 0x0027,
    TX_CTRL_MOT_CLEAR_ALARM           = 0x0028,
    TX_CTRL_MOT_CHANGE_VEL            = 0x0029,
    TX_CTRL_MOT_MOVE_VEL              = 0x002A,
    TX_CTRL_MOT_RELMOVE_VEL           = 0x002B,
    TX_CTRL_MOT_VEL_JOG               = 0x002C,

    
  };



  //RX  (secondary -> main) provide information
  enum MSG_TYPE:uint16_t
  {
    MSG_OK_RESPONSE                   = 0x0000,
    MSG_TOOL_DATA                     = 0x0001,
  };

#endif

//TX  (secondary -> main) provide information
  enum TX_TYPE:uint16_t
  {    
    TX_OK_RESPONSE      = 0x0000,
    TX_TOOL_DATA        = 0x0001,
    TX_TOOL_INFO        = 0x0002,
  };



  //RX (main -> secondary) request information or action
  enum CMD_TYPE:uint16_t
  {
    CMD_OK_RESPONSE                   = 0x0000,
    CMD_READ_TOOL_DATA                = 0x0001,
    CMD_READ_TOOL_INFO                = 0x0002,
     
    CMD_CTRL_IO_OUT                   = 0x0010,
    CMD_CTRL_CYL                      = 0x0011,
		CMD_CTRL_VAC                      = 0x0012,
    CMD_CTRL_REG_OPTION               = 0x0013,

    CMD_CTRL_INITIALIZE               = 0x001A,
		CMD_CTRL_VIRTUAL_SW               = 0x001B,

    CMD_CTRL_MOT_ORIGIN               = 0x0020,
    CMD_CTRL_MOT_ONOFF                = 0x0021,
    CMD_CTRL_MOT_MOVE                 = 0x0022,
    CMD_CTRL_MOT_STOP                 = 0x0023,
    CMD_CTRL_MOT_JOG                  = 0x0024,
    CMD_CTRL_MOT_LIMIT                = 0x0025,
    CMD_CTRL_MOT_ZEROSET              = 0x0026,
    CMD_CTRL_MOT_RELMOVE              = 0x0027,
    CMD_CTRL_MOT_CLEAR_ALARM          = 0x0028,
    CMD_CTRL_MOT_CHANGE_VEL           = 0x0029,
    CMD_CTRL_MOT_MOVE_VEL             = 0x002A,
    CMD_CTRL_MOT_RELMOVE_VEL          = 0x002B,
    CMD_CTRL_MOT_VEL_JOG              = 0x002C,

  };


  constexpr uint8_t CMD_STX0               = 0x4A;
  constexpr uint8_t CMD_STX1               = 0x4C;


  constexpr int CMD_MAX_DATA_LENGTH       = 120;
  constexpr int CMD_MAX_PACKET_LENGTH     =(CMD_MAX_DATA_LENGTH + 8);
  constexpr int PACKET_BUFF_LENGTH        = CMD_MAX_PACKET_LENGTH;

#ifdef _USE_HW_RTOS
#define AP_UART_CMD_LOCK_BEGIN osMutexWait(m_mutex_id, osWaitForever)
#define AP_UART_CMD_LOCK_END   osMutexRelease(m_mutex_id)
#else
#define AP_UART_CMD_LOCK_BEGIN
#define AP_UART_CMD_LOCK_END
#endif




  struct uart_cmd
  {
    /****************************************************
     *  data
     ****************************************************/
    struct cfg_t
    {
      uint8_t ch{};
      uint32_t baud{};

      cfg_t() = default;
      // copy constructor
      cfg_t(const cfg_t& other) = default;
      // copy assignment
      cfg_t& operator=(const cfg_t& other) = default;
      // move constructor
      cfg_t(cfg_t&& other) = default;
      // move assignment
      cfg_t& operator=(cfg_t&& other) = default;
    };


    struct packet_st {
      uint16_t       type{};
      uint16_t       obj_id{};
      uint16_t       data_length{};
      uint8_t*       data{};
      uint8_t        checksum{};
      uint8_t        checksum_recv{};
      std::array <uint8_t, CMD_MAX_PACKET_LENGTH> buffer{};
      uint8_t         buffer_idx{};
      uint16_t        data_cnt{};
      uint32_t        resp_ms{};
      prc_step_t      state{};

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
        checksum += rx_data;
        buffer[buffer_idx] = rx_data;
        buffer_idx = (buffer_idx + 1) % CMD_MAX_PACKET_LENGTH;
        return buffer_idx;
      }

      void BufferClear()
      {
        buffer.fill(0);
        buffer_idx = 0;
        data_cnt = 0;
        checksum = 0;
        state.SetStep(0);
      }


    };

    bool m_Isconnected;
    cfg_t m_cfg;
    packet_st m_packet;

    void* m_obj;
    evt_cb m_func;
    uint32_t m_packet_sending_ms;
    uint8_t m_reqFlag;

#ifdef _USE_HW_RTOS
    osMutexId m_mutex_id;
#endif

    /****************************************************
     *  Constructor
     ****************************************************/
  public:
    uart_cmd():m_Isconnected{}, m_cfg{}, m_packet{}, m_obj{}, m_func{}, m_packet_sending_ms{}, m_reqFlag{}
    {
#ifdef _USE_HW_RTOS
      osMutexDef(m_mutex_id);
      m_mutex_id = osMutexCreate (osMutex(m_mutex_id));
#endif

    }
    ~uart_cmd (){}

    /****************************************************
     *  func
     ****************************************************/

    inline void Init(cfg_t &cfg) {
      m_cfg = cfg;
      if (uartOpen(cfg.ch, cfg.baud))
      {
        LOG_PRINT("[OK] Init Open Success! Uart ch[%d], baud[%d]",cfg.ch, cfg.baud);
        m_Isconnected = true;
      }
    }


    inline bool Recovery() {
      uartClose(m_cfg.ch);
      /* */
      m_Isconnected = uartOpen(m_cfg.ch, m_cfg.baud);
      return m_Isconnected;
    }

    inline bool IsAvailableComm(){
      return (m_reqFlag == 0);
    }

    inline bool IsConnected() {
      return m_Isconnected;
    }

    inline bool ReceiveProcess(){
      bool ret = false;
      if (receivePacket())
      {
        receiveCplt();
        ret = true;
      }
      return ret;
    }

    inline void AttCallbackFunc(void* obj, evt_cb cb)
    {
      m_obj = obj;
      m_func = cb;
    }


    inline errno_t SendCmd(uint8_t* ptr_data, uint32_t size) {
      m_packet_sending_ms = millis();
      m_reqFlag++;
      if (uartWrite(m_cfg.ch, ptr_data, size)){
        return ERROR_SUCCESS;
      }
      return -1;
    }


    inline errno_t SendCmdRxResp(uint8_t* ptr_data, uint32_t size, uint32_t timeout){
      if (SendCmd(ptr_data, size) == ERROR_SUCCESS)
      {
        uint32_t pre_ms = millis();
        bool result = true;
        while (receivePacket() == false)
        {
          if ((millis() - pre_ms) > timeout)
          {
            result = false;
            break;
          }
        }
        if (result)
        {
          receiveCplt();
          return ERROR_SUCCESS;
        }
      }
      return -1;
    }


    inline bool receivePacket() {

      enum : uint8_t
      { STATE_WAIT_STX0, STATE_WAIT_STX1,
        STATE_WAIT_TYPE_L, STATE_WAIT_TYPE_H, STATE_WAIT_OBJ_ID_L, STATE_WAIT_OBJ_ID_H,
        STATE_WAIT_LENGTH_L, STATE_WAIT_LENGTH_H, STATE_WAIT_DATA, STATE_WAIT_CHECKSUM
      };

      uint8_t rx_data = 0x00 ;

      if (m_packet.state.MoreThan(100))
      {
        m_packet.BufferClear();
      }

      while (uartAvailable(m_cfg.ch))
      {
        rx_data = uartRead(m_cfg.ch);
        //LOG_PRINT("rx_data %d", rx_data);

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
            if (m_packet.checksum == m_packet.checksum_recv)
              return true;

            break;

          default:
            return false;
        }
        // end of  switch
      }
      //while (uartAvailable(m_cfg.ch))




      return false;
    }


  private:
    inline void receiveCplt() {
      m_reqFlag = 0;
      m_packet.resp_ms = millis() - m_packet_sending_ms;
      if (m_func && m_obj)
      {
        m_func(m_obj,nullptr,&m_packet);
      }
    }


  };

}
// end of CMD namespace


#endif /* AP__INC_UART_CMD_HPP_ */
