
namespace engine
{

struct enOp 
	{
		enum mode_e
		{
			READY,
			AUTORUN,
			STOP,
			DRY_RUN,
		};

		enum status_e
		{
			INIT,
			ERR_STOP,
			STEP_STOP,
			RUN_READY,
			RUN,
		};

		enum button_e
		{
			BTN_START,
			BTN_STOP,
			BTN_RESET,
			BTN_ESTOP,
			BTN_MAX
		};

		enum lamp_e
		{
			LAMP_START,
			LAMP_STOP,
			LAMP_RESET,
		};

		struct cfg_t
		{
			IIO* ptr_io{};

			uint32_t btn_addr_start{};
			uint32_t btn_addr_stop{};
			uint32_t btn_addr_reset{};
			uint32_t btn_addr_estop{};

			uint32_t lamp_addr_start{};
			uint32_t lamp_addr_stop{};
			uint32_t lamp_addr_reset{};

			cfg_t() = default;
			// copy constructor
			cfg_t(const cfg_t& rhs) = default;
			// copy assignment operator
			cfg_t& operator=(const cfg_t& rhs) = default;
			// move constructor
			cfg_t(cfg_t&& rhs) = default;
			// move assignment operator
			cfg_t& operator=(cfg_t&& rhs) = default;
		};

		enOp::status_e m_status;
		enOp::mode_e m_mode;
		enOp::cfg_t m_cfg;
		bool m_isInit;

	public:
		enOp(const attribute_t& f) : IObj(f), m_status{ enOp::status_e::INIT }, m_mode{ enOp::mode_e::STOP }, m_cfg{}, m_isInit{} {}
		virtual ~enOp()
		{
		}
		int Init(enOp::cfg_t& cfg)
		{
			m_cfg = cfg;
			return 0;
		}

		enOp::status_e GetStatus() const
		{
			return m_status;
		}

		void SetStatus(enOp::status_e state)
		{
			m_status = state;
		}

		enOp::mode_e GetMode() const
		{
			return m_mode;
		}

		void SetMode(enOp::mode_e md)
		{
			m_mode = md;
		}

		void UpdateState()
		{
			// panel io 상태를 업데이트 하는 함수. callback에 attach하거나 폴링을 한다.
		}

		bool GetPressed(enOp::button_e op_sw)
		{
			switch (op_sw)
			{
			case enOp::button_e::BTN_START:
				break;
			case enOp::button_e::BTN_STOP:
				break;
			case enOp::button_e::BTN_RESET:
				break;
			case enOp::button_e::BTN_ESTOP:
				break;
			default:
				return false;
			}
			return true;
		}

		void LampOnOff(lamp_e lamp, bool state = true)
		{
			switch (lamp)
			{
			case LAMP_START:
				// io set
				break;
			case LAMP_STOP:
				// io set
				break;
			case LAMP_RESET:
				// io set
				break;
			default:
				break;
			};
		}

		void LampToggle(lamp_e lamp)
		{
			switch (lamp)
			{
			case LAMP_START:
				// io set
				break;
			case LAMP_STOP:
				// io set
				break;
			case LAMP_RESET:
				// io set
				break;
			default:
				break;
			};
		}
	};

}
// end of engine namespace