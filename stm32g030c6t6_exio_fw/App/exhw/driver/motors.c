/*
 * motors.c
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */





#include "motors.h"

#ifdef _USE_EXHW_MOTORS


#include "tim.h"
#include "s_curve.h"

#define RUN   0
#define STOP  1
#define TABLE_PSC     1
#define MAX_FREQ    800


motor_tbl_t  motor_tbl[AXIS_MAX];
//static tim_tbl_t *pTimer;


static void motorFuncStep(axis_dir axis);
static void motorFuncDir(axis_dir axis, int dir);

static void motorISR_Axis1(void);
static void motorISR_Axis2(void);

static uint8_t remainingMotorsFlag = 0;


#ifdef _USE_HW_CLI
static void cliMotors(cli_args_t *args);
#endif

bool motorsInit(void)
{
  timStart(_DEF_TIM1);

  for (int i=0; i<(int)AXIS_MAX; i++)
  {

    motor_tbl[i].axis =(axis_dir)i ;
    motor_tbl[i].p_tim_handle = NULL;
    motor_tbl[i].channel = 0;

    motor_tbl[i].func_dir = NULL;
    motor_tbl[i].func_step = NULL;

    motor_tbl[i].stepPosition = 0;

    motor_tbl[i].dir = 0;
    motor_tbl[i].totalSteps = 0;
    motor_tbl[i].stepCount = 0;

    motor_tbl[i].speedScale = 0;

    motor_tbl[i].state = 0;
    motor_tbl[i].edge = 0;
    motor_tbl[i].is_busy = 0;

    motor_tbl[i].count = 0;
    motor_tbl[i].speed = 0;

    motor_tbl[i].accel_index = 0;
    motor_tbl[i].vel_index = 0;
    motor_tbl[i].decel_index = 0;

    motor_tbl[i].moveDone = false;

    motor_tbl[i].is_init = true;
    motor_tbl[i].is_start =false;

    motorDisable((axis_dir)i);
  }

#ifdef _USE_HW_CLI
cliAdd("motors", cliMotors);
#endif
return true;
}




bool motorStart(axis_dir axis)
{
  switch (axis)
  {
    case AXIS_1:
      motor_tbl[AXIS_1].func_dir                    = motorFuncDir;
      motor_tbl[AXIS_1].func_step                   = motorFuncStep;

      if (timStart(_DEF_TIM1) == false)        {   return false;  }


      motor_tbl[AXIS_1].p_tim_handle                = timGetData(_DEF_TIM1);
      motor_tbl[AXIS_1].p_tim_handle->func_callback = motorISR_Axis1;
      timChangePrescale(_DEF_TIM1, 612);

      // timEnableISR(_DEF_TIM1);
      break;

    case AXIS_2:
      motor_tbl[AXIS_2].func_dir                    = motorFuncDir;
      motor_tbl[AXIS_2].func_step                   = motorFuncStep;

      if (timStart(_DEF_TIM2) == false)       { return false;      }

      motor_tbl[AXIS_2].p_tim_handle                = timGetData(_DEF_TIM2);
      motor_tbl[AXIS_2].p_tim_handle->func_callback = motorISR_Axis2;
      timChangePrescale(_DEF_TIM2, 612);



      timEnableISR(_DEF_TIM2);
      break;

    case AXIS_3:
      break;
    case AXIS_4:
      break;

    case AXIS_MAX:
      break;
  }



  return true;
}


void motorEnable(axis_dir axis)
{
  switch (axis)
  {
    case AXIS_1:
      gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_LOW);
      break;
    case AXIS_2:
      gpioPinWrite(_GPIO_MOT_Y_ENABLE,_DEF_LOW);
      break;

    case AXIS_3:
      break;

    case AXIS_4:
      break;

    case AXIS_MAX:
      gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_LOW);
      gpioPinWrite(_GPIO_MOT_Y_ENABLE,_DEF_LOW);
      break;
  }
}



void motorDisable(axis_dir axis)
{
  switch (axis)
  {
    case AXIS_1:
      gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_HIGH);
      break;
    case AXIS_2:
      gpioPinWrite(_GPIO_MOT_Y_ENABLE,_DEF_HIGH);
      break;

    case AXIS_3:
      break;
    case AXIS_4:
      break;

    case AXIS_MAX:
      gpioPinWrite(_GPIO_MOT_X_ENABLE,_DEF_HIGH);
      gpioPinWrite(_GPIO_MOT_Y_ENABLE,_DEF_HIGH);
      break;
  }
}


