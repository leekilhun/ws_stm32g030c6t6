#include "../def.hpp"
#include "../cli/mcc_cli.hpp"
#include "../log/mcc_log.hpp"

/****************************************************
 *	cli
 ****************************************************/
TinyC::Cli ap_cli;

// initializer static memvers
bool TinyC::Cli::trd_life = true;
HANDLE TinyC::Cli::hConsole = nullptr;
std::string TinyC::Cli::c_input{};
std::map<std::string, std::function<int(int, char**)>> TinyC::Cli::callbacks{};

///////////////////////////////////////////////////////////////////

/****************************************************
 *	log  global
 ****************************************************/
mcc_log ap_log;

// initializer static memvers
uint32_t mcc_log::trace_prc_pre_tick = 0;
std::mutex mcc_log::log_mutex{};
TinyC::Que<mcc_log::dat_st> mcc_log::log_table[mcc_log::lvl_max] = { TinyC::Que<mcc_log::dat_st>(4096), TinyC::Que<mcc_log::dat_st>(4096), TinyC::Que<mcc_log::dat_st>(4096) };

#define SYSLOGFILE_INFO "./Log/system.log"
#define SYSLOGFILE_ERR "./Log/sysError.log"

#define __LOGFUNC__ __FUNCTION__
#define SYSLOG_INFO(obj, fmt, ...) mcc_log::PutLog(mcc_log::lvl_info, obj, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_PRINT(fmt, ...) mcc_log::WriteLog(mcc_log::lvl_info, 0, SYSLOGFILE_INFO, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)
#define ERR_PRINT(fmt, ...) mcc_log::WriteLog(mcc_log::lvl_err, 0, SYSLOGFILE_ERR, (__FILE__), __LOGFUNC__, __LINE__, fmt, ##__VA_ARGS__)

///////////////////////////////////////////////////////////////////

#include "../hw/serial_comm.hpp"
#include "../json/mcc_json.hpp"
#include "fm_pXY.hpp"


namespace fm //flash Memory
{
	constexpr uint8_t line_index_max = 10;
	using task_t = fmpXY::cnTasks<fmpXY::AxisSystemXYZ>;
	using pos_data_t = fmpXY::pos_data_st<fmpXY::AxisSystemXYZ>;
	using data_t = fmpXY::sequece_idx_data_st;
	using type_t = data_t::linetype_e;
	using idx_t = fmpXY::pos_data_st<>::idx_e;

	std::array<pos_data_t, 3> pose_datas =
	{
		{
			{{0,0,0},0,0,0},
			{{0,0,0},0,0,0},
			{{0,0,0},0,0,0}
		}
	};



