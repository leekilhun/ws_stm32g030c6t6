/*
 * enOp.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_ENOP_HPP_
#define AP__INC_ENOP_HPP_




struct enOp
{
  enum mode_e
  {
    READY,
    AUTORUN,
    STOP,
    DRY_RUN,
  };

  enum status_e
  {
    INIT,
    ERR_STOP,
    STEP_STOP,
    RUN_READY,
    RUN,
  };

  enum panel_e
  {
    SW_START,
    SW_STOP,
    SW_RESET,
    SW_ESTOP,
    SW_MAX
  };

  enum lamp_e
  {
    LAMP_START,
    LAMP_STOP,
    LAMP_RESET,
  };

  struct cfg_t
  {
    ap_reg* ptr_mcu_reg{};
    ap_io * ptr_mcu_io{};

    uint8_t sw_pin_start{};
    uint8_t sw_pin_stop{};
    uint8_t sw_pin_reset{};
    uint8_t sw_pin_estop{};


    uint8_t lamp_pin_start{};
    uint8_t lamp_pin_stop{};
    uint8_t lamp_pin_reset{};

    cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t& rhs) = default;
    // copy assignment operator
    cfg_t& operator=(const cfg_t& rhs) = default;
    // move constructor
    cfg_t(cfg_t&& rhs) = default;
    // move assignment operator
    cfg_t& operator=(cfg_t&& rhs) = default;

  };

  volatile enOp::status_e m_status;
  volatile enOp::mode_e m_mode;
  enOp::cfg_t m_cfg;

public:
  enOp() : m_status(enOp::status_e::INIT), m_mode (enOp::mode_e::STOP){ }
  virtual ~enOp(){}
  inline int Init(enOp::cfg_t& cfg){
    LOG_PRINT("Init Success!");
    m_cfg = cfg;
    return 0;
  }

  inline enOp::status_e GetStatus() const{
    return m_status;
  }

  inline void SetStatus(enOp::status_e state){
    m_status = state;
  }

  inline enOp::mode_e GetMode() const{
    return m_mode;
  }

  inline void SetMode(enOp::mode_e md){
    m_mode = md;
  }


  inline void UpdateState(){
    enum{in, out, _max};
    std::array<uint8_t, _max> data {};

    uint8_t gpio {};
    gpio |= gpioPinRead(m_cfg.sw_pin_start) << 0;
    gpio |= gpioPinRead(m_cfg.sw_pin_stop)  << 1;
    gpio |= gpioPinRead(m_cfg.sw_pin_reset) << 2;
    gpio |= gpioPinRead(m_cfg.sw_pin_estop) << 3;
    data[in] = gpio;

    gpio = 0x00;
    gpio |= gpioPinRead(m_cfg.lamp_pin_start) << 0;
    gpio |= gpioPinRead(m_cfg.lamp_pin_stop)  << 1;
    gpio |= gpioPinRead(m_cfg.lamp_pin_reset) << 2;
    data[out] = gpio;

    m_cfg.ptr_mcu_io->m_sysio.system_io = (uint16_t)((uint16_t)data[in]<<0 | (uint16_t)data[out]<<8) ;
  }

  inline bool GetPressed(enOp::panel_e op_sw){
    switch (op_sw)
    {
      case enOp::panel_e::SW_START:   return gpioPinRead(m_cfg.sw_pin_start);
      case enOp::panel_e::SW_STOP:    return gpioPinRead(m_cfg.sw_pin_stop);
      case enOp::panel_e::SW_RESET:   return gpioPinRead(m_cfg.sw_pin_reset);
      case enOp::panel_e::SW_ESTOP:   return !gpioPinRead(m_cfg.sw_pin_estop); //reverse
      default:        return false;
    }
  }

  inline void LampOnOff(lamp_e lamp, bool state = true){
    switch (lamp) {
      case LAMP_START:
        gpioPinWrite(m_cfg.lamp_pin_start, state);
        break;
      case LAMP_STOP:
        gpioPinWrite(m_cfg.lamp_pin_stop, state);
        break;
      case LAMP_RESET:
        gpioPinWrite(m_cfg.lamp_pin_reset, state);
        break;
      default:
        break;
    };
  }

  inline void LampToggle(lamp_e lamp){
    switch (lamp) {
      case LAMP_START:
        gpioPinToggle(m_cfg.lamp_pin_start);
        break;
      case LAMP_STOP:
        gpioPinToggle(m_cfg.lamp_pin_stop);
        break;
      case LAMP_RESET:
        gpioPinToggle(m_cfg.lamp_pin_reset);
        break;
      default:
        break;
    };
  }


};





#endif /* AP__INC_ENOP_HPP_ */
