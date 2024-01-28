#pragma once
#ifndef _FM_PXY_HPP
#define _FM_PXY_HPP

#include "../def.hpp"

/**
 * @brief
 * functional modules portable XY system class
 */
namespace fmpXY
{

	struct AxisSystemX
	{
		uint16_t pos_x{}; // max position data 650mm

		AxisSystemX() = default;
		AxisSystemX(uint16_t x) : pos_x{x} {};
		~AxisSystemX() = default;
		// copy constructor
		AxisSystemX(const AxisSystemX &rhs) = default;
		// copy assignment operator
		AxisSystemX &operator=(const AxisSystemX &rhs) = default;
		// move constructor
		AxisSystemX(AxisSystemX &&rhs) = default;
		// move assignment operator
		AxisSystemX &operator=(AxisSystemX &&rhs) = default;

		void Init()
		{
			pos_x = 0;
		}
	};

	struct AxisSystemXY
	{
		uint16_t pos_x{}; // max position data 650mm
		uint16_t pos_y{}; // max position data 650mm

		AxisSystemXY() = default;
		AxisSystemXY(uint16_t x, uint16_t y) : pos_x{x}, pos_y{y} {};
		~AxisSystemXY() = default;
		// copy constructor
		AxisSystemXY(const AxisSystemXY &rhs) = default;
		// copy assignment operator
		AxisSystemXY &operator=(const AxisSystemXY &rhs) = default;
		// move constructor
		AxisSystemXY(AxisSystemXY &&rhs) = default;
		// move assignment operator
		AxisSystemXY &operator=(AxisSystemXY &&rhs) = default;

		void Init()
		{
			pos_x = 0;
			pos_y = 0;
		}
	};

	struct AxisSystemXYZ
	{
		uint16_t pos_x{}; // max position data 650mm
		uint16_t pos_y{}; // max position data 650mm
		uint16_t pos_z{}; // max position data 650mm

		AxisSystemXYZ() = default;
		AxisSystemXYZ(uint16_t x, uint16_t y, uint16_t z) : pos_x{x}, pos_y{y}, pos_z{z} {};
		~AxisSystemXYZ() = default;
		// copy constructor
		AxisSystemXYZ(const AxisSystemXYZ &rhs) = default;
		// copy assignment operator
		AxisSystemXYZ &operator=(const AxisSystemXYZ &rhs) = default;
		// move constructor
		AxisSystemXYZ(AxisSystemXYZ &&rhs) = default;
		// move assignment operator
		AxisSystemXYZ &operator=(AxisSystemXYZ &&rhs) = default;

		void Init()
		{
			pos_x = 0;
			pos_y = 0;
			pos_z = 0;
		}
	};

	/* axis system data type requires */
	template <typename P>
	concept axis_c = std::same_as<P, AxisSystemX> || std::same_as<P, AxisSystemXY> || std::same_as<P, AxisSystemXYZ>;

	template <typename DataType = AxisSystemX>
		requires axis_c<DataType>
	struct pos_data_st
	{
		using type = DataType;
		enum idx_e : uint8_t
		{
			mdi_null,
			mdi_0,
			mdi_1,
			mdi_2,
			mdi_3,

			mdi_253,
			mdi_max // max 255
		};

		/*position unit = 10um*/
		/*velocity unit =rpm */
		/*acc dec unit =m/ss */
		// max position data 650mm
		DataType data{};
		uint16_t rpm{};
		uint16_t acc{};
		uint16_t dec{};

		pos_data_st() = default;
		pos_data_st(DataType da, uint16_t r, uint16_t a, uint16_t d) : data{da}, rpm{r}, acc{a}, dec{d} {}
		~pos_data_st() = default;
		// copy constructor
		pos_data_st(const pos_data_st &rhs) = default;
		// copy assignment operator
		pos_data_st &operator=(const pos_data_st &rhs) = default;
		// move constructor
		pos_data_st(pos_data_st &&rhs) = default;
		// move assignment operator
		pos_data_st &operator=(pos_data_st &&rhs) = default;

		void Init()
		{
			data.Init();
			rpm = 0;
			acc = 0;
			dec = 0;
		}
	};
	// end of pos_data_st
	template <typename DataType>
	pos_data_st(DataType t) -> pos_data_st<DataType>;

	static std::string pos_data_name(pos_data_st<>::idx_e idx)
	{
		using index_t = pos_data_st<>::idx_e;
		switch (idx)
		{
		case index_t::mdi_0:
			return "mdi_0";
			break;
		case index_t::mdi_1:
			return "mdi_1";
			break;
		case index_t::mdi_2:
			return "mdi_2";
			break;

		default:
			return "";
		}
		// end of switch
	}

	struct mcu_tool_data_align_48_t
	{
	private:
		static constexpr auto io_dword_max = 1;
		static constexpr auto motor_dword_max = 3;
		static constexpr auto data_dword_max = 2;

	public:
		enum dat_addr : uint8_t
		{
			addr_system = 0,
			addr_state = 2,
			addr_option = 4,
			addr_err = 8,
			addr_io_in_0 = 12,
			addr_io_out_0 = 16,
			addr_motor_cnt = 20,
			addr_motor_reserved = 22,
			addr_motor_pulse_0 = 24,
			addr_motor_pulse_1 = 28,
			addr_motor_pulse_2 = 32,
			addr_motor_state_0 = 36,
			addr_motor_state_1 = 38,
			addr_motor_state_2 = 40,
			addr_motor_reserved2 = 42,
			addr_datas_0 = 44,
			addr_datas_1 = 46,
		};
		uint16_t reg_sys{};                                   //
		uint16_t reg_state{};                                 //
		uint32_t reg_option{};                                //
		uint32_t reg_err{};                                   // 
		std::array<uint32_t, io_dword_max> io_in{};           // 
		std::array<uint32_t, io_dword_max> io_out{};          // 
		uint16_t motor_cnt{};                                 //
		uint16_t motor_reserved{};                            // 24
		std::array<uint32_t, motor_dword_max> motor_pulse{};  // 36
		std::array<uint16_t, motor_dword_max> motor_status{}; // 
		uint16_t motor_reserved2{};                           // 44
		std::array<uint16_t, data_dword_max> datas{};         // 48

