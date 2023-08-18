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

class enStepMotor
{
public:
  struct cfg_t{
    uint8_t       motor_id{};
    tim_tbl_t*    ptr_timer{};

    GPIO_TypeDef * gpio_port_step{};
  	uint16_t       gpio_pin_step{};
  	GPIO_TypeDef * gpio_port_dir{};
  	uint16_t       gpio_pin_dir{};
  	GPIO_TypeDef * gpio_port_enable{};
  	uint16_t       gpio_pin_enable{};

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
public:

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
  prc_step_t m_step;
  cfg_t m_cfg;

public:
  enStepMotor ():accel_index{},vel_index{},decel_index{},dir{},
  step_position{}, total_steps{}, speed{}, step_count{}, count{}, move_done{},
  state{}, m_step{} , m_cfg{} {

  };
  virtual ~enStepMotor (){};
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

  uint S_table_len(uint* table);
  void S_curve_gen(int steps);

public:
  inline void Init(cfg_t &cfg) {
    // Initialize GPIO

    // Initialize timer

    // Calculate motor control variables

    // Set initial motor control variables

    // Start the timer and motor

    timStart(_DEF_TIM1);
    timAttachCB(_DEF_TIM1, this, ISR_axis);
    m_cfg = cfg;

  };


  /* test code 1cycle for and backward*/
  inline void test_stepper_constant_accel()
  {
    constexpr auto STEPS = MOTOR_PULSES_PER_REV;

    auto delay_us = [](uint32_t us)
    {
     volatile uint32_t i;
    for (i=0; i<us; i++) { }        
    };

    uint32_t delays[STEPS]{};
    float angle =1.0;
    float accel = 0.01;
    float c0 = 2000.0 * sqrt(2.0 * angle/accel)*0.67703;
    float last_delay = 0.0;
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
    HAL_GPIO_WritePin(m_cfg.gpio_port_enable, m_cfg.gpio_pin_enable, GPIO_PIN_SET);
  };

  inline void Disable(){
    HAL_GPIO_WritePin(m_cfg.gpio_port_enable, m_cfg.gpio_pin_enable, GPIO_PIN_RESET);
  };

  inline void StepPulse(){
    //LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
    //LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
    HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_SET);
    HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_RESET);
  };

  inline void Direction(bool is_ccw)
  {
    if (is_ccw)
      HAL_GPIO_WritePin(m_cfg.gpio_port_dir, m_cfg.gpio_pin_dir, GPIO_PIN_SET);
    else
      HAL_GPIO_WritePin(m_cfg.gpio_port_dir, m_cfg.gpio_pin_dir, GPIO_PIN_RESET);
  };

  static void ISR_axis(void* obj, void* w_param, void* l_param);

  inline static void ISR_axis(void){

  };

};
// end of class


#endif /* AP__INC_ENSTEPMOTOR_HPP_ */
