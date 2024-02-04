/*
 * def_obj.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP_DEF_OBJ_HPP_
#define AP_DEF_OBJ_HPP_



#define AP_DEF_OBJ_BASE_MOTOR                   0x0010
#define AP_DEF_OBJ_BASE_CYLINDER                0x0100
#define AP_DEF_OBJ_BASE_VACUUM                  0x0120
#define AP_DEF_OBJ_BASE_BUTTON                  0x0140
#define AP_DEF_OBJ_BASE_LED		                  0x0160

#define AP_DEF_OBJ_BASE_IO                      0x0140
#define AP_DEF_START_IN_ADDR                    1000
#define AP_DEF_START_OUT_ADDR                   3000

#if 1
#define AP_DEF_OBJ_MOTOR_ID_BASE                1
#define M_SetMotorId(instance_no)               AP_DEF_OBJ_MOTOR_ID_BASE + instance_no
#define M_GetMotorInstanceId(node_id)           node_id - AP_DEF_OBJ_MOTOR_ID_BASE
#define M_GetMotorObjId(obj)                    AP_DEF_OBJ_BASE_MOTOR|obj

#define M_GetCylinderObjId(obj)              		AP_DEF_OBJ_BASE_CYLINDER|obj

#define GetVacObjId(obj)                     		AP_DEF_OBJ_BASE_VACUUM|obj
#define GetButtonObjId(obj)                     AP_DEF_OBJ_BASE_BUTTON|obj
#define GetLedLampObjId(obj)                    AP_DEF_OBJ_BASE_LED|obj

// #define MOTOR_ID_A                              0b0001
// #define MOTOR_ID_B                              0b0010
// #define MOTOR_ID_C                              0b0100
// #define MOTOR_ID_ALL                            0b1000

#endif

namespace AP_OBJ
{
	enum MOTOR
	{
		MOTOR_A, // left step motor
		MOTOR_B, // right step motor
		MOTOR_C, // center step motor
		MOTOR_MAX,

	};
	
  enum LED
  {
    LED_STATUS,
    LED_OP_G_READY,
    LED_OP_Y_BUSY,
    LED_OP_R_ERROR,
    LED_MAX,
  };

	  enum BTN
  {
    BTN_START,
    BTN_RESET,
    BTN_STOP,
    BTN_MAX,
  };

	enum CYL
	{
		CYL_TOOL_OPENCLOSE,

		CYL_MAX,

	};

	enum VAC
	{
		VAC_A, // left vacuum
		VAC_B, // right vacuum

		VAC_MAX,
	};

}




#endif /* AP_DEF_OBJ_HPP_ */
