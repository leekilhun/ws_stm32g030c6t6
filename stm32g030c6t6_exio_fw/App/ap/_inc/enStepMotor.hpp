/*
 * enStepMotor.hpp
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */
#pragma once
#ifndef AP__INC_ENSTEPMOTOR_HPP_
#define AP__INC_ENSTEPMOTOR_HPP_

#include "ap_def.hpp"

#ifdef _USE_AP_P_STEP_MOTOR

namespace MOTOR
{

  enum class stepmotorType_e
  {
    floating_zero, // set the initial position to zero and increase in the (cw) reverse direction.
    cw_to_zero,    //(clock wise) increase in the (cw) reverse direction.
    ccw_to_zero,   //(counter clock wise) increase in the (ccw) Forward direction.
  };

  class enStepMotor
  {
  public:
    struct cfg_t
    {
      uint8_t obj_idx{};
      uint8_t timer_id{};
      tim_tbl_t *ptr_timer{};
      ap_io *ptr_io{};
      stepmotorType_e sm_type{};
      uint32_t org_offset_cnt{};
      uint16_t io_input_idx_zero{};
      uint16_t io_input_idx_zero_ex{};

      GPIO_TypeDef *gpio_port_step{};
      uint16_t gpio_pin_step{};
      GPIO_TypeDef *gpio_port_dir{};
      uint16_t gpio_pin_dir{};
      GPIO_TypeDef *gpio_port_enable{};
      uint16_t gpio_pin_enable{};

      cfg_t() = default;

      // copy constructor
      cfg_t(const cfg_t &rhs) = default;
      // copy assignment operator
      cfg_t &operator=(const cfg_t &rhs) = default;
      // move constructor
      cfg_t(cfg_t &&rhs) = default;
      // move assignment operator
      cfg_t &operator=(cfg_t &&rhs) = default;
    };

    union motor_status
    {
      uint16_t sc_status;
      struct
      {
        unsigned motor_enabled : 1;   // = 0x00000001;
        unsigned drive_fault : 1;     // = 0x00000002;
        unsigned in_position : 1;     // = 0x00000004;
        unsigned moving : 1;          // = 0x00000008;
        unsigned accel_moving : 1;    // = 0x00000010;
        unsigned decel_moving : 1;    // = 0x00000020;
        unsigned constant_moving : 1; // = 0x00000040;
        unsigned initializ_cplt : 1;  // = 0x00000080;
        unsigned zero_homing : 1;     // = 0x00000100;
        unsigned not_used0 : 1;       // = 0x00000200;
        unsigned not_used1 : 1;       // = 0x00000400;
        unsigned not_used2 : 1;       // = 0x00000800;
        unsigned not_used3 : 1;       // = 0x00001000;
        unsigned not_used4 : 1;       // = 0x00002000;
        unsigned not_used5 : 1;       // = 0x00004000;
        unsigned err_timeout : 1;     // = 0x00008000;
      };
    } m_status{};

    // enum motor_state: uint8_t
    // {
    //   motor_run,
    //   motor_stop
    // };

  public:
    // Motor control variables
    volatile uint32_t accel_index;
    volatile uint32_t vel_index;
    volatile uint32_t decel_index;
    volatile int8_t dir; // direction

    volatile uint32_t step_position; // 위치   스텝
    volatile uint32_t total_steps;   // 가야할 스텝

    volatile uint32_t speed;      //
    volatile uint32_t step_count; //
    volatile uint32_t count;
    volatile bool move_done;
    // volatile uint8_t state;

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
    // bool m_enable;
  protected:
    static constexpr uint16_t def_org_fast_interrupt_time_us = 10;
    static constexpr uint16_t def_org_slow_interrupt_time_us = 500;
    static constexpr uint16_t def_default_offset_pulse = 300;

    static constexpr int def_motor_pulse_per_resolution = 1600; // Motor pulses per revolution, motor step 1.8 degree, tmc2209 microstep 1/8
    static constexpr int def_motor_max_rpm = 500;               // Maximum motor RPM
    static constexpr int def_motor_accel_time_ms = 200;         // Acceleration and deceleration time

  public:
    enStepMotor() : accel_index{}, vel_index{}, decel_index{}, dir{},
                    step_position{}, total_steps{}, speed{}, step_count{}, count{}, move_done{true},
                    /*state{},*/ m_step{}, m_cfg{} /*, m_enable{}*/ {

                                           };
    ~enStepMotor(){};
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
    uint S_table_len(uint *table);
    void S_curve_gen(int steps);