		mcu_tool_data_align_48_t() = default;
		~mcu_tool_data_align_48_t() = default;
		// copy constructor
		mcu_tool_data_align_48_t(const mcu_tool_data_align_48_t& rhs) = default;
		// copy assignment operator
		mcu_tool_data_align_48_t& operator=(const mcu_tool_data_align_48_t& rhs) = default;
		// move constructor
		mcu_tool_data_align_48_t(mcu_tool_data_align_48_t&& rhs) = default;
		// move assignment operator
		mcu_tool_data_align_48_t& operator=(mcu_tool_data_align_48_t&& rhs) = default;
	};


	struct mcu_data_align_48_t
	{
	private:
		static constexpr auto io_dword_max = 2;
		static constexpr auto motor_dword_max = 2;
		static constexpr auto data_dword_max = 1;

	public:
		enum dat_addr : uint8_t
		{
			addr_system = 0x00,
			addr_state = 0x02,
			addr_option = 0x04,
			addr_err = 0x08,
			addr_io_in_0 = 0x0C,
			addr_io_in_1 = 0x10,
			addr_io_out_0 = 0x14,
			addr_io_out_1 = 0x18,
			addr_motor_cnt = 0x1C,
			addr_motor_reserved = 0x1E,
			addr_motor_pulse_0 = 0x20,
			addr_motor_pulse_1 = 0x24,
			addr_motor_state_0 = 0x28,
			addr_motor_state_1 = 0x2A,
			addr_datas_0 = 0x2C,
		};
		uint16_t reg_sys{};
		uint16_t reg_state{};
		uint32_t reg_option{};
		uint32_t reg_err{};
		std::array<uint32_t, io_dword_max> io_in{};
		std::array<uint32_t, io_dword_max> io_out{};
		uint16_t motor_cnt{};
		uint16_t motor_reserved{};														// 32
		std::array<uint32_t, motor_dword_max> motor_pulse{};	// 40
		std::array<uint16_t, motor_dword_max> motor_status{}; // 44
		std::array<uint32_t, data_dword_max> datas{};

		mcu_data_align_48_t() = default;
		~mcu_data_align_48_t() = default;
		// copy constructor
		mcu_data_align_48_t(const mcu_data_align_48_t &rhs) = default;
		// copy assignment operator
		mcu_data_align_48_t &operator=(const mcu_data_align_48_t &rhs) = default;
		// move constructor
		mcu_data_align_48_t(mcu_data_align_48_t &&rhs) = default;
		// move assignment operator
		mcu_data_align_48_t &operator=(mcu_data_align_48_t &&rhs) = default;
	};

	struct sequece_idx_data_st
	{

		using idx_t = pos_data_st<>::idx_e;

		enum linetype_e : uint8_t
		{
			lt_sequence,
			lt_pos,
		};

		struct opt_st
		{
			enum opt_e : uint8_t
			{
				opt_sim_input_reg,
				opt_01,
				opt_02,
				opt_03,
				opt_04,
				opt_05,
				opt_06,
				opt_07,

				opt_10,
				opt_11,
				opt_12,
				opt_13,
				opt_14,
				opt_15,
				opt_16,
				opt_17,

				opt_20,
				opt_21,
				opt_22,
				opt_23,
				opt_24,
				opt_25,
				opt_26,
				opt_27,

				opt_30,
				opt_31,
				opt_32,
				opt_33,
				opt_34,
				opt_35,
				opt_36,
				opt_37,

				opt_max,

			};

			std::bitset<opt_e::opt_max> opt_sr;

			bool Sim_InputReg() const { return opt_sr.test(opt_e::opt_sim_input_reg); }
		};

		uint16_t idx{};
		uint16_t next_line{};

		linetype_e line_type{};
		idx_t pos_data_idx{};

		opt_st opt_reg{};
		uint32_t entry_setout{};
		uint32_t exit_setout{};

		uint16_t entry_delay{}; // ms
		uint16_t exit_delay{};	// ms

		uint32_t condition_in{};
		uint16_t condition_pass_line{};
		uint16_t condition_fail_line{};

		sequece_idx_data_st() = default;

		sequece_idx_data_st(uint16_t i, uint16_t n, linetype_e t, idx_t pos_i, opt_st opt,
												uint32_t en_out, uint32_t ex_out, uint32_t en_dly, uint32_t ex_dly, uint32_t con_in,
												uint16_t pass_n, uint16_t fail_n) : idx(i), next_line(n), line_type(t), pos_data_idx(pos_i), opt_reg(opt),
																														entry_setout(en_out), exit_setout(ex_out), entry_delay(en_dly), exit_delay(ex_dly),
																														condition_in(con_in), condition_pass_line(pass_n), condition_fail_line(fail_n)
		{
		}
		~sequece_idx_data_st() = default;
		// copy constructor
		sequece_idx_data_st(const sequece_idx_data_st &rhs) = default;
		// copy assignment operator
		sequece_idx_data_st &operator=(const sequece_idx_data_st &rhs) = default;
		// move constructor
		sequece_idx_data_st(sequece_idx_data_st &&rhs) = default;
		// move assignment operator
		sequece_idx_data_st &operator=(sequece_idx_data_st &&rhs) = default;
	};
	// end of sequece_idx_data_st

	struct taskDat
	{

		struct dat_t
		{
			uint16_t index{};
			sequece_idx_data_st line_data{};
		};

		enum idx_e : uint8_t //
		{
			idx_curr,
			idx_frnt1,
			idx_frnt2,
			idx_frnt3,
			idx_frnt4,
			_max
		};

		std::array<dat_t, _max> task_dat{};

		inline dat_t *GetData()
		{
			return task_dat.data();
		}
	};

	constexpr uint8_t line_index_max = 200;

	template <typename DataType = AxisSystemX>
		requires axis_c<DataType>
	struct cnTasks
	{

		using line_t = sequece_idx_data_st::linetype_e;
		using pos_data_t = pos_data_st<DataType>;
		using pos_idx_t = pos_data_t::idx_e;
		struct cfg_t
		{
			taskDat *ptr_taskDat{};
			cfg_t() = default;
			~cfg_t() = default;
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
		step::state_st<> m_step{};
		uint8_t m_idx{};
		std::array<sequece_idx_data_st, line_index_max> m_lines{};
		std::array<pos_data_t, pos_idx_t::mdi_max> m_pos{};

