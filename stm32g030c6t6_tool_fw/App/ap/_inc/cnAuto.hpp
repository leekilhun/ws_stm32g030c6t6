/*
 * cnAuto.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#pragma once
#ifndef AP__INC_CNAUTO_HPP_
#define AP__INC_CNAUTO_HPP_

#include "ap_def.hpp"

#define AUTO_ALARM(head, msg) AlarmAuto(head, (__FILE__), __FUNCTION__, __LINE__, msg)


struct cnAuto
{
  /****************************************************
   *  data
   ****************************************************/
public:
  enum virtual_key_e : uint8_t
  {
    vkey_start,
    vkey_stop,
    vkey_reset,
    vkey_max,
  };

  struct cfg_t
  {
    ap_reg *ptr_apReg{};
    enOp *ptr_op{};
    ap_io *ptr_io{};

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

  cfg_t m_cfg{};

  bool m_checkReady{};
  prc_step_t m_step{};
  bool m_autoreadyFlag{};
  ap_err_st::err_e m_apErr{};
  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnAuto() = default;
  ~cnAuto() = default;

  /****************************************************
   *  func
   ****************************************************/
  inline errno_t Init(cnAuto::cfg_t &cfg)
  {
    m_cfg = cfg;
    LOG_PRINT("[OK] Init Success!");
    return ERROR_SUCCESS;
  }

  inline enOp::mode_e GetOPMode()
  {
    return m_cfg.ptr_op->GetMode();
  }

  inline enOp::status_e GetOPStatus()
  {
    return m_cfg.ptr_op->GetStatus();
  }
  inline void SetOPMode(enOp::mode_e mode)
  {
    m_cfg.ptr_op->SetMode(mode);
  }

  inline void SetOPStatus(enOp::status_e status)
  {
    //LOG_PRINT("SetOPStatus - status[%d] ", status);
    m_cfg.ptr_op->SetStatus(status);
  }

  inline void StartSw()
  {
    UiStartSw();
  }

  inline void StopSw()
  {
    SetOPMode(enOp::mode_e::STOP);
    if (m_cfg.ptr_apReg->state_reg.system_origin_cplt) // GetOPStatus() !=(enOp::status_e::INIT )
    {
      SetOPStatus(enOp::status_e::STEP_STOP);
    }
    else
    {
      SetOPStatus(enOp::status_e::INIT);
    }
    m_cfg.ptr_apReg->state_reg.initializing = false;
    m_checkReady = false;
  }

  inline void ResetSw()
  {
    //logPrintf("ResetSw\n");
    StopSw();
    m_cfg.ptr_apReg->ClearAlarmState();
  }

  inline void PauseStop()
  {
    // m_IsDetectedPauseSensor = true;
  }

  inline bool SetEStop(bool state)
  {
    m_cfg.ptr_apReg->SetReg_State(ap_reg::EMG_STOP, state);
    return state;
  }

  inline bool IsDetectAreaSensor() const
  {
    return m_cfg.ptr_apReg->state_reg.detect_safe_sensor;
  }

  inline bool IsRequestSystemInit() const
  {
    return m_cfg.ptr_apReg->state_reg.request_initial;
  }

#if 0

  inline bool IsModeAuto() {
    if (m_cfg.ptr_apReg->state_reg.factory_initial)
      return false;
    return m_cfg.p_ApIo->m_in.in_mode_key_auto
      || m_cfg.p_apReg->option_reg.mode_auto;
  }

  inline void ResetFlagAutoReady() {
    m_autoreadyFlag = false;
    SetOPStatus(enOp::status_e::RUN_READY);
  }
#endif

  inline void UiStartSw()
  {
    m_autoreadyFlag = true;
    m_checkReady = true;
  }

  inline void ThreadJob()
  {
    /*
      check register updates and checked items by op status status.
    */
    doRunStep();

    if (m_checkReady)
      AutoReady();
  }

  inline void doRunStep()
  {
    using state_t = enOp::status_e;
    // using mode_t = enOp::mode_e;
    using reg_t = ap_reg::state_e;
    // enOp* op = m_cfg.ptr_op;
    ap_reg *reg = m_cfg.ptr_apReg;

    enum : uint8_t
    {
      STEP_INIT,
      STEP_TODO,
      STEP_STATUS_INIT,
      STEP_STATUS_STOP,
      STEP_STATUS_RUN_READY,
      STEP_STATUS_RUN,
      STEP_STATUS_ERR_STOP,
      STEP_TIMEOUT,
      STEP_END
    };

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
      if (reg->state_reg.system_origin_cplt == false && GetOPStatus() != enOp::INIT)
      {
        SetOPStatus(enOp::INIT);
      }

      switch (GetOPStatus())
      {
      case state_t::INIT:
        m_step.SetStep(STEP_STATUS_INIT);
        break;

      case state_t::STEP_STOP:
        m_step.SetStep(STEP_STATUS_STOP);
        break;

      case state_t::RUN_READY:
        m_step.SetStep(STEP_STATUS_RUN_READY);
        break;

      case state_t::RUN:
        m_step.SetStep(STEP_STATUS_RUN);
        break;

      case state_t::ERR_STOP:
        m_step.SetStep(STEP_STATUS_ERR_STOP);
        break;
      default:
        break;
      }
      // end of switch (GetOPStatus())
    }
    break;

    case STEP_TIMEOUT:
    {
      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_STATUS_INIT:
    {
      if (reg->state_reg.system_origin_cplt && GetOPStatus() != enOp::STEP_STOP)
        SetOPStatus(enOp::STEP_STOP);

      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_STATUS_STOP:
    {
      reg->SetReg_State(reg_t::AUTO_STOP, true);

      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_STATUS_RUN_READY:
    {
      reg->SetReg_State(reg_t::AUTO_READY, true);

      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_STATUS_RUN:
    {
      reg->SetReg_State(reg_t::AUTO_RUNNING, true);

      m_step.SetStep(STEP_TODO);
    }
    break;

    case STEP_STATUS_ERR_STOP:
    {
      m_step.SetStep(STEP_TODO);
    }
    break;

    default:
      break;
    } // switch (m_step.GetStep())
  }

  // Check the automatic operating conditions of the module
  inline bool IsRunCondition()
  {
    ap_reg *ap_reg = m_cfg.ptr_apReg;
    using err_t = ap_err_st::err_e;
    if (ap_reg->state_reg.emg_stop)
    {
      m_apErr = err_t::emg_stop;
      logPrintf("cnAuto checkStartRunCondition  ap_reg->state_reg.emg_stop  \n");
      return false;
    }
    /*
    else
    {
      m_apErr = err_t::no_error;
    }

    return true;
    */

    if (ap_reg->state_reg.alarm_status)
    {
      if (ap_reg->state_reg.auto_stop)
      {
        logPrintf("cnAuto checkStartRunCondition cnAuto::state_e::error_stop \n");
        m_apErr = err_t::error_stop; // cnAuto::state_e::error_stop;
        return false;
      }
      else if (ap_reg->option_reg.not_use_motor ? false : (ap_reg->state_reg.motor_on == false))
      {
        logPrintf("cnAuto checkStartRunCondition cnAuto::state_e::servo_on_err \n");
        m_apErr = err_t::servo_on_err;
        return false;
      }
      else if (ap_reg->state_reg.system_origin_cplt == false)
      {
        logPrintf("cnAuto checkStartRunCondition cnAuto::state_e::axis_origin_err \n");
        m_apErr = err_t::axis_origin_err;
        return false;
      }
      else
      {
        logPrintf("cnAuto checkStartRunCondition cnAuto::state_e::mcu_unit_err \n");
        m_apErr = err_t::mcu_unit_err;
        return false;
      }
    }
    else
    {
    }

    return true;
  }

  inline bool IsModeAuto()
  {
      return false;
  }

  // condition for auto-run
  inline int AutoReady()
  {
    using state_t = enOp::status_e;
    using mode_t = enOp::mode_e;
    enOp *op = m_cfg.ptr_op;
    ap_reg *reg = m_cfg.ptr_apReg;
    if (IsRunCondition())
    {
      if (m_autoreadyFlag)
      {
        m_autoreadyFlag = false;
        if (op->GetStatus() == state_t::RUN_READY)
        {
          if (reg->option_reg.use_dryrun)
          {
            SetOPMode(mode_t::DRY_RUN);
          }
          else
          {
            SetOPMode(mode_t::AUTORUN);
          }
          SetOPStatus(state_t::RUN);
        }
        else if (op->GetStatus() != state_t::RUN)
        {
          SetOPMode(mode_t::READY);
          SetOPStatus(state_t::RUN_READY);
        }
      }
    }
    else
    {
      AlarmAuto(m_apErr);
    }

    return 0;
  }

  inline void AlarmAuto(ap_err_st::err_e err)
  {
    using err_t = ap_err_st::err_e;

    // ap_reg* reg = m_cfg.ptr_apReg;
    switch (err)
    {
    case err_t::no_error:
      break;
    case err_t::mcu_unit_err:
      m_cfg.ptr_apReg->error_reg.err_test_unit = true;
      break;
    case err_t::seq_initial_timeout:
      __attribute__((fallthrough));
    case err_t::emg_stop:
      __attribute__((fallthrough));
    case err_t::error_stop:
      break;

    case err_t::servo_on_err:
      __attribute__((fallthrough));
    case err_t::axis_state_err:
      __attribute__((fallthrough));
    case err_t::axis_origin_err:
      m_cfg.ptr_apReg->error_reg.not_ready_motor = true;
      break;

    case err_t::axis_start_timeout:
      __attribute__((fallthrough));
    case err_t::axis_move_timeout:
      __attribute__((fallthrough));
    case err_t::axis_stop_timeout:
      m_cfg.ptr_apReg->error_reg.motor_timeout = true;
      break;

    case err_t::cyl_open_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_close_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_for_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_back_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_hold_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_release_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_push_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_pull_timeout:
      __attribute__((fallthrough));
    case err_t::cyl_timeout:
      m_cfg.ptr_apReg->error_reg.cylinder_timeout = true;
      break;

    case err_t::vac_deletc_on_timeout:
      __attribute__((fallthrough));
    case err_t::vac_timeout:
      m_cfg.ptr_apReg->error_reg.vacuum_timeout = true;
      break;

    case err_t::cyl_interlock_State:
      m_cfg.ptr_apReg->error_reg.cylinder_interlock = true;
      break;

    default:
      m_cfg.ptr_apReg->error_reg.not_defined = true;
      break;
    }
    // switch (err)

    SetOPStatus(enOp::status_e::ERR_STOP);

    //LOG_PRINT("AlarmAuto - err[%d] ", err);
  }

  inline void AlarmAuto(log_dat::head_t *ptr_head,
                        const char *file,
                        const char *func,
                        const int line,
                        const char *msg)
  {
    constexpr int ARG_TBL_CNT_MAX = 10;
    constexpr int FILE_STR_MAX = 40;
    using err_t = ap_err_st::err_e;
    err_t err = static_cast<err_t>(ptr_head->error_no);

    char tmp_str[FILE_STR_MAX] = {
        0,
    };

    size_t source_length = strlen(file);
    const char *copy_start = nullptr;
    if (source_length > (FILE_STR_MAX - 1))
      copy_start = file + (source_length - (FILE_STR_MAX - 1));
    else
      copy_start = file;

    strncpy(tmp_str, copy_start, FILE_STR_MAX);
    char *arg_tbl[ARG_TBL_CNT_MAX] = {};
    uint8_t arg_cnt = trans::SplitArgs(tmp_str, arg_tbl, "/\\", ARG_TBL_CNT_MAX);

    // m_cfg.ptr_apLog->apLogWrite(p_head, "file[%s],func[%s-%d],msg[%d-%d-%s]", arg_tbl[arg_cnt - 1], func, line, p_head->error_no, p_head->obj_id, msg);
    LOG_PRINT("file[%s],func[%s-%d],msg[%d-%d-%s]", arg_tbl[arg_cnt - 1], func, line, ptr_head->error_no, ptr_head->obj_id, msg);

    AlarmAuto(err);
  }
};

#endif /* AP__INC_CNAUTO_HPP_ */
