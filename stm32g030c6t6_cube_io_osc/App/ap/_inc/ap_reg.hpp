/*
 * ap_reg.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_AP_REG_HPP_
#define AP__INC_AP_REG_HPP_

/*

   01. AUTO_RUNNING :
   02. EMG_STOP :
   03. AUTO_READY : Check if auto-run is possible, and if there is no problem, wait for the start signal
   04. AUTO_STOP :Stopped during auto run
   05. DETECT_AREA_SEN : Automatic operation should be stopped.
   06. MOTOR_ON : Motor is enabled normally
   07. ORG_COMPLETED : System initialization has been completed and automatic operation is possible
   08. ALARM_STATUS : There is an error to reset the system.

	 11. STATE_10,
	 12. STATE_11,
	 13. STATE_12,
	 14. STATE_13,
	 15. STATE_14,
	 16. STATE_15,
	 17. REQUEST_INITAL,
	 18. ALL_CHECK_OK,

 */


/*

 ap register 상태정보는 어디서 업데이트를 할지?

 */

#ifdef _USE_HW_RTOS
#define AP_REG_LOCK_BEGIN osMutexWait(ap_reg_mutex_id, osWaitForever)
#define AP_REG_LOCK_END   osMutexRelease(ap_reg_mutex_id)
#else
#define AP_REG_LOCK_BEGIN
#define AP_REG_LOCK_END
#endif

struct ap_reg
{
  enum state_e
  {
    AUTO_RUNNING,
    EMG_STOP,
    AUTO_READY,
    AUTO_STOP,
    DETECT_AREA_SEN,
    MOTOR_ON,
    SYSTEM_INIT_COMPLETED,
    ALARM_STATUS,

    JOB_COMPLETED,
    JOB_FAIL,
    STATE_12,
    STATE_13,
    STATE_14,
    FACTORY_INITIAL,
    REQUEST_INITIAL,
    INITIALIZING,
  };

  union AP_STATE_REG
  {

    uint16_t ap_state{};
    struct {
      unsigned auto_running : 1;
      unsigned emg_stop : 1;
      unsigned auto_ready : 1;
      unsigned auto_stop : 1;
      unsigned detect_safe_sensor : 1;
      unsigned motor_on : 1;
      unsigned system_origin_cplt : 1;
      unsigned alarm_status : 1;

      unsigned job_cplt : 1;
      unsigned job_fail : 1;
      unsigned state_12 : 1;
      unsigned state_13 : 1;
      unsigned state_14 : 1;
      unsigned factory_initial : 1;
      unsigned request_initial : 1;
      unsigned initializing : 1;
    };
  };

  enum option_e
  {
    USE_BEEP,
    USE_DRYRUN,
    NOT_USE_DOORSENSOR,
    NOT_USE_SAFETYSENSOR,
    NOT_USE_ESTOP,
    USE_RETRY,
    USE_BLOW,
    USE_IGNORE_1_ERR,

    USE_PILE_VINYL,
    OPTION_11,
    OFFSET_X1,
    OFFSET_X2,
    TEST_LONG,
    MODE_AUTO,
    NOT_USE_MOTOR,
    NOT_USE_LCD,
  };

  union AP_OPT_REG
  {
    uint16_t ap_option{};
    struct
    {
      unsigned use_beep : 1;
      unsigned use_dryrun : 1;
      unsigned not_use_doorsensor : 1;
      unsigned not_use_safesensor : 1;
      unsigned not_use_estop : 1;
      unsigned use_retry : 1;
      unsigned use_blow : 1;
      unsigned use_ignore_1_err : 1;

      unsigned use_pile_vinyl : 1;
      unsigned option_11 : 1;
      unsigned offset_x1 : 1;
      unsigned offset_x2 : 1;
      unsigned test_long : 1;
      unsigned mode_auto : 1;
      unsigned not_use_motor : 1;
      unsigned not_use_lcd : 1;
    };
  };

  union AP_ERR_REG
  {
    uint32_t ap_error{};
    struct
    {
      unsigned no_error : 1;
      unsigned no_resp_lcd : 1;
      unsigned no_resp_mot : 1;
      unsigned err_03 : 1;
      unsigned err_04 : 1;
      unsigned err_test_unit : 1;
      unsigned err_state_motor : 1;
      unsigned not_ready_motor : 1;

