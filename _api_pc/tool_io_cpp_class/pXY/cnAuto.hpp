


namespace control
{
	struct cnAutoManager 
	{
		using op_t = engine::enOp;
		struct cfg_t
		{
			op_t* ptr_op;

			cfg_t() = default;
			~cfg_t() = default;
			// copy constructor
			cfg_t(const cfg_t& f) = default;
			// move constructor
			cfg_t(cfg_t&& f) = default;
			// copy assignment operator
			cfg_t& operator=(const cfg_t& f) = default;
			// move assignment operator
			cfg_t& operator=(cfg_t&& f) = default;
		};

		cfg_t m_cfg;
		bool m_isReadyAll;
		std::thread m_trd;
		std::atomic<bool> m_stopThread;
		std::function<int(void*, void*)> m_cb;
		std::mutex m_mutex;
		bool m_isRunTrd;
		bool m_isInit;

	public:
		cnAutoManager() : m_cfg{}, m_isReadyAll{}, m_trd{}, m_stopThread{}, m_cb{}, m_mutex{}, m_isRunTrd{}, m_isInit{} {}
		virtual ~cnAutoManager()
		{
			threadStop();
		}
		int Init(cfg_t& cfg)
		{
			m_cfg = cfg;
			return 0;
		}

		op_t::mode_e GetOPMode()
		{
			return op_t::mode_e{};
		}

		op_t::status_e GetOPStatus()
		{
			return op_t::status_e{};
		}

		void SetOPMode(op_t::mode_e mode)
		{
		}

		void SetOPStatus(op_t::status_e status)
		{
		}

		bool IsDetectAreaSensor()
		{
			return false;
		}

		void PauseStop()
		{
		}

		void UiSw(op_t::lamp_e btn, bool enable = true)
		{
			using button_t = op_t::lamp_e;
			switch (btn)
			{
			case button_t::LAMP_START:
				/* code */
				break;
			case button_t::LAMP_STOP:
				/* code */
				break;
			case button_t::LAMP_RESET:
				/* code */
				break;

			default:
				break;
			}
		}
		errno_t checkStartRunCondition()
		{

			return -1;
		}

		int AutoReady()
		{
		}

		void AlarmAuto()
		{
		}

		void AlarmAuto(const char* file, const char* func, int line, const char* msg)
		{
		}

		bool ThreadRun()
		{
			m_trd = std::thread(&cnAutoManager::threadRun, this);
			return true;
		}

		bool TreadStop()
		{
			threadStop();
			return m_isRunTrd;
		}

		void threadStop()
		{
			if (!m_isRunTrd)
				return;
			m_stopThread = true;
			if (m_trd.joinable())
				m_trd.join();
		}

		void threadJob()
		{
		}

		void threadRun()
		{
			std::cout << "Thread Start!" << std::endl;
			m_isRunTrd = true;
			while (!m_stopThread)
			{
				threadJob();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			m_isRunTrd = false;
			std::cout << "Thread Terminatet!" << std::endl;
		}
	};
	// end of cnAutoManager
}