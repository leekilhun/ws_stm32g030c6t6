/*
 * enStepMotor.cpp
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */





#include "ap.hpp"
#include "s_curve.h"

#include "enStepMotor.hpp"

using namespace MOTOR;

static uint8_t remaining_flag {};


void enStepMotor::SetRel(int steps, uint32_t speed)
{
  int position = steps ;
  this->dir = (steps < 0);//CCW
  this->Direction((position < 0));

  S_curve_gen(abs(position));
  this->speed = u32_Speed[constrain(speed,10,100)];
  this->move_done = false;

  remaining_flag |= (1 << m_cfg.obj_idx);
}

void enStepMotor::SetStep(int steps, uint32_t speed)
{
  int position = steps - this->step_position;
  this->dir = (position < 0); // CCW
  this->Direction((position < 0));

  S_curve_gen(abs(position));
  this->speed = u32_Speed[constrain(speed,1,100)];
  this->move_done = false;

  remaining_flag |= (1 << m_cfg.obj_idx);
}

errno_t enStepMotor::Run(int steps, uint32_t speed)
{
  if (m_status.motor_enabled == false)
  {
    LOG_PRINT("motor_enabled Fail! motor_idx[0x%02X]", m_cfg.obj_idx);
    return -1;
  }
  if (m_status.initializ_cplt == false)
  {
    LOG_PRINT("initializ_cplt Fail! motor_idx[0x%02X]", m_cfg.obj_idx);
    return -2;
  }

  //LOG_PRINT("id[0x%02X], steps[%d], speed[%d]%", m_cfg.obj_idx, steps, speed);
  this->SetStep(steps,speed);
  timEnableISR(m_cfg.timer_id);
  return ERROR_SUCCESS;
}

// void enStepMotor::RunAndWait()
// {
//   timEnableISR(m_cfg.timer_id);

//   constexpr uint32_t timeout = 1000*5;
//   uint32_t pre_ms = millis();

//   while (remaining_flag)
//   {
//     if ((millis() - pre_ms) > timeout)
//       break;
//   }
// }


uint enStepMotor::S_table_len(uint* table)
{
  int i;
  constexpr int  TABLE_PSC  =  1;
  constexpr int  MAX_FREQ   = 800;
  for(i = 0;i < S_CURVE_TABLE_LEN;i++)
  {
    table[i] = table[i] / TABLE_PSC;

    if(table[i] >= MAX_FREQ-2) return i;
  }

  return S_CURVE_TABLE_LEN;
}


void enStepMotor::S_curve_gen(int steps)
{

  this->accel_index  = S_table_len(s_curve_table);
  this->decel_index  = accel_index;
  this->vel_index    = steps - accel_index - decel_index;
  this->total_steps  = steps;
  //this->state        = motor_run;
  this->m_status.moving = true;
  this->m_status.constant_moving = false;
  this->m_status.decel_moving = false;
  this->m_status.accel_moving = false;
  this->m_status.in_position = false;
  this->count = 0;
}