		cnTasks() = default;
		~cnTasks() = default;
		// copy constructor
		cnTasks(const cnTasks &rhs) = default;
		// copy assignment operator
		cnTasks &operator=(const cnTasks &rhs) = default;
		// move constructor
		cnTasks(cnTasks &&rhs) = default;
		// move assignment operator
		cnTasks &operator=(cnTasks &&rhs) = default;

		inline void ThreadJob()
		{

#if 0
			//auto
			switch (m_cfg.ptr_AutoManger->GetOPStatus())
			{
			case enOp::status_e::RUN_READY:
				m_IsAutoReady = true;
				break;

			case enOp::status_e::RUN:
				switch (m_cfg.p_AutoManger->GetOPMode())
				{
				case enOp::mode_e::DRY_RUN:__attribute__((fallthrough));
				case enOp::mode_e::AUTORUN:
					if (!m_cfg.p_AutoManger->IsDetectAreaSensor()) //not
						doRunStep();
					break;
				case enOp::mode_e::READY:			break;
				case enOp::mode_e::STOP:			break;
				default:					break;
				}
				// end of switch (GetOPMode())
				break;

			case enOp::status_e::INIT:__attribute__((fallthrough));
			case enOp::status_e::STEP_STOP:__attribute__((fallthrough));
			case enOp::status_e::ERR_STOP:
				m_IsAutoReady = false;
				doRunStep();
				break;

			default:			break;
			}
#endif

			doRunStep(m_idx);
		}

		inline bool IsMotorRun()
		{
			return false;
		}

		inline bool IsMoveCplt()
		{
			return false;
		}

		inline errno_t MoveToPos(pos_data_st<>::idx_e idx)
		{
			using type = decltype(m_pos[0].data);

			if constexpr (std::is_same_v<type, AxisSystemX>)
			{
				// std::cout << "std::is_same_v<type, AxisSystemX>";
				return 0;
			}
			else if constexpr (std::is_same_v<type, AxisSystemXY>)
			{
				// std::cout << "std::is_same_v<type, AxisSystemXY>";
				return 0;
			}
			else if constexpr (std::is_same_v<type, AxisSystemXYZ>)
			{
				// std::cout << "std::is_same_v<type, AxisSystemXYZ>";
				return 0;
			}
		}

		inline void doRunStep(uint8_t idx = 0)
		{
			sequece_idx_data_st *line = &m_lines[idx];
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
				std::cout << "STEP_INIT" << std::endl;
				m_idx = 0;
				m_step.SetStep(STEP_TODO);
			}
			break;

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
			////////////////////////////////////////////////////
			case STEP_DO_IDX1:
			{
				if (line->entry_setout)
				{
					// io set out
					std::cout << "entry_setout : " << line->entry_setout << std::endl;
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
					std::cout << "condition_in :" << line->condition_in << std::endl;
					std::cout << "condition_pass_line :" << line->condition_pass_line << std::endl;
					std::cout << "condition_fail_line :" << line->condition_fail_line << std::endl;
					if (1) // line->condition_in
						m_idx = (uint8_t)line->condition_pass_line;
					else
						m_idx = (uint8_t)line->condition_fail_line;
				}
			}
			break;

			case STEP_DO_IDX1_END:
			{
				if (line->exit_setout)
				{
					// io set out
					std::cout << "exit_setout :" << line->exit_setout << std::endl;
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
			m_lines[0] = &m_cfg.ptr_taskDat->task_dat[0].line_data;
			m_lines[1] = &m_cfg.ptr_taskDat->task_dat[1].line_data;
			m_lines[2] = &m_cfg.ptr_taskDat->task_dat[2].line_data;
			m_lines[3] = &m_cfg.ptr_taskDat->task_dat[3].line_data;
			m_step.SetStep(STEP_DO_IDX1);
		}
	};
	// end of TaskEdit

	constexpr int MODULE_XYIO_MAX_DATA_LENGTH = 120;
	constexpr int MODULE_XYIO_MAX_PACKET_LENGTH = (MODULE_XYIO_MAX_DATA_LENGTH + 8);

	const uint8_t MODULE_XYIO_CMD_STX = 0x4A;
	const uint8_t MODULE_XYIO_CMD_ETX = 0x4C;

	namespace OBJ
	{
		enum MOTOR
		{
			MOTOR_X,
			MOTOR_Y,

			MOTOR_MAX,

		};

		enum CYL
		{

			CYL_MAX,

		};

		enum VAC
		{

			VAC_MAX,
		};

	}

	class ModuleXYIO
	{
	public:
		struct cfg_t
		{
			serial_comm *ptr_uart_comm{};

			cfg_t() = default;

			// copy constructor
			cfg_t(const cfg_t &other) = default;
			// copy assignment
			cfg_t &operator=(const cfg_t &other) = default;
			// move constructor
			cfg_t(cfg_t &&other) = default;
			// move assignment
			cfg_t &operator=(cfg_t &&other) = default;
		};

		// RX  (mcu -> pc) provide information
		enum RX_TYPE : uint8_t
		{
			RX_MCU_DATA = 0x01,
			RX_MOTOR_DATA = 0x02,
			RX_MOTOR_CFG_MOTION_ORIGIN = 0x03,
			RX_TOOL_PnP_DATA = 0x04,

			RX_EEPROM_MOTION_DATA_L = 0x11,
			RX_EEPROM_MOTION_DATA_H = 0x12,
			RX_EEPROM_CONFIG_DATA = 0x13,
			RX_EEPROM_CYLINDER_DATA = 0x14,
			RX_EEPROM_VACUUM_DATA = 0x15,
			RX_EEPROM_SEQUNCE_DATA = 0x16,
			RX_EEPROM_LOG_DATA = 0x19,


			RX_FIRM_INFO = 0x50,
			RX_BOOT_INFO = 0x51,
			RX_OK_RESPONSE = 0xAA
		};

