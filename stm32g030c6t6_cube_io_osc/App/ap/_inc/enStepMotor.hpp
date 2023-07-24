/*
 * enStepMotor.hpp
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */

#ifndef AP__INC_ENSTEPMOTOR_HPP_
#define AP__INC_ENSTEPMOTOR_HPP_


constexpr int MOTOR_PULSES_PER_REV = 1600;  // Motor pulses per revolution, motor step 1.8 degree, tmc2209 microstep 1/8
constexpr int MOTOR_MAX_PRM = 500;// Maximum motor RPM
constexpr int MOTOR_ACCEL_TIME_MS = 200;  // Acceleration and deceleration time

struct enStepMotor
{
  struct cfg_t{
    uint8_t       motor_id{};
    tim_tbl_t*    ptr_timer{};
    ap_io *ptr_mcu_io{};
    uint32_t out_addr_step{};
    uint32_t out_addr_dir{};
    uint32_t out_addr_enable{};
    uint32_t in_addr_org{};
    uint32_t in_addr_ccw_limit{};
    uint32_t in_addr_cw_limit{};
 /*
    GPIO_TypeDef * gpio_enable_port{};
  	uint16_t       gpio_enable_pin{};
    GPIO_TypeDef * gpio_dir_port{};
  	uint16_t       gpio_dir_pin{};
    GPIO_TypeDef * gpio_pulse_port{};
  	uint16_t       gpio_pulse_pin{};
*/
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


  enum :uint8_t {motor_run,motor_stop};


  // Motor control variables
  volatile uint32_t accel_index;
  volatile uint32_t vel_index;
  volatile uint32_t decel_index;
  volatile int8_t   dir;          // direction

  volatile uint32_t step_position; // 위치   스텝
  volatile uint32_t total_steps;   // 가야할 스텝

  volatile uint32_t speed;  //
  volatile uint32_t step_count;  //
  volatile uint32_t count;
  volatile bool     move_done ;
  volatile uint8_t  state;


  /*
    volatile uint32_t steps_per_s;  // Steps per second
    volatile uint32_t steps_per_ms;  // Steps per millisecond
    volatile uint32_t curr_speed;  // Current speed in steps per second
    volatile uint32_t accel_steps;  // Steps needed to reach maximum speed during acceleration
    volatile uint32_t decel_steps;  // Steps needed to stop the motor during deceleration
    volatile uint32_t steps_to_target;  // Steps remaining to reach target revolutions

    volatile uint32_t accel_count;  // Number of steps taken during acceleration
    volatile uint32_t decel_count;  // Number of steps taken during deceleration
    volatile uint32_t time_since_last_step;  // Time since the last step in microseconds
    volatile uint32_t last_step_time;  // Time of the last step in microseconds
   */
  prc_step_t step;
  cfg_t m_cfg;
  bool m_isEnable;
  bool m_isInit;

public:
  enStepMotor ():accel_index{},vel_index{},decel_index{},dir{},
  step_position{}, total_steps{}, speed{}, step_count{}, count{}, move_done{},
  state{}, step{} , m_cfg{} ,m_isEnable{}, m_isInit{} {
  };
  ~enStepMotor (){};
  /*
    enStepMotor (const enStepMotor &other){};
    enStepMotor (enStepMotor &&other){};

    enStepMotor& operator= (enStepMotor &&other){
      return *this;
    };
    enStepMotor& operator= (const enStepMotor &other){
      return *this;
    };
   */

private:
  inline void delay_us(uint32_t us)
  {
    volatile uint32_t i;
    for (i = 0; i < us; i++)
    {    }
    // for (i=0; i<us*10; i++) {
    //   __NOP();
    // }
  }

  uint S_table_len(uint *table);
  void S_curve_gen(int steps);

public:
  inline void Init(cfg_t &cfg) {
    // Initialize GPIO

    // Initialize timer

    // Calculate motor control variables

    // Set initial motor control variables

    // Start the timer and motor

    //timStart(_DEF_TIM1);
    //timAttachCB(_DEF_TIM1, this, ISR_axis);

    m_cfg = cfg;
    m_cfg.ptr_timer->obj = this;
    m_cfg.ptr_timer->func_cb = ISR_axis;
  };


  /* test code 1cycle for and backward*/
  inline void test_stepper_constant_accel()
  {
    constexpr auto STEPS = MOTOR_PULSES_PER_REV;

    uint32_t delays[STEPS]{};
    float angle =1;
    float accel = 0.01;
    float c0 = 2000 * sqrt(2 * angle/accel)*0.67703;
    float last_delay = 0;
    //float max_feq = (MOTOR_MAX_PRM/60)*MOTOR_PULSES_PER_REV;
    uint32_t high_speed = /*(1/max_feq)*1000000 */360;


    float d = c0;

    for (uint32_t i =0; i <STEPS; i++)
    {
      if ( i > 0 )
        d = last_delay - (2 * last_delay)/(4*i+1);

      if (d < high_speed)
        d = high_speed;

      delays[i] = d;
      last_delay = d;
    }

    // use delays from the array, forward
    for (int i=0; i<STEPS ; i++)
    {
      StepPulse();
      delay_us(delays[i]);
    }

    // high speed
    for (int i=0; i<STEPS*4; i++)
    {
      StepPulse();
      delay_us(high_speed);
    }

    // use delays from the array, backward
    for (int i=0; i<STEPS; i++)
    {
      StepPulse();
      delay_us(delays[STEPS-i-1]);
    }

  }


  void Run(int steps, uint32_t speed);
  void SetStep(int steps, uint32_t speed);
  void SetRel(int steps, uint32_t speed);

  void RunAndWait();

  inline void Stop(){
    timDisableISR(_DEF_TIM1);
  }

  inline void Enable(){
    //gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_LOW);
    m_cfg.ptr_mcu_io->OutputOn(m_cfg.out_addr_enable);
  };

  inline void Disable(){
    //gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_HIGH);
    m_cfg.ptr_mcu_io->OutputOff(m_cfg.out_addr_enable);
  };

  inline void StepPulse(){
    //LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
    //LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
    m_cfg.ptr_mcu_io->OutputToggle(m_cfg.out_addr_step);
  };

  inline void Direction(bool is_ccw){
    //gpioPinWrite(_GPIO_MOT_X_DIR, is_ccw);
    if (is_ccw)
      m_cfg.ptr_mcu_io->OutputOn(m_cfg.out_addr_dir);
    else
      m_cfg.ptr_mcu_io->OutputOff(m_cfg.out_addr_dir);
  };


  static void ISR_axis(void* obj, void* w_param, void* l_param);

  inline static void ISR_axis(void){

  };

};
// end of class


#endif /* AP__INC_ENSTEPMOTOR_HPP_ */
