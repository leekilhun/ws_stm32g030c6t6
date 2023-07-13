/*
 * enStepMotor.cpp
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */





#include "ap.hpp"
#include "s_curve.h"

#include "enStepMotor.hpp"


static uint8_t remaining_flag {};


void enStepMotor::SetRel(int steps, uint32_t speed)
{
  int position = steps ;
  this->dir = (steps < 0);//CCW
  this->Direction((steps < 0));

  S_curve_gen(abs(position));
  this->speed = u32_Speed[constrain(speed,10,100)];
  this->move_done = false;

  remaining_flag |= (1 << m_cfg.motor_id);
}

void enStepMotor::SetStep(int steps, uint32_t speed)
{

  int position = steps - this->step_position ;
  this->dir = (position < 0);//CCW
  this->Direction((position < 0));

  S_curve_gen(abs(position));
  this->speed = u32_Speed[constrain(speed,10,100)];
  this->move_done = false;

  remaining_flag |= (1 << m_cfg.motor_id);
}

void enStepMotor::Run(int steps, uint32_t speed)
{
  timEnableISR(_DEF_TIM1);
  this->SetStep(steps,speed);

}


void enStepMotor::RunAndWait()
{
  timEnableISR(_DEF_TIM1);

  constexpr uint32_t timeout = 1000*5;
  uint32_t pre_ms = millis();

  while (remaining_flag)
  {
    if ((millis() - pre_ms) > timeout)
      break;
  }
}


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
  this->state        = motor_run;
  this->count        = 0;

}



// Timer interrupt handler
void enStepMotor::ISR_axis(void* obj, void* w_param, void* l_param){
  enStepMotor* pM = (enStepMotor*)obj;
  TIM_TypeDef * ptr_tm = pM->m_cfg.ptr_timer->pHandle->Instance;

  uint8_t time_id = 0 ;
  if(pM->m_cfg.motor_id == 0) // ptr_tm == TIM3
  {
    time_id = _DEF_TIM1;
  }

  pM->StepPulse();

  // pM->edge ^= 1; //XOR
  // if (pM->edge == 1)
  // {




  //When the motor moves in the reverse direction, it stops when it reaches the origin switch.
  /*
    if (pM->dir == CCW)
    {
      if (0) // is Checked origin S/W
        pM->state = motor_stop; // stop
    }
   */

  if(pM->state == motor_run )
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
        ptr_tm->CCR1 = pM->speed / s_curve_table[pM->accel_index-1];
        ptr_tm->ARR  = pM->speed / s_curve_table[pM->accel_index-1];
      }
      // deceleration
      else
      {
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
      pM->state = motor_stop;
    }
    else
    {
      if(pM->dir == CW)     {pM->step_position++;}
      else                  {pM->step_position--;}

      pM->step_count++;
    }

  }
  else
  {
    timDisableISR(time_id);
    pM->count = 0;
    pM->step_count = 0;
    pM->move_done = true;
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

  if (pM->move_done == true) remaining_flag &= ~(1 << pM->m_cfg.motor_id); //Off  motor timer_cal

};