		// TX(pc->mcu) request information or action
		enum CMD_TYPE : uint8_t
		{
			CMD_OK_RESPONSE = 0x00,
			CMD_READ_MCU_DATA = 0x01,
			CMD_READ_FIRM_INFO = 0x02,

			CMD_CTRL_IO_OUT = 0x10,
			CMD_CTRL_CYL = 0x11,
			CMD_CTRL_VAC = 0x12,
			CMD_CTRL_MCU_OPTION_REG = 0x13,

			CMD_CTRL_JOB_INITIAL = 0x1A,
			CMD_CTRL_VIRTUAL_SW = 0x1B,

			CMD_CTRL_MOT_ORIGIN = 0x20,
			CMD_CTRL_MOT_ONOFF = 0x21,
			CMD_CTRL_MOT_MOVE = 0x22,
			CMD_CTRL_MOT_STOP = 0x23,
			CMD_CTRL_MOT_JOG = 0x24,
			CMD_CTRL_MOT_LIMIT = 0x25,
			CMD_CTRL_MOT_ZEROSET = 0x26,
			CMD_CTRL_MOT_RELMOVE = 0x27,
			CMD_CTRL_MOT_CLEAR_ALARM = 0x28,
			CMD_CTRL_MOT_CHANGE_VEL = 0x29,
			CMD_CTRL_MOT_MOVE_VEL = 0x2A,
			CMD_CTRL_MOT_RELMOVE_VEL = 0x2B,
			CMD_CTRL_MOT_VEL_JOG = 0x2C,

			CMD_CTRL_MOT_DATA_MOVE_PARAM = 0x3A,
			CMD_CTRL_MOT_DATA_INIT_PARAM = 0x3B,

			CMD_EEPROM_WRITE_MOTOR_POS_DATA_L = 0x40,
			CMD_EEPROM_WRITE_MOTOR_POS_DATA_H = 0x41,
			CMD_EEPROM_WRITE_CFG_DATA = 0x42,
			CMD_EEPROM_WRITE_CYL_DATA = 0x43,
			CMD_EEPROM_WRITE_VAC_DATA = 0x44,
			CMD_EEPROM_WRITE_SEQ_DATA = 0x45,
			CMD_EEPROM_READ_MOTOR_POS_DATA = 0x48,
			CMD_EEPROM_WRITE_TEACHING_DATA = 0x49,
			//0x49,
			CMD_EEPROM_READ_CFG_DATA = 0x4A,
			CMD_EEPROM_READ_CYL_DATA = 0x4B,
			CMD_EEPROM_READ_VAC_DATA = 0x4C,
			CMD_EEPROM_READ_SEQ_DATA = 0x4D,
			CMD_EEPROM_READ_LINK_POS = 0x4E,
			CMD_EEPROM_READ_LOG = 0x4F,

			CMD_EEPROM_CLEAR_ROM_DATA = 0x50,
			CMD_EEPROM_RELOAD_ROM_DATA = 0x51,

			CMD_READ_MOTOR_DATA = 0x61,
			CMD_READ_MOTION_ORG_CFG_DATA = 0x62,
			CMD_READ_CTRL_DATA = 0x63,

			CMD_READ_TOOL_PnP_DATA = 0x70,
			CMD_CTRL_TOOL_PnP = 0x71,

			CMD_RESET_SELFTEST_CPLT = 0x90,
			CMD_RESET_WORK_CNT = 0x91,
			CMD_FW_TO_BOOTMODE = 0x92,
			CMD_SET_FACTORYDATA = 0x93,
			CMD_SET_SELFTEST_CPLT = 0x94,
		};

		enum tool_PnP_order_e : int16_t
		{
			TOOL_CMD_ERR_RET = -1,
			TOOL_CMD_NONE = 0,
			TOOL_CMD_ORD_MOTOR_ENABLE = 1,
			TOOL_CMD_ORD_MOTOR_DISABLE,
			TOOL_CMD_ORD_MOTOR_RUN,
			TOOL_CMD_ORD_MOTOR_ORG,
			TOOL_CMD_ORD_4,
		};

		struct firm_version_st
		{
			uint8_t firm_version[32]{};
			uint8_t firm_name[32]{};
		};

		struct packet_st
		{
			uint8_t rx_type{};
			uint8_t obj_id{};
			uint16_t data_length{};
			uint16_t checksum{};
			uint16_t checksum_recv{};
			std::array<uint8_t, MODULE_XYIO_MAX_PACKET_LENGTH> buffer{};
			uint8_t buffer_idx{};
			uint8_t *data{};
			uint16_t data_cnt{};

			step::state_st<> state{};

			packet_st() = default;

			// copy constructor
			packet_st(const packet_st &other) = default;

			// copy assignment
			packet_st &operator=(const packet_st &other) = default;

			// move constructor
			packet_st(packet_st &&other) = default;

			// move assignment

			packet_st &operator=(packet_st &&other) = default;
			// destructor
			~packet_st() = default;

			uint8_t BufferAdd(uint8_t rx_data)
			{
				buffer[buffer_idx] = rx_data;
				buffer_idx = (buffer_idx + 1) % MODULE_XYIO_MAX_PACKET_LENGTH;
				return buffer_idx;
			}

			void BufferClear()
			{
				buffer.fill(0);
				buffer_idx = 0;
				checksum = 0xffff;
				state.SetStep(0);
			}
		};

		cfg_t m_cfg{};
		step::state_st<> m_step{};
		packet_st m_packetData{};
		bool m_waitResp{};
		uint32_t m_respTime{};
		mcu_data_align_48_t m_mcuData{};
		mcu_tool_data_align_48_t m_toolData{};
		std::thread m_trd{};
		std::atomic<bool> m_stopThread{};
		std::function<int(void *, void *)> m_cb{};
		std::mutex m_mutex{};
		firm_version_st m_firmInfo{};
		firm_version_st m_bootInfo{};
		// constructor
		ModuleXYIO() = default;

		// copy constructor
		ModuleXYIO(const ModuleXYIO &other) = default;
		// copy assignment
		ModuleXYIO &operator=(const ModuleXYIO &other) = default;
		// move constructor
		ModuleXYIO(ModuleXYIO &&other) = default;
		// move assignment
		ModuleXYIO &operator=(ModuleXYIO &&other) = default;

