/*
 * motors.h
 *
 *  Created on: Jul 10, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_EXHW_MOTORS_H_
#define COMMON_INC_EXHW_MOTORS_H_



#ifdef __cplusplus
extern "C" {
#endif


#include "exhw_def.h"


#ifdef _USE_EXHW_MOTORS

#define TABLE_LEN   200
#define TABLE_PSC     1
#define MAX_FREQ    800
#define START_FREQ  100



typedef struct
{
    axis_dir          axis;
    tim_tbl_t*        p_tim_handle;
    uint32_t          channel;

    void (*func_dir)(axis_dir axis, int dir);
    void (*func_step)(axis_dir axis);

    long                stepPosition; // current position of stepper (total of all movements)

    volatile int8_t     dir;          // current direction of movement, used to keep
    volatile uint32_t   totalSteps ;  // number of  steps requested for current movement
    volatile uint32_t   stepCount;    // number of steps completed in current movement

    volatile float      speedScale;   // used to slow down this motor to make coordinated movement with other motors

    volatile uint8_t    state;
    volatile uint8_t    is_busy;
    volatile uint8_t    edge;


    volatile uint32_t   count;
    volatile uint32_t   speed;

    volatile int   accel_index;
    volatile int   vel_index;
    volatile int   decel_index;

    volatile bool       moveDone ;

    bool                is_init;
    bool                is_start;

} motor_tbl_t;


  bool motorsInit(void);
  bool motorStart(axis_dir axis);

  void motorEnable(axis_dir axis);
  void motorDisable(axis_dir axis);

  void motorMoveRel( axis_dir axis, int steps, uint32_t speed);
  void motorsRunAndWait(void);

#endif


#ifdef __cplusplus
}
#endif




#endif /* COMMON_INC_EXHW_MOTORS_H_ */
