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
  enum mode_e :uint8_t
  {
    READY = 0x10,
    AUTORUN,
    STOP,
    DRY_RUN,
  };

  enum status_e :uint8_t
  {
    INIT = 0x20,
    ERR_STOP,
    STEP_STOP,
    RUN_READY,
    RUN,
  };

  enum panel_e :uint8_t
  {
    SW_START = 0x30,
    SW_STOP,
    SW_RESET,
    SW_ESTOP,
  };

  enum lamp_e :uint8_t
  {
    LAMP_START = 0x40,
    LAMP_STOP,
    LAMP_RESET,
  };

  struct cfg_t
  {
  	ap_reg* ptr_mcu_reg{};
  	ap_io * ptr_mcu_io{};

  	GPIO_TypeDef * key_start_port{};
  	uint16_t       key_start_pin{};
  	GPIO_TypeDef * key_stop_port{};
  	uint16_t       key_stop_pin{};
  	GPIO_TypeDef * key_reset_port{};
  	uint16_t       key_reset_pin{};
  	GPIO_TypeDef * key_estop_port{};
  	uint16_t       key_estop_pin{};

  	GPIO_TypeDef * lamp_start_port{};
  	uint16_t       lamp_start_pin{};
  	GPIO_TypeDef * lamp_stop_port{};
  	uint16_t       lamp_stop_pin{};
  	GPIO_TypeDef * lamp_reset_port{};
  	uint16_t       lamp_reset_pin{};

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

  inline GPIO_PinState gpioRead(uint8_t key) {
  	switch (key) {
  		case SW_START:
  			return HAL_GPIO_ReadPin(m_cfg.key_start_port,m_cfg.key_start_pin);
  		case SW_STOP:
  			return HAL_GPIO_ReadPin(m_cfg.key_stop_port,m_cfg.key_stop_pin);
  		case SW_RESET:
  			return HAL_GPIO_ReadPin(m_cfg.key_reset_port,m_cfg.key_reset_pin);
  		case SW_ESTOP:
  			if (HAL_GPIO_ReadPin(m_cfg.key_estop_port,m_cfg.key_estop_pin) == GPIO_PIN_RESET)// reverse
  				return GPIO_PIN_SET;
  			else
  				return GPIO_PIN_RESET;
  		case LAMP_START:
  			return HAL_GPIO_ReadPin(m_cfg.lamp_start_port,m_cfg.lamp_start_pin);
  		case LAMP_STOP:
  			return HAL_GPIO_ReadPin(m_cfg.lamp_stop_port,m_cfg.lamp_stop_pin);
  		case LAMP_RESET:
  			return HAL_GPIO_ReadPin(m_cfg.lamp_reset_port,m_cfg.lamp_reset_pin);
  		default:
  			break;
		}
  	return GPIO_PIN_RESET;
  }
  inline void UpdateState(){
    enum{in, out, _max};
    std::array<uint8_t, _max> data {};

    uint8_t gpio {};
    gpio |= gpioRead(SW_START) << 0;
    gpio |= gpioRead(SW_STOP)  << 1;
    gpio |= gpioRead(SW_RESET) << 2;
    gpio |= gpioRead(SW_ESTOP) << 3;
    data[in] = gpio;

    gpio = 0x00;
    gpio |= gpioRead(LAMP_START) << 0;
    gpio |= gpioRead(LAMP_STOP)  << 1;
    gpio |= gpioRead(LAMP_RESET) << 2;
    data[out] = gpio;

    m_cfg.ptr_mcu_io->m_sysio.system_io = (uint16_t)((uint16_t)data[in]<<0 | (uint16_t)data[out]<<8) ;
  }

  inline bool GetPressed(enOp::panel_e op_sw){
  	return (gpioRead((uint8_t)op_sw) == GPIO_PIN_SET);
  }

  inline void LampOnOff(lamp_e lamp, bool state = true){
  	GPIO_PinState set_state = state?GPIO_PIN_SET:GPIO_PIN_RESET;
    switch (lamp) {
      case LAMP_START:
      	HAL_GPIO_WritePin(m_cfg.lamp_start_port, m_cfg.lamp_start_pin, set_state );
        break;
      case LAMP_STOP:
      	HAL_GPIO_WritePin(m_cfg.lamp_stop_port, m_cfg.lamp_stop_pin, set_state );
        break;
      case LAMP_RESET:
      	HAL_GPIO_WritePin(m_cfg.lamp_reset_port, m_cfg.lamp_reset_pin, set_state );
        break;
      default:
        break;
    };
  }

  inline void LampToggle(lamp_e lamp){
    switch (lamp) {
      case LAMP_START:
      	HAL_GPIO_TogglePin(m_cfg.lamp_start_port, m_cfg.lamp_start_pin);
        break;
      case LAMP_STOP:
      	HAL_GPIO_TogglePin(m_cfg.lamp_stop_port, m_cfg.lamp_stop_pin);
        break;
      case LAMP_RESET:
      	HAL_GPIO_TogglePin(m_cfg.lamp_reset_port, m_cfg.lamp_reset_pin);
        break;
      default:
        break;
    };
  }


};





#endif /* AP__INC_ENOP_HPP_ */