	private:
		inline void processCplt()
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			std::cout << "RxData.Type (resp time): 0x" << std::hex << (int)m_packetData.rx_type << std::dec << "(" << m_respTime << ")" << std::endl;
			Ret_OkResponse();
			RX_TYPE rx_type = (RX_TYPE)m_packetData.rx_type;

			std::cout << std::dec;
			if (m_packetData.data == nullptr)
			{
				LOG_PRINT("received data is null, rx type[0x%02X]", m_packetData.rx_type);
				return;
			}
			switch (rx_type)
			{
			case RX_TYPE::RX_MCU_DATA:
				m_mcuData = *((mcu_data_align_48_t *)m_packetData.data);
				std::cout << "mcu data (word ) reg_sys : " << m_mcuData.reg_sys << std::endl;
				std::cout << "mcu data (word ) reg_state : " << m_mcuData.reg_state << std::endl;
				std::cout << "mcu data (dword) reg_option : " << m_mcuData.reg_option << std::endl;
				std::cout << "mcu data (dword) reg_err : " << m_mcuData.reg_err << std::endl;
				std::cout << "mcu data (dword) io_in[0] : " << m_mcuData.io_in[0] << std::endl;
				std::cout << "mcu data (dword) io_in[1] : " << m_mcuData.io_in[1] << std::endl;
				std::cout << "mcu data (dword) io_out[0] : " << m_mcuData.io_out[0] << std::endl;
				std::cout << "mcu data (dword) io_out[1] : " << m_mcuData.io_out[1] << std::endl;
				std::cout << "mcu data (word ) motor_cnt : " << m_mcuData.motor_cnt << std::endl;
				std::cout << "mcu data (dword) motor_pos[0] : " << m_mcuData.motor_pulse[0] << std::endl;
				std::cout << "mcu data (word ) motor_status[0] : " << m_mcuData.motor_status[0] << std::endl;
				std::cout << "mcu data (dword) motor_pos[1] : " << m_mcuData.motor_pulse[1] << std::endl;
				std::cout << "mcu data (word ) motor_status[1] : " << m_mcuData.motor_status[1] << std::endl;
				std::cout << "mcu data (dword) datas[0] : " << m_mcuData.datas[0] << std::endl;

				break;

			case RX_TYPE::RX_TOOL_PnP_DATA:
				m_toolData = *((mcu_tool_data_align_48_t*)m_packetData.data);
				std::cout << "tool data (word ) reg_sys : " << m_toolData.reg_sys << std::endl;
				std::cout << "tool data (word ) reg_state : " << m_toolData.reg_state << std::endl;
				std::cout << "tool data (dword) reg_option : " << m_toolData.reg_option << std::endl;
				std::cout << "tool data (dword) reg_err : " << m_toolData.reg_err << std::endl;
				std::cout << "tool data (dword) io_in[0] : " << m_toolData.io_in[0] << std::endl;
				std::cout << "tool data (dword) io_out[0] : " << m_toolData.io_out[0] << std::endl;
				std::cout << "tool data (word ) motor_cnt : " << m_toolData.motor_cnt << std::endl;
				std::cout << "tool data (dword) motor_pos[0] : " << m_toolData.motor_pulse[0] << std::endl;
				std::cout << "tool data (word ) motor_status[0] : " << m_toolData.motor_status[0] << std::endl;
				std::cout << "tool data (dword) motor_pos[1] : " << m_toolData.motor_pulse[1] << std::endl;
				std::cout << "tool data (word ) motor_status[1] : " << m_toolData.motor_status[1] << std::endl;
				std::cout << "tool data (dword) motor_pos[2] : " << m_toolData.motor_pulse[2] << std::endl;
				std::cout << "tool data (word ) motor_status[2] : " << m_toolData.motor_status[2] << std::endl;
				std::cout << "tool data (word) datas[0] : " << m_toolData.datas[0] << std::endl;
				std::cout << "tool data (word) datas[1] : " << m_toolData.datas[1] << std::endl;
				break;

			case RX_TYPE::RX_FIRM_INFO:
				m_firmInfo = *((firm_version_st *)m_packetData.data);
				std::cout << "m_firmInfo.firm_version  : " << std::string{(char *)m_firmInfo.firm_version} << std::endl;
				std::cout << "m_firmInfo.firm_name  : " << std::string{(char *)m_firmInfo.firm_name} << std::endl;
				break;

			case RX_TYPE::RX_BOOT_INFO:
				m_bootInfo = *((firm_version_st*)m_packetData.data);
				std::cout << "m_bootInfo.firm_version  : " << std::string{ (char*)m_bootInfo.firm_version } << std::endl;
				std::cout << "m_bootInfo.firm_name  : " << std::string{ (char*)m_bootInfo.firm_name } << std::endl;
				break;

			default:
				break;
			}
		}

		inline int UartCallback(void *w_parm, void *l_parm)
		{
			if (w_parm == nullptr)
				return -1;
			int length = *((int *)w_parm);
			int index = 0;

			for (index = 0; index < length; index++)
			{
				uint8_t data = *((uint8_t *)l_parm + index);

				{
					if (recievePacket(data))
					{
						m_waitResp = false;
						processCplt();
					}
				}
			}

			return 0;
		}

		inline bool recievePacket(uint8_t rx_data)
		{

			std::lock_guard<std::mutex> lock(m_mutex);

			// if ((UTL::millis() - m_packetData.state.GetPrevTime()) > 1000)
			if (m_packetData.state.MoreThan(1000))
			{
				m_packetData.BufferClear();
			}

			/*

			 | SOF  | rx_Type | obj_Id| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:--------|:----- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A | 1 byte  | 1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |

			*/

			const uint8_t CMD_STX = MODULE_XYIO_CMD_STX;
			const uint8_t CMD_ETX = MODULE_XYIO_CMD_ETX;

			enum : uint8_t
			{
				STATE_WAIT_STX,
				STATE_WAIT_TYPE,
				STATE_WAIT_ID,
				STATE_WAIT_LENGTH_L,
				STATE_WAIT_LENGTH_H,
				STATE_WAIT_DATA,
				STATE_WAIT_CHECKSUM_L,
				STATE_WAIT_CHECKSUM_H,
				STATE_WAIT_ETX
			};

			// std::cout << "received data - " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(rx_data) << std::endl;
			switch (m_packetData.state.GetStep())
			{
			case STATE_WAIT_STX:
				m_packetData.buffer_idx = 0;
				m_packetData.checksum = 0xffff;
				if (rx_data == CMD_STX)
				{
					m_packetData.buffer.fill(0);
					m_packetData.buffer[m_packetData.buffer_idx] = rx_data;
					m_packetData.buffer_idx++;
					m_packetData.state.SetStep(STATE_WAIT_TYPE);
				}
				break;

			case STATE_WAIT_TYPE:
				m_packetData.rx_type = rx_data;
				m_packetData.BufferAdd(rx_data);
				crc::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_ID);
				break;

			case STATE_WAIT_ID:
				m_packetData.obj_id = rx_data;
				m_packetData.BufferAdd(rx_data);
				crc::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_LENGTH_L);
				break;

