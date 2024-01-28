/*
 * def_err.h
 *
 *  Created on: 2023. 06. 11.
 *      Author: gns2l
 */

#ifndef SRC_AP_DEF_ERR_H_
#define SRC_AP_DEF_ERR_H_


#define errcode int

struct ap_err_st
{
	enum err_e : uint8_t
	{
		no_error,
		mcu_unit_err,
		seq_initial_timeout,
		emg_stop,
		error_stop,

		servo_on_err,
		axis_state_err,
		axis_origin_err,
		axis_start_timeout,
		axis_move_timeout,
		axis_stop_timeout,

		cyl_timeout,
		cyl_open_timeout,
		cyl_close_timeout,
		cyl_for_timeout,
		cyl_back_timeout,
		cyl_hold_timeout,
		cyl_release_timeout,
		cyl_push_timeout,
		cyl_pull_timeout,

		vac_timeout,
		vac_deletc_on_timeout,

		cyl_interlock_State,

		sensor_phone_empty,
		sensor_vinyl_on_roll,
		sensor_hold_vinly_err,

		axis_position_err,

		return_fail,
		undefined_err
	};
  ap_err_st() = default;
  ~ap_err_st() = default;

};


#define ERR_API_LCD_COMM_TIMEOUT  101


#endif /* SRC_AP_DEF_ERR_H_ */
