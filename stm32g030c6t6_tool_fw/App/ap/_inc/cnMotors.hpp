/*
 * cnMotors.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 * 
 *  edit : 24.01.31  : id  data -> idx
 */

#pragma once
#ifndef AP__INC_CNMOTORS_HPP_
#define AP__INC_CNMOTORS_HPP_

#include "ap_def.hpp"

// constexpr uint8_t def_motor_id_1   = 1;
// constexpr uint8_t def_motor_id_2   = 2;
// constexpr uint8_t def_motor_id_3   = 4;
// constexpr uint8_t def_motor_id_all = 8;

// #define MoTID_To_idx(id) (id == def_motor_id_1 ? AP_OBJ::MOTOR_A : (id == def_motor_id_2 ? AP_OBJ::MOTOR_B : (id == def_motor_id_3 ? AP_OBJ::MOTOR_C : (id == def_motor_id_all ? AP_OBJ::MOTOR_MAX : -1))))

class cnMotors
{
  /****************************************************
   *  data
   ****************************************************/
public:
  struct cfg_t
  {
    // axis_dat* p_apAxisDat;
    ap_reg      *ptr_apReg{};
    enStepMotor *ptr_motor{};
    ap_io       *ptr_io{};

    cfg_t()  = default;
    ~cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t &rhs) = default;
    // copy assignment operator
    cfg_t &operator=(const cfg_t &rhs) = default;
    // move constructor
    cfg_t(cfg_t &&rhs) = default;
    // move assignment operator
    cfg_t &operator=(cfg_t &&rhs) = default;

  } m_cfg{};

public:
  prc_step_t                          m_step{};
  /****************************************************
   *  Constructor
   ****************************************************/
public:
  cnMotors() = default;

  ~cnMotors() = default;
  /****************************************************
   *  func
   ****************************************************/
public:
  inline int Init(cnMotors::cfg_t &cfg)
  {
    m_cfg = cfg;
    LOG_PRINT("Init Success!");
    return ERROR_SUCCESS;
  }

  /**
   * @brief
   *
   * @param id_data
   * B0000'0001  motor 1   MOTOR_A
   * B0000'0010  motor 2   MOTOR_B
   * B0000'0100  motor 3   MOTOR_C
   * B0000'1000  motor all MOTOR_MAX
   *
   * @param is_on
   * @return errno_t
   */
  inline errno_t MotorOnOff(AP_OBJ::MOTOR idx, bool is_on = true)
  {

    if (idx == AP_OBJ::MOTOR_MAX)
    {
      is_on ? m_cfg.ptr_motor[AP_OBJ::MOTOR_A].Enable() : m_cfg.ptr_motor[AP_OBJ::MOTOR_A].Disable();
      is_on ? m_cfg.ptr_motor[AP_OBJ::MOTOR_B].Enable() : m_cfg.ptr_motor[AP_OBJ::MOTOR_B].Disable();
      is_on ? m_cfg.ptr_motor[AP_OBJ::MOTOR_C].Enable() : m_cfg.ptr_motor[AP_OBJ::MOTOR_C].Disable();
      // LOG_PRINT("motor idx [%d]", motor_idx);
    }
    else if (idx < AP_OBJ::MOTOR_MAX)
    {
      is_on ? m_cfg.ptr_motor[idx].Enable() : m_cfg.ptr_motor[idx].Disable();
    }
    else
      return ERROR_FAIL;

    return ERROR_SUCCESS;
  }

  inline errno_t Move(AP_OBJ::MOTOR idx, int cmd_cnt)
  {

    if (idx == AP_OBJ::MOTOR_MAX)
    {
      m_cfg.ptr_motor[AP_OBJ::MOTOR_A].Run(cmd_cnt, 10);
      m_cfg.ptr_motor[AP_OBJ::MOTOR_B].Run(cmd_cnt, 10);
      m_cfg.ptr_motor[AP_OBJ::MOTOR_C].Run(cmd_cnt, 10);
    }
    else if (idx < AP_OBJ::MOTOR_MAX)
    {
      return m_cfg.ptr_motor[idx].Run(cmd_cnt, 10);
    }
    else
      return ERROR_FAIL;

    return ERROR_SUCCESS;
  }

  inline errno_t DoOrigin(AP_OBJ::MOTOR idx)
  {

    if (idx == AP_OBJ::MOTOR_MAX)
    {
      m_cfg.ptr_motor[AP_OBJ::MOTOR_A].SetZeroPosition();
      m_cfg.ptr_motor[AP_OBJ::MOTOR_B].SetZeroPosition();
      m_cfg.ptr_motor[AP_OBJ::MOTOR_C].SetZeroPosition();
    }
    else if (idx < AP_OBJ::MOTOR_MAX)
    {
      return m_cfg.ptr_motor[idx].SetZeroPosition();
    }
    else
      return ERROR_FAIL;

    return ERROR_SUCCESS;
  }