bool enStepMotor::ProcessZeroSet()
{
  enum
  {
    STEP_INIT,
    STEP_TODO,
    STEP_TIMEOUT,
    STEP_ZERO_CPLT,
    STEP_CHECK_DISABLE,
    STEP_CHECK_DISABLE_START,
    STEP_CHECK_DISABLE_WAIT,
    STEP_CHECK_DISABLE_END,
    STEP_CHECK_DIR,
    STEP_CHECK_DIR_START,
    STEP_CHECK_DIR_WAIT,
    STEP_CHECK_DIR_END,

  };

  // if (m_cfg.obj_idx == AP_OBJ::MOTOR_C)
  //   LOG_PRINT("step [%d]  wait[%d]", m_step.curr_step, m_step.wait_step);

  switch (m_step.GetStep())
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
    m_status.zero_homing = true;

    Direction(false);
    if (m_cfg.sm_type == stepmotorType_e::ccw_to_zero)
    {
      m_step.SetStep(STEP_CHECK_DIR);
    }
    else if (m_cfg.sm_type == stepmotorType_e::cw_to_zero)
    {
      m_step.SetStep(STEP_CHECK_DIR);
    }
    else
    {
      m_step.SetStep(STEP_CHECK_DISABLE);
    }
  }
  break;
  /*######################################################
    process completed
  ######################################################*/
  case STEP_ZERO_CPLT:
  {
    m_status.zero_homing = false;
    m_status.initializ_cplt = true;
    Stop();
    return true;
  }
  break;
  /*######################################################
    timeout
  ######################################################*/
  case STEP_TIMEOUT:
  {
    m_status.zero_homing = false;
    m_status.drive_fault = true;
    m_status.err_timeout = true;
    Stop();
    return true;
  }
  break;
  /*######################################################
    STEP_CHECK_DISABLE
  ######################################################*/
  case STEP_CHECK_DISABLE:
  {
    m_step.wait_resp = false;
    m_step.wait_step = 0;
    m_step.retry_cnt = 0;
    m_step.SetStep(STEP_CHECK_DISABLE_START);
  }
  break;

  case STEP_CHECK_DISABLE_START:
  {
    Disable();
    m_step.SetStep(STEP_CHECK_DISABLE_WAIT);
  }
  break;

  case STEP_CHECK_DISABLE_WAIT:
  {
    if (m_step.LessThan(1'000))
      break;

    m_step.SetStep(STEP_CHECK_DISABLE_END);  
    if (m_cfg.ptr_io->IsOn(m_cfg.io_input_idx_zero) && m_cfg.ptr_io->IsOn(m_cfg.io_input_idx_zero_ex))
    {
      m_step.SetStep(STEP_CHECK_DISABLE_END);
      Enable();
    }
    else
    {
      if (m_step.retry_cnt++ < 5)
        m_step.SetStep(STEP_CHECK_DISABLE_START);
      else
        m_step.SetStep(STEP_TIMEOUT);
    }
  }
  break;

  case STEP_CHECK_DISABLE_END:
  {
    m_step.SetStep(STEP_ZERO_CPLT);
    break;
  }
  break;
  /*######################################################
    STEP_CHECK_DIR
  ######################################################*/
  case STEP_CHECK_DIR:
  {
    m_step.wait_resp = false;
    m_step.wait_step = 0;
    m_step.retry_cnt = 0;
    step_count = 0;
    
    constexpr uint8_t def_step_move_sensor_to_detect = 1;
    if (m_cfg.ptr_io->IsOn(m_cfg.io_input_idx_zero))
    {
      m_step.wait_step = def_step_move_sensor_to_detect;
    }    
    m_step.SetStep(STEP_CHECK_DIR_START);
  }
  break;

  case STEP_CHECK_DIR_START:
  {
    enum : uint8_t
    {
      step_move_out_of_sensor,
      step_move_sensor_to_detect,
      step_move_to_offset_pos,
      step_slow_sensor_pos,
      step_cplt
    };

    switch (m_step.wait_step)
    {
    case step_move_out_of_sensor: __attribute__((fallthrough));
    case step_move_sensor_to_detect:
      StepPulse();
      break;

    case step_move_to_offset_pos:
      step_count++;
      StepPulse();
      break;

    case step_slow_sensor_pos:
      StepPulse();
      break;

    case step_cplt:
      break;

    default:
      break;
    }

    m_step.SetStep(STEP_CHECK_DIR_WAIT);
  }
  break;   

  case STEP_CHECK_DIR_WAIT:
  {    
    // if (m_step.LessThan(30))
    //   break;
    enum : uint8_t
    {
      step_move_out_of_sensor,
      step_move_sensor_to_detect,
      step_move_to_offset_pos,
      step_slow_sensor_pos,
      step_cplt
    };

    m_step.SetStep(STEP_CHECK_DIR_START);

    // true = current sensor detection status
    volatile bool is_sensor_in = (m_cfg.ptr_io->IsOff(m_cfg.io_input_idx_zero));
    switch (m_step.wait_step)
    {
    case step_move_out_of_sensor:
      if (is_sensor_in)
        m_step.wait_step = step_move_sensor_to_detect;
      break;

    case step_move_sensor_to_detect:
      if (is_sensor_in)
      {
        step_count = 0;
        Direction(true);
        m_step.wait_step = step_move_to_offset_pos;
      }
      break;

    case step_move_to_offset_pos:
      if ((step_count < m_cfg.org_offset_cnt) == false)
      {
        step_count = 0;
        Direction(false);
        vel_index = def_org_slow_interrupt_time_us; //set timer interrupt 300us
        m_step.wait_step = step_slow_sensor_pos;
      }
      break;

    case step_slow_sensor_pos:
     if (is_sensor_in)
      {
        step_count = 0;
        vel_index = 0;
        m_step.wait_step = step_cplt;
      }

      break;

    case step_cplt:
      m_step.SetStep(STEP_CHECK_DIR_END);
      break;

    default:
      break;
    }
  }
  break;

  case STEP_CHECK_DIR_END:
  {
    m_step.SetStep(STEP_ZERO_CPLT);
    break;
  }
  break;

  default:
    break;
  }
  // end of switch

  return false;
}

