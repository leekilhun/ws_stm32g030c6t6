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
#if 0

struct AP_ERR_REG
{
  enum err_e : size_t
  {
    NO_ERR_STATE,
    NO_RESP_LCD,
    NO_RESP_MOT,
    ERR_03,
    ERR_04,
    ERR_TEST_UNIT,
    ERR_STATE_MOTOR,
    NOT_READY_MOTOR,

    LIMIT_ERR_MOTOR,
    CYLINDER_INTERLOCK,
    ERR_12,
    ERR_13,
    CYLINDER_TIMEOUT,
    VACUUM_TIMEOUT,
    MOTOR_TIMEOUT,
    SEQUNCE_TIMEOUT,

    BAD_VINYL,
    NO_PHONE,
    NO_VINYL,
    VINYL_DETECT,
    ERR_24,
    ERR_25,
    ERR_26,
    ERR_27,
    ERR_30,
    ERR_31,
    ERR_32,
    ERR_33,
    ERR_34,
    ERR_35,
    ERR_36,
    ERR_37,
    ERR_MAX
  };

  std::bitset<err_e::ERR_MAX> ap_error;

  bool no_error() const { return ap_error.test(NO_ERR_STATE); }
  bool no_resp_lcd() const { return ap_error.test(NO_RESP_LCD); }
  bool no_resp_mot() const { return ap_error.test(NO_RESP_MOT); }
  bool err_03() const { return ap_error.test(ERR_03); }
  bool err_04() const { return ap_error.test(ERR_04); }
  bool err_test_unit() const { return ap_error.test(ERR_TEST_UNIT); }
  bool err_state_motor() const { return ap_error.test(ERR_STATE_MOTOR); }
  bool not_ready_motor() const { return ap_error.test(NOT_READY_MOTOR); }
  bool limit_err_motor() const { return ap_error.test(LIMIT_ERR_MOTOR); }
  bool cylinder_interlock() const { return ap_error.test(CYLINDER_INTERLOCK); }
  bool err_12() const { return ap_error.test(ERR_12); }
  bool err_13() const { return ap_error.test(ERR_13); }
  bool cylinder_timeout() const { return ap_error.test(CYLINDER_TIMEOUT); }
  bool vacuum_timeout() const { return ap_error.test(VACUUM_TIMEOUT); }
  bool motor_timeout() const { return ap_error.test(MOTOR_TIMEOUT); }
  bool sequnce_timeout() const { return ap_error.test(SEQUNCE_TIMEOUT); }
  bool bad_vinyl() const { return ap_error.test(BAD_VINYL); }
  bool no_phone() const { return ap_error.test(NO_PHONE); }
  bool no_vinyl() const { return ap_error.test(NO_VINYL); }
  bool vinyl_detect() const { return ap_error.test(VINYL_DETECT); }
  bool err_24() const { return ap_error.test(ERR_24); }
  bool err_25() const { return ap_error.test(ERR_25); }
  bool err_26() const { return ap_error.test(ERR_26); }
  bool err_27() const { return ap_error.test(ERR_27); }
  bool err_30() const { return ap_error.test(ERR_30); }
  bool err_31() const { return ap_error.test(ERR_31); }
  bool err_32() const { return ap_error.test(ERR_32); }
  bool err_33() const { return ap_error.test(ERR_33); }
  bool err_34() const { return ap_error.test(ERR_34); }
  bool err_35() const { return ap_error.test(ERR_35); }
  bool err_36() const { return ap_error.test(ERR_36); }
  bool err_37() const { return ap_error.test(ERR_37); }

  // void set_no_error(bool value) { ap_error.set(0, value); }*
};