  public:
    inline void Init(cfg_t &cfg)
    {
      // Initialize GPIO

      // Initialize timer

      // Calculate motor control variables

      // Set initial motor control variables

      // Start the timer and motor

      timAttachCB(cfg.timer_id, this, ISR_axis);
      m_cfg = cfg;
      m_cfg.org_offset_cnt = def_default_offset_pulse;
      // state = motor_stop;

      Disable();
    };

    /*
    skr e3
    hardware microstep resolution
    x 1/8
    y 1/64
    z 1/32
    e 1/16
    */

#if 0
    /* test code 1cycle for and backward*/
    inline void test_stepper_constant_accel()
    {
      constexpr auto STEPS = MOTOR_PULSES_PER_REV;

      auto delay_us = [](uint32_t us)
      {
        volatile uint32_t i;
        for (i = 0; i < us; i++)
        {
        }
      };
      state = motor_run;
      uint32_t delays[STEPS]{};
      float angle = 1.0;
      float accel = 0.01;
      float c0 = 2000.0 * sqrt(2.0 * angle / accel) * 0.67703;
      float last_delay = 0.0;
      // float max_feq = (MOTOR_MAX_PRM/60)*MOTOR_PULSES_PER_REV;
      uint32_t high_speed = 640; //(1/max_feq)*1'000'000;

      float d = c0;

      for (uint32_t i = 0; i < STEPS; i++)
      {
        if (i > 0)
          d = last_delay - (2 * last_delay) / (4 * i + 1);

        if (d < high_speed)
          d = high_speed;

        delays[i] = d;
        last_delay = d;
      }

      // use delays from the array, forward
      for (int i = 0; i < STEPS; i++)
      {
        StepPulse();
        delay_us(delays[i]);
      }

      // high speed
      for (int i = 0; i < STEPS * 4; i++)
      {
        StepPulse();
        delay_us(high_speed);
      }

      // use delays from the array, backward
      for (int i = 0; i < STEPS; i++)
      {
        StepPulse();
        delay_us(delays[STEPS - i - 1]);
      }

      state = motor_stop;
    }
#endif

    inline void test_constant_run(int steps, uint32_t time)
    {
      // use delays from the array, forward
      for (int i = 0; i < steps; i++)
      {
        // StepPulse();
        // delay_us(time_us);
        HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_RESET);
        delay_us(1);
        HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_SET);
        delay_us(time);
      }
      Disable();
    }

    errno_t SetZeroPosition()
    {
      if (m_status.motor_enabled == false)
        return -1;

      m_step.SetStep(0);

      vel_index = def_org_fast_interrupt_time_us; // timer interrupt us

      timEnableISR(m_cfg.timer_id);
      return ERROR_SUCCESS;
    }

    bool ProcessZeroSet();

    errno_t Run(int steps, uint32_t speed);

    // void RunAndWait();

    inline void Stop()
    {
      timDisableISR(m_cfg.timer_id);
    }

    inline void Enable()
    {
      // m_enable = true;
      m_status.motor_enabled = true;
      HAL_GPIO_WritePin(m_cfg.gpio_port_enable, m_cfg.gpio_pin_enable, GPIO_PIN_RESET);
    };

    inline void Disable()
    {
      // m_enable = false;
      Stop();
      m_status.motor_enabled = false;
      HAL_GPIO_WritePin(m_cfg.gpio_port_enable, m_cfg.gpio_pin_enable, GPIO_PIN_SET);
    };

  private:
    void SetStep(int steps, uint32_t speed);
    void SetRel(int steps, uint32_t speed);
    inline void StepPulse()
    {
      // LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
      // LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
      // HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_SET);
      HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_RESET);
      delay_us(1);
      HAL_GPIO_WritePin(m_cfg.gpio_port_step, m_cfg.gpio_pin_step, GPIO_PIN_SET);
    };

    inline void Direction(bool is_ccw)
    {
      bool direction = is_ccw;
      if (m_cfg.sm_type == stepmotorType_e::cw_to_zero)
        direction = !is_ccw; // reverse

      if (direction)
        HAL_GPIO_WritePin(m_cfg.gpio_port_dir, m_cfg.gpio_pin_dir, GPIO_PIN_RESET);
      else
        HAL_GPIO_WritePin(m_cfg.gpio_port_dir, m_cfg.gpio_pin_dir, GPIO_PIN_SET);
    };

    static void ISR_axis(void *obj, void *w_param, void *l_param);

    inline static void ISR_axis(void){};
  };
  // end of class

}
// end of namespce


#endif


#endif /* AP__INC_ENSTEPMOTOR_HPP_ */