void motorFuncStep(axis_dir axis)
{
  if (axis == AXIS_1)
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
  }
  else if (axis == AXIS_2)
  {
    LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10);
  }
}

void motorFuncDir(axis_dir axis, int dir)
{
  switch (axis)
  {
    case AXIS_1:
      gpioPinWrite(_GPIO_MOT_X_DIR,dir);
      break;
    case AXIS_2:
      gpioPinWrite(_GPIO_MOT_Y_DIR,dir);
      break;

    case AXIS_3:
      break;
    case AXIS_4:
      break;

    case AXIS_MAX:
      break;
  }
}


unsigned int S_table_len(unsigned int* table)
{
  int i;

  for(i = 0;i < TABLE_LEN;i++)
  {
    table[i] = table[i] / TABLE_PSC;

    if(table[i] >= MAX_FREQ-2) return i;
  }

  return TABLE_LEN;
}




void S_curve_gen(int steps, axis_dir axis)
{

  motor_tbl[axis].accel_index  = S_table_len(s_curve_table);
  motor_tbl[axis].decel_index  = motor_tbl[axis].accel_index;
  motor_tbl[axis].vel_index    = steps - motor_tbl[axis].accel_index - motor_tbl[axis].decel_index;
  motor_tbl[axis].totalSteps   = steps;
  motor_tbl[axis].count        = 0;
  motor_tbl[axis].state        = 0;
}


void motorMoveRel (axis_dir axis, int steps, uint32_t speed)
{
  if (speed>105)    speed=100;

  int position = steps - motor_tbl[axis].stepPosition ;
  if (position < 0)
  {
    motor_tbl[axis].dir = CCW;
    motor_tbl[axis].func_dir(axis,CCW);
  }
  else
  {
    motor_tbl[axis].dir = CW;
    motor_tbl[axis].func_dir(axis,CW);
  }

  S_curve_gen(abs(position), axis);

  motor_tbl[axis].speed = u32_Speed[speed];
  motor_tbl[axis].moveDone = false;
  motor_tbl[axis].state = 0;
  //motor_tbl[axis].p_tim_handle->pHandle->Instance->CCR1 = 10;
  //motor_tbl[axis].p_tim_handle->pHandle->Instance->ARR  = 10;



  remainingMotorsFlag |= (1 << axis);
}



void motorsRunAndWait(void)
{
  timEnableISR(_DEF_TIM1);

  while (1)
  {
    if (remainingMotorsFlag == 0)
      break;
  }
  remainingMotorsFlag = 0;

}

void MD_Stop (axis_dir axis)
{
  motor_tbl[axis].state = STOP;
}




