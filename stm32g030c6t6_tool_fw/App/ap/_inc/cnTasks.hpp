/*
 * cnTasks.hpp
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef AP__INC_CNTASKS_HPP_
#define AP__INC_CNTASKS_HPP_

constexpr uint8_t line_index_max = 200;

struct cnTasks
{

  using line_t = sequece_idx_data_st::linetype_e;
  using pos_idx_t = pos_data_st::idx_e;
  using pos_data_t = pos_data_st;


  struct cfg_t
  {
    ap_reg* ptr_apReg;
    enOp* ptr_op;
    MOTOR::cnMotors* ptr_motors{};
    taskDat* ptr_taskDat{};
    //ap_log* ptr_apLog;
    ap_io* ptr_io;
    cnAuto* ptr_AutoManger;

    cfg_t() = default;
    ~cfg_t() = default;

    // copy constructor
    cfg_t(const cfg_t& rhs) = default;
    // copy assignment operator
    cfg_t& operator=(const cfg_t& rhs) = default;
    // move constructor
    cfg_t(cfg_t&& rhs) = default;
    // move assignment operator
    cfg_t& operator=(cfg_t&& rhs) = default;

  };

  cfg_t m_cfg;
  prc_step_t m_step;
  uint8_t m_idx;
  std::array<sequece_idx_data_st*, line_index_max> m_lines;
  //std::array<pos_data_t, pos_idx_t::mdi_max> m_pos;

  cnTasks() = default;
  ~cnTasks() = default;

  // copy constructor
  cnTasks(const cnTasks& rhs) = default;
  // copy assignment operator
  cnTasks& operator=(const cnTasks& rhs) = default;
  // move constructor
  cnTasks(cnTasks&& rhs) = default;
  // move assignment operator
  cnTasks& operator=(cnTasks&& rhs) = default;


  inline int Init(cnTasks::cfg_t& cfg) {
    LOG_PRINT("Init Success!");
    m_cfg = cfg;
    return ERROR_SUCCESS;
  }

	inline void ThreadJob()
	{
		switch (m_cfg.ptr_AutoManger->GetOPStatus())
		{
		case enOp::status_e::RUN_READY:
			break;

		case enOp::status_e::RUN:
		{
			switch (m_cfg.ptr_AutoManger->GetOPMode())
			{
			case enOp::mode_e::DRY_RUN:__attribute__((fallthrough));
			case enOp::mode_e::AUTORUN:
				if (!m_cfg.ptr_AutoManger->IsDetectAreaSensor()) //not
					doRunStep(m_idx);
				break;
			case enOp::mode_e::READY:			break;
			case enOp::mode_e::STOP:			break;
			default:					break;
			}
			// end of switch (GetOPMode())
		}
		break;

		case enOp::status_e::INIT:__attribute__((fallthrough));
		case enOp::status_e::STEP_STOP:__attribute__((fallthrough));
		case enOp::status_e::ERR_STOP:
			break;

		default:			break;
		}
		// end of switch (m_cfg.ptr_AutoManger->GetOPStatus())
	}

  inline bool IsMotorRun()
  {
    return false;
  }
  inline bool IsMoveCplt()
  {
    return false;
  }

	inline errno_t MoveToPos(pos_data_st::idx_e idx)
	{

		return 0;
	}

  inline void doRunStep(uint8_t idx = 0)
  {
    sequece_idx_data_st* line = m_lines[idx];
    constexpr uint32_t step_wait_delay = 300;
    constexpr uint8_t retry_max = 3;
    constexpr uint32_t move_timeout = 1'000 * 5;

    enum : uint8_t
    {
      STEP_INIT,
      STEP_TODO,
      STEP_TIMEOUT,
      STEP_WAIT_DONE,
      STEP_WAIT_RETURN,
      STEP_DO_INITAIL,
      STEP_DO_INITAIL_START,
      STEP_DO_INITAIL_WAIT,
      STEP_DO_INITAIL_END,
      STEP_DO_STANDBY,
      STEP_DO_STANDBY_START,
      STEP_DO_STANDBY_WAIT,
      STEP_DO_STANDBY_END,
      STEP_DO_IDX1,
      STEP_DO_IDX1_START,
      STEP_DO_IDX1_WAIT,
      STEP_DO_IDX1_END,
    };

    switch (m_step.GetStep())
    {
    case STEP_INIT:
    {
      LOG_PRINT("STEP_INIT");
      m_idx = 0;
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

    case STEP_TIMEOUT:
    {
    }
    break;

    case STEP_WAIT_DONE:
    {
    }
    break;

    case STEP_WAIT_RETURN:
    {
    }
    break;
    ////////////////////////////////////////////////////
    /*######################################################
       do app system initialize
      ######################################################*/
    case STEP_DO_INITAIL:
    {

    }
    break;
    case STEP_DO_INITAIL_START:
    {

    }
    break;
    case STEP_DO_INITAIL_WAIT:
    {

    }
    break;
    case STEP_DO_INITAIL_END:
    {

    }
    break;
    /*######################################################
       do system standby state and wait start
      ######################################################*/
    case STEP_DO_STANDBY:
    {

    }
    break;
    case STEP_DO_STANDBY_START:
    {

    }
    break;
    case STEP_DO_STANDBY_WAIT:
    {

    }
    break;
    case STEP_DO_STANDBY_END:
    {

    }
    break;
    /*######################################################
       line index
      ######################################################*/
    case STEP_DO_IDX1:
    {
      if (line->entry_setout)
      {
        LOG_PRINT("entry_setout : OutputOn Set [%d]",line->entry_setout + AP_DEF_START_OUT_ADDR );
        m_cfg.ptr_io->OutputOn((uint32_t)(line->entry_setout + AP_DEF_START_OUT_ADDR));
        // io set out
      }

      m_step.retry_cnt = 0;
      m_step.wait_step = 0;
      m_step.SetStep(STEP_DO_IDX1_START);
    }
    break;

    case STEP_DO_IDX1_START:
    {
      if (line->entry_delay > 0)
      {
        if (m_step.LessThan((uint32_t)line->entry_delay))
          break;
      }

      if (line->line_type == line_t::lt_pos)
      {
        // m_pos[line->pos_data_idx].data.pos_x
        if (MoveToPos(line->pos_data_idx) == ERROR_SUCCESS)
        {
          m_step.SetStep(STEP_DO_IDX1_WAIT);
        }
      }
      else
      {
      }
    }
    break;

    case STEP_DO_IDX1_WAIT:
    {
      if (m_step.LessThan(step_wait_delay))
        break;

      if (line->line_type == line_t::lt_pos)
      {
        if (this->IsMoveCplt())
        {
          m_step.SetStep(STEP_DO_IDX1_END);
          break;
        }

        if (IsMotorRun() == false)
        {
          if (m_step.retry_cnt++ < retry_max)
          {
            m_step.SetStep(STEP_DO_IDX1_START);
            break;
          }
        }

        if (m_step.MoreThan(move_timeout))
        {
          // timeout error
        }
      }
      else
      {
        LOG_PRINT("condition_in : IsOn Set [%d]",line->condition_in + AP_DEF_START_IN_ADDR);
        if ( m_cfg.ptr_io->IsOn((uint32_t)(line->condition_in + AP_DEF_START_IN_ADDR))) //
          m_idx = line->condition_pass_line;
        else
          m_idx = line->condition_fail_line;
      }
    }
    break;

    case STEP_DO_IDX1_END:
    {
      if (line->exit_setout)
      {
        LOG_PRINT("line->exit_setout : OutputOn Set [%d]",line->exit_setout + AP_DEF_START_OUT_ADDR);
        // io set out
        m_cfg.ptr_io->OutputOn((uint32_t)(line->exit_setout + AP_DEF_START_OUT_ADDR));
      }

      if (line->exit_delay > 0)
      {
        if (m_step.LessThan((uint32_t)line->exit_delay))
          break;
      }
    }
    break;
    ////////////////////////////////////////////////////
    default:
      break;
    }
  }

  inline void RunLineTask(uint8_t idx)
  {
    constexpr auto STEP_DO_IDX1 = 13;
    m_idx = idx;
    m_lines[0] =  &m_cfg.ptr_taskDat->task_dat[0].line_data;
    m_lines[1] =  &m_cfg.ptr_taskDat->task_dat[1].line_data;
    m_lines[2] =  &m_cfg.ptr_taskDat->task_dat[2].line_data;
    m_lines[3] =  &m_cfg.ptr_taskDat->task_dat[3].line_data;
    m_step.SetStep(STEP_DO_IDX1);
  }

};
//end of cnTasks




#endif /* AP__INC_CNTASKS_HPP_ */