	std::array<data_t, line_index_max> datas =
	{

		//  idx ,  next_line,  line_type, pos_data_idx,  opt_reg,  entry_setout,  exit_setout, entry_delay, exit_delay,  condition_in, condition_pass_line, condition_fail_line
		{
			{0, 0, {}/*type_t::lt_sequence*/, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{1, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{2, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{3, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{4, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{5, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{6, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{7, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{8, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0},
			{9, 0, {}, {}, {}, 0, 0, 0, 0, 0, 0, 0}
		}
	};


	std::array<fmpXY::taskDat::dat_t, line_index_max> task_datas =
	{
		{
			{0,datas[0]}, {1,datas[1]}, {2,datas[2]}, {3,datas[3]}, {4,datas[4]},
			{5,datas[5]}, {6,datas[6]}, {7,datas[7]}, {8,datas[8]}, {9,datas[9]}
		}
	
	};


}











fmpXY::cnTasks<fmpXY::AxisSystemXYZ>  line_task;
std::thread task_trd;
std::atomic<bool> task_trd_life;
void run_test_trd()
{	


	std::function<void(int)> f = [=](int a)
	{		
		std::cout << "Task started with a = " << a << std::endl;
		while (task_trd_life)
		{
			line_task.doRunStep();
		}
		std::cout << "Task Thread  terminate  " << std::endl;
	};

	task_trd = std::thread(f, 10);
	// 람다 함수를 사용하여 스레드 생성
}




int testCB_func(int argc, char* argv[])
{
	auto get_data = [](char* int_ptr) -> int
	{
		int ret = 0;
		ret = (int)strtoul((const char*)int_ptr, (char**)NULL, (int)0);
		return ret;
	};

	auto get_bool = [](char* int_ptr, std::string_view comp) -> bool
	{
		return (std::string(int_ptr).compare(comp) == 0);
	};

	bool ret{};
	enum : int
	{
		arg_cnt_0,
		arg_cnt_1,
		arg_cnt_2,
		arg_cnt_3,
	};
	switch ((argc - 1))
	{
	case arg_cnt_1:
	{
		if (argv[1])
		{
			if (std::string(argv[1]).compare("info") == 0)
			{
				ret = true;
			}	
			else if (std::string(argv[1]).compare("run") == 0)
			{
					using line_t = fmpXY::sequece_idx_data_st;
					using type_t = line_t::linetype_e;
					using idx_t = fmpXY::pos_data_st<>::idx_e;
					std::array<line_t, 30> line_datas = {};
					::task_trd_life = true;

					::run_test_trd();				
			}
		}
	}
	break;
	case arg_cnt_2:
	{
		if (argv[1])
		{

		}
	}
	break;
	case arg_cnt_3:
	{
		if (argv[1])
		{

		}
	}
	break;
	default:
		break;
	}
	// end of switch

	if (ret)
		return 0;

	std::cout << "app info" << std::endl;
	std::cout << "app run" << std::endl;

	return 0;
}

void test_func()
{

	std::array<uint8_t, 4> datas{0xdc, 0x05, 0x00, 0x00};
	int* ptr1 = reinterpret_cast<int*>(datas.data());

	int result = *ptr1;

	result += 5000;

	std::cout << result << std::endl;
}

std::shared_ptr<serial_comm> uart{};
std::shared_ptr<fmpXY::ModuleXYIO> xyio{};

int main()
{
	

	/* "cmd_str" "func_name" .... */
	ap_cli.registerCallback("help", ap_cli.cli_show_list);

#if 1
	// Create a serial communication object
	uart = std::make_shared<serial_comm>();

	serial_comm::cfg_t cfg{};
	cfg.port_name = "COM19";
	cfg.baudrate = 115200;
	if (uart->Init(cfg) != ERROR_SUCCESS)
	{
		ERR_PRINT("uart->Init(cfg) Fail!");
		task_trd_life = false;
		if (task_trd.joinable())
			task_trd.join();

		return 0;
	}
	else
	{
		LOG_PRINT("uart->Init(cfg) Success!");
	}

	// Create ModuleXYIO object

	xyio = std::make_shared<fmpXY::ModuleXYIO>();
	{
		fmpXY::ModuleXYIO::cfg_t cfg;
		cfg.ptr_uart_comm = uart.get();
		if (xyio->Init(cfg) != ERROR_SUCCESS)
		{
			ERR_PRINT("xyio->Init(cfg) Fail!");
			return 0;
		}
		else
		{
			LOG_PRINT("xyio->Init(cfg) Success!");
		}
	}
	// Initialize the peeler object

	// test cli
	ap_cli.registerCallback("uart", std::bind(&serial_comm::testCB_func, uart.get(), std::placeholders::_1, std::placeholders::_2));

	ap_cli.registerCallback("xyio", std::bind(&fmpXY::ModuleXYIO::testCB_func, xyio.get(), std::placeholders::_1, std::placeholders::_2));
#endif

	ap_cli.registerCallback("app", testCB_func);


	// cli loop
	std::cout << "start " << std::endl;
	while (1)
	{
		ap_cli.cli_main();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	std::cout << "terminate " << std::endl;
	::task_trd_life = false;
	if (::task_trd.joinable())
		::task_trd.join();
	return 0;
}