inline bool IsOriginCplt(AP_OBJ::MOTOR idx = AP_OBJ::MOTOR_MAX)
  {
    if (idx == AP_OBJ::MOTOR_MAX)
    {
      bool ret  = true;
      ret       = m_cfg.ptr_motor[AP_OBJ::MOTOR_A].IsOriginCplt();
      ret      &= m_cfg.ptr_motor[AP_OBJ::MOTOR_B].IsOriginCplt();
      ret      &= m_cfg.ptr_motor[AP_OBJ::MOTOR_C].IsOriginCplt();
      return ret;
    }
    return m_cfg.ptr_motor[idx].IsOriginCplt();
  }

  inline bool IsMotorOn(AP_OBJ::MOTOR idx = AP_OBJ::MOTOR_MAX)
  {
    if (idx == AP_OBJ::MOTOR_MAX)
    {
      bool ret  = true;
      ret       = m_cfg.ptr_motor[AP_OBJ::MOTOR_A].m_status.motor_enabled;
      ret      &= m_cfg.ptr_motor[AP_OBJ::MOTOR_B].m_status.motor_enabled;
      ret      &= m_cfg.ptr_motor[AP_OBJ::MOTOR_C].m_status.motor_enabled;
      return ret;
    }
    return m_cfg.ptr_motor[idx].m_status.motor_enabled;
  }

  // step machine을 통해 nonblock으로 처리된다.
  inline void ThreadJob()
  {
    doRunStep();
  }

  inline void doRunStep()
  {
    enum
    {
      STEP_INIT,
      STEP_TODO,
      STEP_TIMEOUT,
      STEP_RESET_COMM_ALARM,
      STEP_STATE_UPDATE,
      STEP_STATE_UPDATE_START,
      STEP_STATE_UPDATE_WAIT,
      STEP_STATE_UPDATE_END,
      STEP_STATE_ALARM_RESET,
      STEP_STATE_ALARM_RESET_START,
      STEP_STATE_ALARM_RESET_WAIT,
      STEP_STATE_ALARM_RESET_END,
    };
    // constexpr uint8_t step_retry_max = 3;
    // constexpr uint8_t comm_timeout_max = 200;

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
    }
    break;
    /*######################################################
      timeout
    ######################################################*/
    case STEP_TIMEOUT:
    {
    }
    break;
    /*######################################################
      STEP_STATE_UPDATE
    ######################################################*/
    case STEP_STATE_UPDATE:
    {
      m_step.wait_resp = false;
      m_step.wait_step = 0;
      m_step.retry_cnt = 0;
      m_step.SetStep(STEP_STATE_UPDATE_START);
    }
    break;

    case STEP_STATE_UPDATE_START:
    {
    }
    break;

    case STEP_STATE_UPDATE_WAIT:
    {
      if (m_step.LessThan(50))
        break;
    }
    break;

    case STEP_STATE_UPDATE_END:
    {

      m_step.SetStep(STEP_TODO);
      break;
    }
    break;

    default:
      break;
    }
    // end of switch
  }