			case STATE_WAIT_LENGTH_L:
				m_packetData.data_length = rx_data;
				m_packetData.BufferAdd(rx_data);
				crc::crc16_update(&m_packetData.checksum, rx_data);
				m_packetData.state.SetStep(STATE_WAIT_LENGTH_H);
				break;

			case STATE_WAIT_LENGTH_H:
				m_packetData.data_length |= (uint16_t)(rx_data << 8);
				m_packetData.BufferAdd(rx_data);
				crc::crc16_update(&m_packetData.checksum, rx_data);
				if (m_packetData.data_length != 0)
				{
					if (m_packetData.data_length <= MODULE_XYIO_MAX_DATA_LENGTH)
					{
						m_packetData.data_cnt = 0;
						m_packetData.state.SetStep(STATE_WAIT_DATA);
					}
					else
					{
						m_packetData.state.SetStep(STATE_WAIT_STX);
					}
				}
				else
				{
					m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_L);
				}
				break;

			case STATE_WAIT_DATA:
				if (m_packetData.data_cnt == 0)
					m_packetData.data = &m_packetData.buffer[m_packetData.buffer_idx];

				m_packetData.BufferAdd(rx_data);
				crc::crc16_update(&m_packetData.checksum, rx_data);
				// check receive completed
				m_packetData.data_cnt++;
				if (m_packetData.data_length == m_packetData.data_cnt)
				{
					m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_L);
				}
				break;

			case STATE_WAIT_CHECKSUM_L:
				m_packetData.BufferAdd(rx_data);
				m_packetData.checksum_recv = rx_data;
				m_packetData.state.SetStep(STATE_WAIT_CHECKSUM_H);
				break;

			case STATE_WAIT_CHECKSUM_H:
				m_packetData.BufferAdd(rx_data);
				m_packetData.checksum_recv |= (uint16_t)(rx_data << 8);
				m_packetData.state.SetStep(STATE_WAIT_ETX);
				break;