      unsigned limit_err_motor : 1;
      unsigned cylinder_interlock : 1;
      unsigned err_12 : 1;
      unsigned err_13 : 1;
      unsigned cylinder_timeout : 1;
      unsigned vacuum_timeout : 1;
      unsigned motor_timeout : 1;
      unsigned sequnce_timeout : 1;

      unsigned bad_vinyl : 1;
      unsigned no_phone : 1;
      unsigned no_vinyl : 1;
      unsigned vinyl_detect : 1;
      unsigned err_24 : 1;
      unsigned err_25 : 1;
      unsigned err_26 : 1;
      unsigned err_27 : 1;

      unsigned err_30 : 1;
      unsigned err_31 : 1;
      unsigned err_32 : 1;
      unsigned err_33 : 1;
      unsigned err_34 : 1;
      unsigned err_35 : 1;
      unsigned err_36 : 1;
      unsigned not_defined : 1;
    };
  };






#ifdef _USE_HW_RTOS
  osMutexId ap_reg_mutex_id;
#endif

  volatile AP_ERR_REG error_reg;
  volatile AP_OPT_REG option_reg;
  volatile AP_STATE_REG state_reg;
  uint8_t test_reg;
  uint16_t test_state;

  /*constructor*/
  ap_reg() : error_reg{}, option_reg{}, state_reg{}, test_reg{}, test_state{} {
#ifdef _USE_HW_RTOS
    osMutexDef(ap_reg_mutex_id);
    ap_reg_mutex_id = osMutexCreate(osMutex(ap_reg_mutex_id));
#endif
  }


  inline void SetStatusRegister(uint16_t data) {
    state_reg.ap_state = data;
  }

  inline void SetOptionRegister(uint16_t data) {
    option_reg.ap_option = data;
  }

  inline void ClearAlarmState() {
    error_reg.ap_error = 0;
  }

  inline uint16_t SetReg_State(ap_reg::state_e state, bool on_off = true) {
    AP_REG_LOCK_BEGIN;

    switch (state) {
      case AUTO_RUNNING:
      {
        state_reg.auto_ready = false;
        state_reg.auto_running = true;
        state_reg.auto_stop = false;
      }
      break;

      case EMG_STOP:
      {
        if (option_reg.not_use_estop)
          state_reg.emg_stop = false;
        else
          state_reg.emg_stop = on_off;
      }
      break;

      case AUTO_READY:
      {
        state_reg.auto_ready = true;
        state_reg.auto_running = false;
        state_reg.auto_stop = false;
      }
      break;

      case AUTO_STOP:
      {
        state_reg.auto_ready = false;
        state_reg.auto_running = false;
        state_reg.auto_stop = true;
      }
      break;

      case DETECT_AREA_SEN:
      {
        if (option_reg.not_use_safesensor)
          state_reg.detect_safe_sensor = false;
        else
          state_reg.detect_safe_sensor = on_off;
      }
      break;

      case MOTOR_ON:
      {
        state_reg.motor_on = on_off;
      }
      break;

      case SYSTEM_INIT_COMPLETED:
      {
        if (on_off)
        {
          state_reg.system_origin_cplt = true;
          state_reg.initializing = false;
        }

      }
      break;

      case ALARM_STATUS:
      {
        state_reg.alarm_status = on_off;

      }
      break;

      case JOB_COMPLETED:
      {
        state_reg.job_cplt = true;
        state_reg.job_fail = false;
      }
      break;
      case JOB_FAIL:
      {
        state_reg.job_cplt = false;
        state_reg.job_fail = true;
      }
      break;
      case STATE_12:
      {

      }
      break;
      case STATE_13:
      {

      }
      break;
      case STATE_14:
      {

      }
      break;
      case FACTORY_INITIAL:
      {

      }
      break;
      case REQUEST_INITIAL:
      {
      }
      break;
      case INITIALIZING:
      {
        state_reg.initializing = on_off;
      }
      break;
      default:
        break;
    }
    // end of switch (state)


    AP_REG_LOCK_END;
    return state_reg.ap_state;
  }


};




#endif /* AP__INC_AP_REG_HPP_ */