#if 0

    inline bool Recovery() {
      m_commStatus.comm_err = 0;

      m_cfg.p_motor[AP_OBJ::MOTOR_JIG].Recovery();
      m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].Recovery();
      m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].Recovery();

      return m_cfg.p_comm->Recovery();
    }

    inline bool IsConnected() {
      constexpr uint8_t rimit_err_cnt = 10;
      if(m_cfg.p_comm->IsConnected())
        return  (m_cfg.p_comm->GetErrCnt() > rimit_err_cnt) ? false: true;
      return false;
    }

    inline uint8_t GetCommStatus() const{
      return m_commStatus.comm_err;
    }

    inline uint8_t AddErrCnt() {
      //m_commStatus.comm_err= 0;
      Recovery();
      return m_errCnt++;
    }

    inline bool IsAlarmState(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR::MOTOR_MAX){
      if (motor_id < AP_OBJ::MOTOR::MOTOR_MAX)
        return m_cfg.p_motor[motor_id].IsAlarmState();
      if(m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsAlarmState())
        return true;
      if(m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsAlarmState())
        return true;
      if(m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsAlarmState())
        return true;
      return false;
    }


    inline bool IsOriginCpltAll(){
      return (m_IsOriginCplt[AP_OBJ::MOTOR_JIG]&m_IsOriginCplt[AP_OBJ::MOTOR_ROLL]&m_IsOriginCplt[AP_OBJ::MOTOR_HIGH]);
    }

    inline bool IsOriginProcessCplt(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id < AP_OBJ::MOTOR::MOTOR_MAX)
      {
        if (IsMotorHoming(motor_id) == false && m_IsOriginCplt[motor_id] == false)
        {
          m_IsOriginCplt[motor_id] =  true;
          return m_IsOriginCplt[motor_id];
        }

        return false;
      }
      else
      {
        if (IsMotorHoming(AP_OBJ::MOTOR_HIGH) == false && m_IsOriginCplt[AP_OBJ::MOTOR_HIGH] == false)
        {
          m_IsOriginCplt[AP_OBJ::MOTOR_HIGH] =  true;
        }
        if (IsMotorHoming(AP_OBJ::MOTOR_ROLL) == false && m_IsOriginCplt[AP_OBJ::MOTOR_ROLL] == false)
        {
          m_IsOriginCplt[AP_OBJ::MOTOR_ROLL] =  true;
        }
        if (IsMotorHoming(AP_OBJ::MOTOR_JIG) == false && m_IsOriginCplt[AP_OBJ::MOTOR_JIG] == false)
        {
          m_IsOriginCplt[AP_OBJ::MOTOR_JIG] =  true;
        }

        return (m_IsOriginCplt[AP_OBJ::MOTOR_HIGH]& m_IsOriginCplt[AP_OBJ::MOTOR_ROLL]& m_IsOriginCplt[AP_OBJ::MOTOR_JIG]);

      }
    }

    inline int AlarmReset() {
      int ret = 0;
      ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].ClearState();
      ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].ClearState();
      ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].ClearState();
      return ret;
    }

    inline MOTOR::enMotor_moons* GetMotorObject(AP_OBJ::MOTOR motor_id){
      if (motor_id < AP_OBJ::MOTOR::MOTOR_MAX)
        return &m_cfg.p_motor[motor_id];
      return nullptr;
    }


    void UpdateMotorsState();

    inline int SetParamDataMove(AP_OBJ::MOTOR motor_id, uint32_t rpm, uint32_t acc, uint32_t dec){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].SetParamDataMove(constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].SetParamDataMove(constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].SetParamDataMove(constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        return ret;
      }
      return m_cfg.p_motor[motor_id].SetParamDataMove(constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
    }

    inline int SetParamDataInit(AP_OBJ::MOTOR motor_id, MOTOR::enMotor_moons::origin_param_t& param){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].SetOriginParam(param);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].SetOriginParam(param);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].SetOriginParam(param);
        return ret;
      }
      return m_cfg.p_motor[motor_id].SetOriginParam(param);
    }

    inline bool IsLinkMoveCplt(){
      bool ret = true;
      ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsInPose()
               &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsInPose();
      return ret;
    }

    inline int LinkMove(int cmd_dist_10um, uint32_t rpm){
      int ret = 0;

      ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative(
          cal_dist_10um_to_pulse(AP_OBJ::MOTOR_JIG, cmd_dist_10um)
          , constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), 100, 100);
      ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative(
          cal_dist_10um_to_pulse(AP_OBJ::MOTOR_ROLL, cmd_dist_10um)
          , cal_sync_vel_dec(constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX)), 100, 100);

      return ret;
    }

    inline int Move(AP_OBJ::MOTOR motor_id, int cmd_dist_10um){

      /*must assign motor id*/
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        if (!IsOriginCpltAll())
          return -1;
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveAbsolutive(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_JIG, cmd_dist_10um));
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveAbsolutive(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_ROLL, cmd_dist_10um));
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveAbsolutive(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_HIGH, cmd_dist_10um));
        return ret;
      }
      if (!(m_IsOriginCplt[motor_id]))
        return -1;
      return m_cfg.p_motor[motor_id].MoveAbsolutive(cal_dist_10um_to_pulse(motor_id, cmd_dist_10um));
    }

    inline int Move(AP_OBJ::MOTOR motor_id, int cmd_dist_10um, uint32_t rpm, uint32_t acc = 100, uint32_t dec = 100){

      /*must assign motor id*/
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        if (!(m_IsOriginCplt[AP_OBJ::MOTOR_JIG]&m_IsOriginCplt[AP_OBJ::MOTOR_ROLL]&m_IsOriginCplt[AP_OBJ::MOTOR_HIGH]))
          return -1;

        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveAbsolutive(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_JIG, cmd_dist_10um)
            , constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveAbsolutive(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_ROLL, cmd_dist_10um)
            , constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveAbsolutive(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_HIGH, cmd_dist_10um)
            , constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        return ret;
      }
      if (!m_IsOriginCplt[motor_id])
        return -1;
      return m_cfg.p_motor[motor_id].MoveAbsolutive( cal_dist_10um_to_pulse(motor_id, cmd_dist_10um), constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
    }

    inline int RelMove(AP_OBJ::MOTOR motor_id, int cmd_dist_10um){

      /*must assign motor id*/
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        if (!(m_IsOriginCplt[AP_OBJ::MOTOR_JIG]&m_IsOriginCplt[AP_OBJ::MOTOR_ROLL]&m_IsOriginCplt[AP_OBJ::MOTOR_HIGH]))
          return -1;
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_JIG, cmd_dist_10um));
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_ROLL, cmd_dist_10um));
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveRelative(cal_dist_10um_to_pulse(AP_OBJ::MOTOR_HIGH, cmd_dist_10um));
        return ret;
      }
      if (!m_IsOriginCplt[motor_id])
        return -1;
      return m_cfg.p_motor[motor_id].MoveRelative(cal_dist_10um_to_pulse(motor_id, cmd_dist_10um));
    }

    inline int RelMove(AP_OBJ::MOTOR motor_id, int cmd_dist_10um, uint32_t rpm, uint32_t acc = 100, uint32_t dec = 100){

      /*must assign motor id*/
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        if (!(m_IsOriginCplt[AP_OBJ::MOTOR_JIG]&m_IsOriginCplt[AP_OBJ::MOTOR_ROLL]&m_IsOriginCplt[AP_OBJ::MOTOR_HIGH]))
          return -1;
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveRelative(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_JIG, cmd_dist_10um),
            constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveRelative(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_ROLL, cmd_dist_10um),
            constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveRelative(
            cal_dist_10um_to_pulse(AP_OBJ::MOTOR_HIGH, cmd_dist_10um),
            constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
        return ret;
      }
      if (!m_IsOriginCplt[motor_id])
        return -1;
      return m_cfg.p_motor[motor_id].MoveAbsolutive( cal_dist_10um_to_pulse(motor_id, cmd_dist_10um), constrain(rpm,1,CN_MOTORS_VELOCITY_RPM_MAX), acc, dec);
    }

    inline int SetOutput(AP_OBJ::MOTOR motor_id, uint16_t pin_no, bool is_on = true){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].SetOutput(pin_no, is_on);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].SetOutput(pin_no, is_on);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].SetOutput(pin_no, is_on);
        return ret;
      }
      return m_cfg.p_motor[motor_id].SetOutput(pin_no, is_on);
    }

    inline int Origin(AP_OBJ::MOTOR motor_id =AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;

        for (int i = 0; i <AP_OBJ::MOTOR_MAX ; i++)
        {
          if (m_cfg.p_motor[i].OriginMotor() == ERROR_SUCCESS)
          {
            m_IsOriginCplt[i] = false;
            ret = ret + 1;
          }
        }
        return (ret == 3? ERROR_SUCCESS : -1 );
      }
      m_IsOriginCplt[motor_id] = false;
      return m_cfg.p_motor[motor_id].OriginMotor();
    }


    inline int MotorClearAlarm(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].ClearState();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].ClearState();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].ClearState();
        return ret;
      }
      return m_cfg.p_motor[motor_id].ClearState();
    }

    inline int MotorZeroEncode(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].ClearEncoder();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].ClearEncoder();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].ClearEncoder();
        return ret;
      }
      return m_cfg.p_motor[motor_id].ClearEncoder();
    }

    inline int MoveToOffset(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){

      /*must assign motor id*/
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveToOffset();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveToOffset();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveToOffset();
        return ret;
      }
      return m_cfg.p_motor[motor_id].MoveToOffset();
    }


    inline int MotorJogMoveStop(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogStop();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogStop();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogStop();
        return ret;
      }
      return m_cfg.p_motor[motor_id].JogStop();
    }

    inline int MotorMoveStop(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].MoveStop();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].MoveStop();
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].MoveStop();
        return ret;
      }
      return m_cfg.p_motor[motor_id].MoveStop();
    }

    inline int MotorJogMove(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX, bool is_cw = true){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogMove(is_cw);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogMove(is_cw);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogMove(is_cw);
        return ret;
      }
      return m_cfg.p_motor[motor_id].JogMove(is_cw);
    }

    inline int MotorJogMove(AP_OBJ::MOTOR motor_id, uart_moons::speed_t& param, bool is_cw){
      param.speed = constrain(param.speed,1,CN_MOTORS_VELOCITY_RPM_MAX);
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        int ret = 0;
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_JIG].JogMove(param,is_cw);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].JogMove(param,is_cw);
        ret += m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].JogMove(param,is_cw);
        return ret;
      }
      return m_cfg.p_motor[motor_id].JogMove(param,is_cw);
    }


    inline bool IsConnected() const{
      return m_cfg.p_comm->IsConnected();
    }

    inline bool IsMotorOn(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        bool ret = true;
        ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorOn()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorOn()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorOn();
        return ret;
      }
      return m_cfg.p_motor[motor_id].IsMotorOn();
    }

    inline bool IsMotorRun(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        bool ret = true;
        ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorRun()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorRun()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorRun();
        return ret;
      }
      return m_cfg.p_motor[motor_id].IsMotorRun();
    }

    inline bool IsMotorHomingAnyOne(){
      bool ret = true;
      ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorHoming()
               |m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorHoming()
               |m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorHoming();
      return ret;
    }

    inline bool IsMotorHoming(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        bool ret = false;
        ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorHoming()
                       |m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorHoming()
                       |m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorHoming();
        return ret;
      }
      return m_cfg.p_motor[motor_id].IsMotorHoming();
    }

    inline bool IsMotorStop(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        bool ret = true;
        ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsMotorStop()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsMotorStop()
                 &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsMotorStop();
        return ret;
      }
      return m_cfg.p_motor[motor_id].IsMotorStop();
    }

    inline bool IsInPosition (AP_OBJ::MOTOR motor_id, int cmd_dist_10um){
      return m_cfg.p_motor[motor_id].IsInPosition(cal_dist_10um_to_pulse(motor_id, cmd_dist_10um));
    }

    inline bool IsInPose(AP_OBJ::MOTOR motor_id = AP_OBJ::MOTOR_MAX){
      if (motor_id == AP_OBJ::MOTOR_MAX)
      {
        bool ret = true;
        ret = m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsInPose()
                       &m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsInPose()
                       &m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsInPose();
        return ret;
      }
      return m_cfg.p_motor[motor_id].IsInPose();
    }

    inline bool IsNoErrStopAndZeroAll(){
      volatile bool ret = true;
      ret &= m_cfg.p_motor[AP_OBJ::MOTOR_JIG].IsNoErrStopAndZero();
      ret &= m_cfg.p_motor[AP_OBJ::MOTOR_HIGH].IsNoErrStopAndZero();
      ret &= m_cfg.p_motor[AP_OBJ::MOTOR_ROLL].IsNoErrStopAndZero();

      return ret;
    }




    inline float cal_sync_vel_dec(uint32_t source_rpm){
      float rate = 0;     ap_dat::dat_t*  p_data{};
      p_data = m_cfg.p_apCfgDat->GetData(ap_dat::sycn_rate);
      rate = ((float)(source_rpm*100)/(float)constrain(p_data->parm1,1,CN_MOTORS_SYNC_DEC_RATE_MAX));
      return rate;
    }

    inline int cal_dist_10um_to_pulse(AP_OBJ::MOTOR motor_id, int um10){
      ap_dat::dat_t*  p_data{};
      int ret = 0;
      int turn_dist_um = 0;
      int turn_by_pulse = 0;
      int dist_by_turn_um = 0;
      switch (motor_id) {
        case AP_OBJ::MOTOR::MOTOR_JIG:
          p_data = m_cfg.p_apCfgDat->GetData(ap_dat::mt_jig_turn_dist);
          break;
        case AP_OBJ::MOTOR::MOTOR_ROLL:
          p_data = m_cfg.p_apCfgDat->GetData(ap_dat::mt_roll_turn_dist);
          break;
        case AP_OBJ::MOTOR::MOTOR_HIGH:
          p_data = m_cfg.p_apCfgDat->GetData(ap_dat::mt_high_turn_dist);
          break;
        default:
          break;
      }
      turn_dist_um =um10*10;
      dist_by_turn_um = int(constrain(p_data->parm1,1,CN_MOTORS_TURN_DIST_MM_MAX))*1000;
      turn_by_pulse = (int)constrain(p_data->parm2,CN_MOTORS_GEAR_RATE_MIN,CN_MOTORS_GEAR_RATE_MAX);
      ret = int(((float)turn_dist_um/(float)dist_by_turn_um) * (float)turn_by_pulse);

      return ret;
    }

#endif
#if 0


    inline int EStop (uint8_t id = static_cast<uint8_t>(cnMotors::mot_Id::_max)){
      int ret = 0;
      if (id < static_cast<uint8_t>(cnMotors::mot_Id::_max))
      {
        ret = m_Cfg.p_motor[id].EStop();
      }
      else if (id == static_cast<uint8_t>(cnMotors::mot_Id::_max))
      {
        ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_JIG].EStop();
        ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_ROLL].EStop();
        ret += m_Cfg.p_motor[AP_DEF_OBJ_MOTOR_ID_HIGH].EStop();

      }
      return ret;
    }

#endif
};
#endif /* AP__INC_CNMOTORS_HPP_ */