struct AP_OPT_REG
{
  enum option_e : size_t
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
    option_max
  };

  std::bitset<option_e::option_max> ap_option;
  bool use_beep() const { return ap_option.test(option_e::USE_BEEP); }
  bool use_dryrun() const { return ap_option.test(option_e::USE_DRYRUN); }
  bool not_use_doorsensor() const { return ap_option.test(option_e::NOT_USE_DOORSENSOR); }
  bool not_use_safetysensor() const { return ap_option.test(option_e::NOT_USE_SAFETYSENSOR); }
  bool not_use_estop() const { return ap_option.test(option_e::NOT_USE_ESTOP); }
  bool use_retry() const { return ap_option.test(option_e::USE_RETRY); }
  bool use_blow() const { return ap_option.test(option_e::USE_BLOW); }
  bool use_ignore_1_err() const { return ap_option.test(option_e::USE_IGNORE_1_ERR); }

  bool use_pile_vinyl() const { return ap_option.test(option_e::USE_PILE_VINYL); }
  bool use_option_11() const { return ap_option.test(option_e::OPTION_11); }
  bool use_offset_x1() const { return ap_option.test(option_e::OFFSET_X1); }
  bool use_offset_x2() const { return ap_option.test(option_e::OFFSET_X2); }
  bool use_test_long() const { return ap_option.test(option_e::TEST_LONG); }
  bool use_mode_auto() const { return ap_option.test(option_e::MODE_AUTO); }
  bool not_use_motor() const { return ap_option.test(option_e::NOT_USE_MOTOR); }
  bool not_use_lcd() const { return ap_option.test(option_e::NOT_USE_LCD); }

  void set_use_beep(bool value) { ap_option.set(option_e::USE_BEEP, value); }
  void set_use_dryrun(bool value) { ap_option.set(option_e::USE_DRYRUN, value); }
  void set_not_use_doorsensor(bool value) { ap_option.set(option_e::NOT_USE_DOORSENSOR, value); }
  void set_not_use_safetysensor(bool value) { ap_option.set(option_e::NOT_USE_SAFETYSENSOR, value); }
  void set_not_use_estop(bool value) { ap_option.set(option_e::NOT_USE_ESTOP, value); }
  void set_use_retry(bool value) { ap_option.set(option_e::USE_RETRY, value); }
  void set_use_blow(bool value) { ap_option.set(option_e::USE_BLOW, value); }
  void set_use_ignore_1_err(bool value) { ap_option.set(option_e::USE_IGNORE_1_ERR, value); }

  void set_use_pile_vinyl(bool value) { ap_option.set(option_e::USE_PILE_VINYL, value); }
  void set_use_option_11(bool value) { ap_option.set(option_e::OPTION_11, value); }
  void set_use_offset_x1(bool value) { ap_option.set(option_e::OFFSET_X1, value); }
  void set_use_offset_x2(bool value) { ap_option.set(option_e::OFFSET_X2, value); }
  void set_use_test_long(bool value) { ap_option.set(option_e::TEST_LONG, value); }
  void set_use_mode_auto(bool value) { ap_option.set(option_e::MODE_AUTO, value); }
  void set_not_use_motor(bool value) { ap_option.set(option_e::NOT_USE_MOTOR, value); }
  void set_not_use_lcd(bool value) { ap_option.set(option_e::NOT_USE_LCD, value); }
};

struct AP_STATE_REG
{
  enum state_e : size_t
  {
    AUTO_RUNNING,
    EMG_STOP,
    AUTO_READY,
    AUTO_STOP,
    DETECT_AREA_SEN,
    MOTOR_ON,
    INIT_COMPLETED,
    ALARM_STATUS,

    JOB_COMPLETED,
    JOB_FAIL,
    STATE_12,
    STATE_13,
    STATE_14,
    FACTORY_INITIAL,
    REQUEST_INITIAL,
    INITIALIZING,
    STATE_MAX
  };
  std::bitset<state_e::STATE_MAX> ap_state;

  bool auto_running() const { return ap_state.test(state_e::AUTO_RUNNING); }
  bool emg_stop() const { return ap_state.test(state_e::EMG_STOP); }
  bool auto_ready() const { return ap_state.test(state_e::AUTO_READY); }
  bool auto_stop() const { return ap_state.test(state_e::AUTO_STOP); }
  bool detect_safe_sensor() const { return ap_state.test(state_e::DETECT_AREA_SEN); }
  bool motor_on() const { return ap_state.test(state_e::MOTOR_ON); }
  bool system_origin_cplt() const { return ap_state.test(state_e::INIT_COMPLETED); }
  bool alarm_status() const { return ap_state.test(state_e::ALARM_STATUS); }

  bool job_cplt() const { return ap_state.test(state_e::JOB_COMPLETED); }
  bool job_fail() const { return ap_state.test(state_e::JOB_FAIL); }
  bool state_12() const { return ap_state.test(state_e::STATE_12); }
  bool state_13() const { return ap_state.test(state_e::STATE_13); }
  bool state_14() const { return ap_state.test(state_e::STATE_14); }
  bool factory_initial() const { return ap_state.test(state_e::FACTORY_INITIAL); }
  bool request_initial() const { return ap_state.test(state_e::REQUEST_INITIAL); }
  bool all_check_ok() const { return ap_state.test(state_e::INITIALIZING); }

  void set_auto_running(bool value) { ap_state.set(state_e::AUTO_RUNNING, value); }
  void set_emg_stop(bool value) { ap_state.set(state_e::EMG_STOP, value); }
  void set_auto_ready(bool value) { ap_state.set(state_e::AUTO_READY, value); }
  void set_auto_stop(bool value) { ap_state.set(state_e::AUTO_STOP, value); }
  void set_detect_safe_sensor(bool value) { ap_state.set(state_e::DETECT_AREA_SEN, value); }
  void set_motor_on(bool value) { ap_state.set(state_e::MOTOR_ON, value); }
  void set_system_origin_cplt(bool value) { ap_state.set(state_e::INIT_COMPLETED, value); }
  void set_alarm_status(bool value) { ap_state.set(state_e::ALARM_STATUS, value); }

  void set_job_cplt(bool value) { ap_state.set(state_e::JOB_COMPLETED, value); }
  void set_job_fail(bool value) { ap_state.set(state_e::JOB_FAIL, value); }
  void set_state_12(bool value) { ap_state.set(state_e::STATE_12, value); }
  void set_state_13(bool value) { ap_state.set(state_e::STATE_13, value); }
  void set_state_14(bool value) { ap_state.set(state_e::STATE_14, value); }
  void set_factory_initial(bool value) { ap_state.set(state_e::FACTORY_INITIAL, value); }
  void set_request_initial(bool value) { ap_state.set(state_e::REQUEST_INITIAL, value); }
  void set_all_check_ok(bool value) { ap_state.set(state_e::INITIALIZING, value); }
};


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