			case STATE_WAIT_ETX:
				m_packetData.BufferAdd(rx_data);
				m_packetData.state.SetStep(STATE_WAIT_STX);
				if (rx_data == CMD_ETX)
				{
					// std::cout << "checksum [" << static_cast<int>(m_packetData.checksum) << "] , checksum_recv [" << static_cast<int>(m_packetData.checksum_recv) << "]" << std::endl;
					if (m_packetData.checksum == m_packetData.checksum_recv)
					{
						return true;
					}
				}
				break;
			}
			// switch (m_packet.state)

			return false;
		}

		inline errno_t SendCmd(uint8_t *p_data, uint32_t length)
		{
			// std::lock_guard<std::mutex> lock(m_mutex);

			std::vector<uint8_t> datas{};
			/*

			 | SOF  | Type |funcId| Data Length |Data          |   Checksum   | EOF  |
			 | :--- |:-----|:---- | :---------- |:-------------|:-------------| :--  |
			 | 0x4A |1 byte|1 byte| 2 byte      |Data 0～Data n|2 byte(crc 16)| 0x4C |
							|-> crc                                            crc <- |
			*/
			uint16_t crc = 0xffff;
			datas.emplace_back(MODULE_XYIO_CMD_STX);
			for (uint32_t i = 0; i < length; i++)
			{
				datas.emplace_back(p_data[i]);
				crc::crc16_update(&crc, p_data[i]);
			}
			datas.emplace_back((uint8_t)(crc >> 0));
			datas.emplace_back((uint8_t)(crc >> 8));
			datas.emplace_back(MODULE_XYIO_CMD_ETX);

			if (m_cb)
			{
				packet_st send_data{};
				for (auto &elm : datas)
				{
					send_data.buffer[send_data.buffer_idx] = elm;
					send_data.buffer_idx++;
				}
				int size = (int)datas.size();
				m_cb(&size, (void *)&send_data);
			}
			std::cout << "TxData.Data: ";
			for (auto &i : datas)
			{
				std::cout << std::hex << (int)i << " ";
			}
			std::cout << std::endl;
			m_waitResp = true;
			if (m_cfg.ptr_uart_comm->SendData((char *)datas.data(), (uint32_t)datas.size()) > 0)
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline bool SendCmdRxResp(uint8_t *p_data, uint32_t length, uint32_t timeout = 100)
		{
			if (SendCmd(p_data, length) == ERROR_SUCCESS)
			{
				uint32_t pre_ms = tim::millis();
				while (m_waitResp)
				{
					if ((tim::millis() - pre_ms) > timeout)
					{
						ERR_PRINT("SendCmdRxResp timeout!");
						m_waitResp = false;
						return false;
					}

					// Sleep(50);
					tim::delay(1);
				}
				m_respTime = tim::millis() - pre_ms;
				return true;
			}
			return false;
		}

	public:
		inline errno_t Init(const cfg_t &cfg)
		{
			m_cfg = cfg;
			serial_comm::cfg_t comm_cfg{};
			m_cfg.ptr_uart_comm->registerCallback(std::bind(&ModuleXYIO::UartCallback, this, std::placeholders::_1, std::placeholders::_2));
			if (m_cfg.ptr_uart_comm->IsOpen())
			{
				return ERROR_SUCCESS;
			}
			else
				return m_cfg.ptr_uart_comm->OpenPort();
		}

		inline errno_t Ret_OkResponse()
		{
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_OK_RESPONSE);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);

			return SendCmd(datas.data(), (uint32_t)datas.size());
		}

		inline errno_t GetMcuState()
		{
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_MCU_DATA);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			return SendCmd(datas.data(), (uint32_t)datas.size());
			//return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}

		inline errno_t GetPnPToonState()
		{
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_TOOL_PnP_DATA);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			//return SendCmd(datas.data(), (uint32_t)datas.size());
			return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}

		inline errno_t GetFirmwareInfoData()
		{
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_READ_FIRM_INFO);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			return SendCmd(datas.data(), (uint32_t)datas.size());
			//return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}


		inline errno_t MotorOnOff(OBJ::MOTOR obj_id, bool on_off)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ONOFF);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x01);
			datas.emplace_back(0x00);
			// data
			datas.emplace_back((uint8_t)(on_off ? 1 : 0));

			return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}

		inline errno_t ClearState(OBJ::MOTOR obj_id)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_CLEAR_ALARM);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			// data

			return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}
		/////////////////
		inline errno_t SetPnPToolOrder(tool_PnP_order_e order, uint8_t obj_id = 0 ,int data_1 = 0, int data_2 = 0)
		{
			std::vector<uint8_t> datas{};
			enum 
			{
				ord1,
				ord2,
				data11,
				data12,
				data13,
				data14,
				data21,
				data22,
				data23,
				data24,
				_max
			};
			datas.emplace_back(CMD_CTRL_TOOL_PnP);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(LoBYTE(_max));
			datas.emplace_back(HiBYTE(_max));
			// data
			datas.emplace_back(LoBYTE(order));
			datas.emplace_back(HiBYTE(order));
			datas.emplace_back(uint8_t(data_1 >> (8 * 0)));
			datas.emplace_back(uint8_t(data_1 >> (8 * 1)));
			datas.emplace_back(uint8_t(data_1 >> (8 * 2)));
			datas.emplace_back(uint8_t(data_1 >> (8 * 3)));
			datas.emplace_back(uint8_t(data_2 >> (8 * 0)));
			datas.emplace_back(uint8_t(data_2 >> (8 * 1)));
			datas.emplace_back(uint8_t(data_2 >> (8 * 2)));
			datas.emplace_back(uint8_t(data_2 >> (8 * 3)));

			return (SendCmdRxResp(datas.data(), (uint32_t)datas.size()) ? ERROR_SUCCESS : -1);
		}

		/////////////////

		inline errno_t Move(OBJ::MOTOR obj_id, int cmd_dist, uint16_t cmd_rpm = 1000, uint16_t acc = 100)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_MOVE_VEL);
			datas.emplace_back((uint8_t)obj_id);
			enum
			{
				pos1,
				pos2,
				pos3,
				pos4,
				vel1,
				vel2,
				accdec1,
				accdec2,
				_max
			};
			// length
			datas.emplace_back(uint8_t(_max >> 0));
			datas.emplace_back(uint8_t(_max >> 8));
			// data
			datas.emplace_back(uint8_t(cmd_dist >> 0));
			datas.emplace_back(uint8_t(cmd_dist >> 8));
			datas.emplace_back(uint8_t(cmd_dist >> 16));
			datas.emplace_back(uint8_t(cmd_dist >> 24));

			datas.emplace_back(uint8_t(cmd_rpm >> 0));
			datas.emplace_back(uint8_t(cmd_rpm >> 8));

			datas.emplace_back(uint8_t(acc >> 0));
			datas.emplace_back(uint8_t(acc >> 8));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t RelMove(OBJ::MOTOR obj_id, int cmd_dist, uint16_t cmd_rpm = 100, uint16_t acc = 100)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_RELMOVE);
			datas.emplace_back((uint8_t)obj_id);
			enum
			{
				pos1,
				pos2,
				pos3,
				pos4,
				vel1,
				vel2,
				accdec1,
				accdec2,
				_max
			};
			// length
			datas.emplace_back(uint8_t(_max >> 0));
			datas.emplace_back(uint8_t(_max >> 8));
			// data
			datas.emplace_back(uint8_t(cmd_dist >> 0));
			datas.emplace_back(uint8_t(cmd_dist >> 8));
			datas.emplace_back(uint8_t(cmd_dist >> 16));
			datas.emplace_back(uint8_t(cmd_dist >> 24));

			datas.emplace_back(uint8_t(cmd_rpm >> 0));
			datas.emplace_back(uint8_t(cmd_rpm >> 8));

			datas.emplace_back(uint8_t(acc >> 0));
			datas.emplace_back(uint8_t(acc >> 8));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t Stop(OBJ::MOTOR obj_id)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_STOP);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			// data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t JogMove(OBJ::MOTOR obj_id, uint16_t speed, uint16_t acc_dec, int dir)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_VEL_JOG);
			// obj_id
			datas.emplace_back((uint8_t)obj_id);
			enum
			{
				vel1,
				vel2,
				acc1,
				acc2,
				dec1,
				dec2,
				dir1,
				dir2,
				dir3,
				dir4,
				_max
			};

			// length
			datas.emplace_back(_max >> 0);
			datas.emplace_back(_max >> 8);
			// data
			datas.emplace_back(uint8_t(speed >> 0));
			datas.emplace_back(uint8_t(speed >> 8));
			datas.emplace_back(uint8_t(acc_dec >> 0));
			datas.emplace_back(uint8_t(acc_dec >> 8));
			datas.emplace_back(uint8_t(acc_dec >> 0));
			datas.emplace_back(uint8_t(acc_dec >> 8));

			datas.emplace_back(uint8_t(dir >> 0));
			datas.emplace_back(uint8_t(dir >> 8));
			datas.emplace_back(uint8_t(dir >> 16));
			datas.emplace_back(uint8_t(dir >> 24));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t JogMove(OBJ::MOTOR obj_id, bool is_cw = true)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_JOG);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x01);
			datas.emplace_back(0x00);
			// data
			datas.emplace_back((uint8_t)(is_cw ? 1 : 0));

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t ZereSetMotorEncoder(OBJ::MOTOR obj_id)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ZEROSET);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			// data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t MotorOrigin(OBJ::MOTOR obj_id)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_ORIGIN);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			// data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}

		inline errno_t MoveToLimit_CCW(OBJ::MOTOR obj_id)
		{
			if (!(obj_id < OBJ::MOTOR::MOTOR_MAX)) // not
				return -1;
			std::vector<uint8_t> datas{};
			datas.emplace_back(CMD_CTRL_MOT_LIMIT);
			datas.emplace_back((uint8_t)obj_id);
			// length
			datas.emplace_back(0x00);
			datas.emplace_back(0x00);
			// data

			if (SendCmdRxResp(datas.data(), (uint32_t)datas.size()))
				return ERROR_SUCCESS;
			else
				return -1;
		}
		//