//uint32_t timer_arr_cnt = 0;
// Timer interrupt handler
void enStepMotor::ISR_axis(void* obj, void* w_param, void* l_param){
  enStepMotor* pM = (enStepMotor*)obj;
  TIM_TypeDef * ptr_tm = pM->m_cfg.ptr_timer->pHandle->Instance;

  uint8_t time_id = pM->m_cfg.timer_id;

  // zero set process
  {
    if (pM->m_status.initializ_cplt == false)
    {
      if (pM->ProcessZeroSet() == false)
      {
        //ptr_tm->CCR1 = (2 - 1);
        volatile uint32_t interrup_us = pM->vel_index;
        ptr_tm->ARR = (interrup_us - 1);//(2 - 1); // 2us timer
      }
      return;
    }
  }
  // end of zero set

  pM->StepPulse();

  // pM->edge ^= 1; //XOR
  // if (pM->edge == 1)


  //When the motor moves in the reverse direction, it stops when it reaches the origin switch.
  /*
    if (pM->dir == CCW)
    {
      if (0) // is Checked origin S/W
        pM->state = motor_stop; // stop
    }
   */

  
  //LOG_PRINT("timer id [%d], state[%d], step_count[%d], total [%d]", pM->m_cfg.timer_id,pM->state ,pM->step_count, pM->total_steps);

  //if(pM->state == motor_run )
  if (pM->m_status.moving)
  {
    if(pM->accel_index != 0)
    {
      // decel_index
      if(pM->step_count < pM->accel_index)
      {
        ptr_tm->CCR1 = pM->speed / s_curve_table[pM->step_count];
        ptr_tm->ARR  = pM->speed / s_curve_table[pM->step_count];
      }
      // max speed
      else if((pM->step_count >= pM->accel_index)
          && (pM->step_count <= pM->accel_index + pM->vel_index))
      {
        pM->m_status.constant_moving = true;        
        pM->m_status.accel_moving = false;
        ptr_tm->CCR1 = pM->speed / s_curve_table[pM->accel_index-1];
        ptr_tm->ARR  = pM->speed / s_curve_table[pM->accel_index-1];
      }
      // deceleration
      else
      {
        pM->m_status.decel_moving = true;
        ptr_tm->CCR1 = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
        ptr_tm->ARR  = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
        pM->count++;
      }
    }

    if(pM->step_count == pM->total_steps)
    {
      timDisableISR(time_id);
      pM->count = 0;
      pM->step_count = 0;
      pM->move_done = true;
      //pM->state = motor_stop;
      pM->m_status.in_position = true;
      pM->m_status.moving = false;
      pM->m_status.constant_moving = false;
      pM->m_status.decel_moving = false;
      pM->m_status.accel_moving = false;
    }
    else
    {
      if(pM->dir == CW)     {pM->step_position++;}
      else                  {pM->step_position--;}

      pM->m_status.accel_moving = true;
      pM->step_count++;
    }

  }
  else
  {
    timDisableISR(time_id);
    pM->count = 0;
    pM->step_count = 0;
    pM->move_done = true;
    pM->m_status.in_position = true;
    pM->m_status.moving = false;
    pM->m_status.constant_moving = false;
    pM->m_status.decel_moving = false;
    pM->m_status.accel_moving = false;
  }

  /* switch (pM->state)
    {
      //======
      case 0:
      {

      }
      //======
      case 1:
        timDisableISR(_DEF_TIM1);
        pM->count = 0;
        pM->stepCount = 0;
        pM->moveDone = true;
        break;
        //======
      case 2:
        timDisableISR(_DEF_TIM1);
        pM->count = 0;
        pM->stepCount = 0;
        pM->moveDone = true;
        pM->stepPosition = 0;
        break;
        //======
      default:
        break;
    }*/
  // }

  if (pM->move_done == true) remaining_flag &= ~(1 << pM->m_cfg.obj_idx); //Off  motor timer_cal

};