void motorISR_Axis1(void)
{
  motor_tbl_t * pM = &motor_tbl[AXIS_1];
  TIM_TypeDef * pTim = motor_tbl[AXIS_1].p_tim_handle->pHandle->Instance;

  pM->func_step(AXIS_1);

  // pM->edge ^= 1; //XOR
  // if (pM->edge == 1)
  // {
  //When the motor moves in the reverse direction, it stops when it reaches the origin switch.
  if (pM->dir == CCW)
  {
    if (0) // is Checked origin S/W
      pM->state = STOP;
  }


  switch (pM->state)
  {
    //======
    case 0:
    {
      if(pM->accel_index != 0)
      {
        // decel_index
        if(pM->stepCount < pM->accel_index)
        {
          pTim->CCR1 = pM->speed / s_curve_table[pM->stepCount];
          pTim->ARR  = pM->speed / s_curve_table[pM->stepCount];
        }
        // max speed
        else if((pM->stepCount >= pM->accel_index)
            && (pM->stepCount <= pM->accel_index + pM->vel_index))
        {
          pTim->CCR1 = pM->speed / s_curve_table[pM->accel_index-1];
          pTim->ARR  = pM->speed / s_curve_table[pM->accel_index-1];
        }
        // deceleration
        else
        {
          pTim->CCR1 = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
          pTim->ARR  = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
          pM->count++;
        }
      }

      if(pM->stepCount == pM->totalSteps)
      {
        timDisableISR(_DEF_TIM1);
        pM->count = 0;
        pM->stepCount = 0;
        pM->moveDone = true;
        pM->state = 1;
      }

      if(pM->dir == CW)     {pM->stepPosition++;}
      else                  {pM->stepPosition--;}

      pM->stepCount++;
      break;
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
  }
  // }

  if (pM->moveDone == true) remainingMotorsFlag &= ~(1 << AXIS_1); //Off  motor timer_cal

}




void motorISR_Axis2(void)
{
  motor_tbl_t * pM = &motor_tbl[AXIS_2];
  TIM_TypeDef * pTim = motor_tbl[AXIS_2].p_tim_handle->pHandle->Instance;

  pM->func_step(AXIS_2);
  pM->edge ^= 1; //XOR
  if (pM->edge == 1)
  {
    if (pM->dir == CCW)
    {
      if (0) // is Checked origin S/W
        pM->state = STOP;
    }

    switch (pM->state)
    {
      //======
      case 0:
      {
        if(pM->accel_index != 0)
        {
          // decel_index
          if(pM->stepCount < pM->accel_index)
          {
            pTim->CCR1 = pM->speed / s_curve_table[pM->stepCount];
            pTim->ARR  = pM->speed / s_curve_table[pM->stepCount];
          }
          // max speed
          else if((pM->stepCount >= pM->accel_index)
              && (pM->stepCount <= pM->accel_index + pM->vel_index))
          {
            pTim->CCR1 = pM->speed / s_curve_table[pM->accel_index-1];
            pTim->ARR  = pM->speed / s_curve_table[pM->accel_index-1];
          }
          // deceleration
          else
          {
            pTim->CCR1 = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
            pTim->ARR  = pM->speed/s_curve_table[pM->accel_index - pM->count-1];
            pM->count++;
          }
        }

        if(pM->stepCount == pM->totalSteps)
        {
          timDisableISR(_DEF_TIM2);
          pM->count = 0;
          pM->stepCount = 0;
          pM->moveDone = true;
          pM->state = 1;
        }

        if(pM->dir == CW)     {pM->stepPosition++;}
        else                  {pM->stepPosition--;}

        pM->stepCount++;
        break;
      }
      //======
      case 1:
        timDisableISR(_DEF_TIM2);
        pM->count = 0;
        pM->stepCount = 0;
        pM->moveDone = true;
        break;
        //======
      case 2:
        timDisableISR(_DEF_TIM2);
        pM->count = 0;
        pM->stepCount = 0;
        pM->moveDone = true;
        pM->stepPosition = 0;
        break;
        //======
      default:
        break;
    }

  }
  else
  {

  }


  if (pM->moveDone == true) remainingMotorsFlag &= ~(1 << AXIS_2); //Off  motor timer_cal

}



void motorISR_Axis3(void)
{
  return;
}



void motorISR_Axis4(void)
{
  return;
}



#ifdef _USE_HW_CLI
void cliMotors(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "info") == true)
    {
      ret = true;

    }
    else if (args->isStr(0, "test") == true)
    {
      ret = true;

    }
  }
  else if (args->argc == 2)
  {
    uint8_t axis_id = (uint8_t)args->getData(1);
    if (args->isStr(0, "on") == true)
    {
      motorEnable(axis_id);
      cliPrintf( "motors Axis[%d] On \n", axis_id);
      ret = true;
    }
    else if (args->isStr(0, "off") == true)
    {
      motorDisable(axis_id);
      cliPrintf( "motors Axis[%d] Off \n", axis_id);
      ret = true;
    }

  }
  else if (args->argc == 3)
  {
    uint8_t axis_id = (uint8_t)args->getData(1);
    uint32_t step_pulse = (uint32_t)args->getData(2);
    if (args->isStr(0, "run") == true)
    {
      motorStart(axis_id);
      motorMoveRel(axis_id, step_pulse, 100);
      motorsRunAndWait();
      {
        cliPrintf("run step[%d] : %d OK\n", step_pulse);
        ret = true;
      }
    }

  }
  else if (args->argc == 4)
  {
    uint8_t axis_id = (uint8_t)args->getData(1);
    uint32_t speed = (uint32_t)args->getData(2);
    int step_pulse = (uint32_t)args->getData(3);
    if (args->isStr(0, "run") == true)
    {
      motorStart(axis_id);
      motorMoveRel(axis_id, step_pulse, speed);
      motorsRunAndWait();
      {
        cliPrintf("run step[%d] speed[%d] : OK\n", step_pulse, speed);
        ret = true;
      }
    }

  }

  if (ret == false)
  {
    cliPrintf( "motors info \n");
    cliPrintf( "motors test \n");
    cliPrintf( "motors on [axis] \n");
    cliPrintf( "motors off [axis] \n");
    cliPrintf( "motors run [axis][step] \n");
    cliPrintf( "motors run [axis][speed][step] \n");

  }


}
#endif




#endif