#if 0

#endif

		///////////////////////////////////////////////////////////////////////////////////////
		inline int testCB_func(int argc, char *argv[])
		{
			auto get_data = [](char *int_ptr) -> int
			{
				int ret = 0;
				ret = (int)strtoul((const char *)int_ptr, (char **)NULL, (int)0);
				return ret;
			};

			bool ret{};
			enum : int
			{
				arg_cnt_0,
				arg_cnt_1,
				arg_cnt_2,
				arg_cnt_3,
				arg_cnt_4,
				arg_cnt_5,
				arg_cnt_6,
			};
			switch ((argc - 1))
			{
			case arg_cnt_1:
			{
				if (argv[1])
				{
					if (std::string(argv[1]).compare("info") == 0)
					{
						if (this->GetFirmwareInfoData() == ERROR_SUCCESS)
						{
							std::cout << "xyio state [" << this << "] success " << std::endl;
							ret = true;
						}
						else
							std::cout << "xyio get state fail" << std::endl;
					}					
					else if (std::string(argv[1]).compare("state") == 0)
					{
						while (TinyC::Cli::cliKeepLoop())
						{
							if (this->GetMcuState() == ERROR_SUCCESS)
							{
								std::cout << "xyio state [" << this << "] success " << std::endl;
								ret = true;
							}
							else
								std::cout << "xyio get state fail" << std::endl;

							tim::delay(50);
						}
					}
				}
			}
			break;
			case arg_cnt_2:
			{
				if (argv[1])
				{
					if (std::string(argv[1]).compare("motor_strop") == 0)
					{
					}
				}
			}
			break;
			case arg_cnt_3:
			{
				if (argv[1])
				{
					if (std::string(argv[1]).compare("motor_onoff") == 0)
					{
						uint8_t id = get_data(argv[2]);
						bool val = (get_data(argv[3]) == 0) ? false : true;
						OBJ::MOTOR motor = (OBJ::MOTOR)id;
						if (MotorOnOff(motor, val) == ERROR_SUCCESS)
						{
							std::cout << "MotorOnOff success" << std::endl;
							ret = true;
						}
						else
						{
							std::cout << "MotorOnOff fail" << std::endl;
						}
					}
					else if (std::string(argv[1]).compare("tool") == 0)
					{
						if (std::string(argv[2]).compare("pnp") == 0)
						{
							if (std::string(argv[3]).compare("info") == 0)
							{
								if (this->GetPnPToonState() == ERROR_SUCCESS)
								{
									std::cout << "xyio GetPnPToonState  success " << std::endl;
									ret = true;
								}
								else
									std::cout << "xyio GetPnPToonState fail" << std::endl;
							}
							else if (std::string(argv[3]).compare("org") == 0)
							{
								if (this->SetPnPToolOrder(TOOL_CMD_ORD_MOTOR_ORG,1) == ERROR_SUCCESS)
								{
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_ORG  success " << std::endl;
									ret = true;
								}
								else
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_ORG fail" << std::endl;
							}
							else if (std::string(argv[3]).compare("motor_on") == 0)
							{
								if (this->SetPnPToolOrder(TOOL_CMD_ORD_MOTOR_ENABLE,1) == ERROR_SUCCESS)
								{
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_ENABLE  success " << std::endl;
									ret = true;
								}
								else
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_ENABLE fail" << std::endl;
							}
							else if (std::string(argv[3]).compare("motor_off") == 0)
							{
								if (this->SetPnPToolOrder(TOOL_CMD_ORD_MOTOR_DISABLE,1) == ERROR_SUCCESS)
								{
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_DISABLE  success " << std::endl;
									ret = true;
								}
								else
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_DISABLE fail" << std::endl;
							}

						}

					}

				}
			}
			break;
			case arg_cnt_4:
			{
			}
			break;
			case arg_cnt_5:
			{
				if (argv[1])
				{
					if (std::string(argv[1]).compare("tool") == 0)
					{
						if (std::string(argv[2]).compare("pnp") == 0)
						{
							if (std::string(argv[3]).compare("run") == 0)
							{
								uint8_t id = get_data(argv[4]);
								int   cnt = get_data(argv[5]);
								if (this->SetPnPToolOrder(TOOL_CMD_ORD_MOTOR_RUN, 1, cnt) == ERROR_SUCCESS)
								{
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_RUN  success " << std::endl;
									ret = true;
								}
								else
									std::cout << "xyio TOOL_CMD_ORD_MOTOR_RUN fail" << std::endl;
							}						

						}

					}

				}
			}
			break;
			default:
				break;
			}
			// end of switch
			



			if (ret)
				return 0;

			std::cout << "xyio info" << std::endl; // firware and boot information
			std::cout << "xyio state" << std::endl;
			std::cout << "xyio motor_strop [idx 0:2]" << std::endl;
			std::cout << "xyio motor_onoff [idx 0:2] [on(1):off(0)]" << std::endl;
			std::cout << "xyio motor_run [idx 0:2] [dist] " << std::endl;

			std::cout << "xyio tool pnp info" << std::endl;
			std::cout << "xyio tool pnp org" << std::endl;
			std::cout << "xyio tool pnp motor_on" << std::endl;
			std::cout << "xyio tool pnp motor_off" << std::endl;
			std::cout << "xyio tool pnp run idx[0:3] cnt" << std::endl;

			return -1;
		}
		// testCB_func
	};
	// end of class ModuleXYIO

}

#endif //_FM_PXY_HPP