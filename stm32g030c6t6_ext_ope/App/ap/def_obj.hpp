/*
 * def_obj.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP_DEF_OBJ_HPP_
#define AP_DEF_OBJ_HPP_



#define AP_DEF_OBJ_BASE_MOTOR                    0x00000010
#define AP_DEF_OBJ_BASE_STP_MOTOR                0x00000050
#define AP_DEF_OBJ_BASE_CYLINDER                 0x00000100
#define AP_DEF_OBJ_BASE_VACUUM                   0x00000150

#define AP_DEF_OBJ_BASE_IO                       0x00000140

#define AP_DEF_START_IN_ADDR                     1000
#define AP_DEF_START_OUT_ADDR                    3000

#define AP_DEF_OBJ_MOTOR_ID_BASE                1
#define M_SetMotorId(instance_no)               AP_DEF_OBJ_MOTOR_ID_BASE + instance_no
#define M_GetMotorInstanceId(node_id)           node_id - AP_DEF_OBJ_MOTOR_ID_BASE
#define M_GetMotorObjIdx(obj)                    AP_DEF_OBJ_BASE_MOTOR|obj
#define M_GetStpMotorObjIdx(obj)                 AP_DEF_OBJ_BASE_STP_MOTOR|obj
#define M_GetCylinderObjIdx(obj)                 AP_DEF_OBJ_BASE_CYLINDER|obj
#define M_GetVacuumObjIdx(obj)                   AP_DEF_OBJ_BASE_VACUUM|obj


namespace AP_OBJ
{
	// enum MOTOR
	// {
	// 	MOTOR_A, // left step motor
	// 	MOTOR_B, // right step motor
	// 	MOTOR_C, // center step motor
	// 	MOTOR_MAX,

	// };
	
  enum LED
  {
    LED_OP_G_START,
    LED_OP_R_STOP,
    LED_OP_Y_RESET,
    LED_TOWER_GREEN,
    LED_TOWER_YELLOW,
    LED_TOWER_RED,
    LED_MAX,
  };
	  enum BTN
  {
    BTN_OP_START,
    BTN_OP_STOP,
    BTN_OP_RESET,
    BTN_OP_ESTOP,
    BTN_MAX,
  };

	enum CYL
	{
		// CYL_TOOL_OPENCLOSE,

		CYL_MAX,

	};

	enum VAC
	{
		// VAC_A, // left vacuum
		// VAC_B, // right vacuum

		VAC_MAX,
	};

}




#endif /* AP_DEF_OBJ_HPP_ */
