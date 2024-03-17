/*****************************************************************//**
 * \file   ap.cpp
 * \brief
 *
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/

 //https://welcomec.tistory.com/entry/visual-studio-2022-%EA%B8%B0%EB%B3%B8-%EB%8B%A8%EC%B6%95%ED%82%A4-%ED%8A%B9%ED%9E%88-%EC%BD%94%EB%93%9C-%EC%9E%90%EB%8F%99%EC%A0%95%EB%A0%AC
	//visual studio 2022 기본 단축키 특히 코드 자동정렬

	// if this is mfc , then include the precompiled header
#include "pch.h"

#include "ap.hpp"


/**
 * @brief
 * It must be written in non-block code as a function called in 1ms time.
 * @param arg
 */
static void apISR_1ms(void* arg1, void* arg2);
static void apISR_50ms(void* arg1, void* arg2);

static void de_init();
static void ap_thread_run();
static void ap_thread_stop();

/****************************************************
 *	생성 객체들
 ****************************************************/
tim::timer period_timer_1ms;
tim::timer period_timer_50ms;

using uarts_t = std::array<std::shared_ptr<ap_core::uart_t>, ap_core::INST_IDX::_SERIAL::uart_max>;
//std::shared_ptr<ap_core::uart_t> uart{};
uarts_t uarts{};


std::shared_ptr<ap_core::uart_lcd_t> nextion_comm{};

using lordcells_t = std::array<std::shared_ptr<ap_core::uart_hx711_t>, ap_core::INST_IDX::_LORDCELL::loadcell_max>;
lordcells_t lordcells{};

/*
* 
struct test_t
	{
	int a;
	int b;
	test_t(int a, int b) : a(a), b(b) {}
	
};


using tests_t = std::array<std::shared_ptr<test_t>, 5>;
tests_t tests{};
int main()
{
	test_t* ptr_test{};
	ptr_test = tests[0].get();
	ptr_test = std::make_shared<uart_t>(ret);

	//ap_core::main();
	//ap_core::exit();
	return 0;
}

*/

errno_t  ap_core::init(void)
{
	LOG_OUT("init");

	/* test */
	{
		uint32_t id = HW_SERIAL_BASE + 1;
		//using test_t = ap_core::ObjDataDef_t<ap_core::ObjType>;
		//using test_t = attribute_st;
		using test_t = ap_core::ObjDataDef_t<ap_core::ObjType>*;

		test_t ret = get_object_data<test_t>(id);

		using conf_jt = ap_core::ConfigData;
		constexpr const char* def_filename = "./_data/config.json";
		conf_jt config_data{};
		if (config_data.LoadJsonFile(def_filename))
		{
			if (config_data.m_json.m_objects.size() != static_cast<size_t>(ConfigData::data_reg_idx::_max))
			{
				std::cout << "json data is unmatching index with json_file" << std::endl;
			}


			int serial_no = config_data.GetData<int>(conf_jt::data_reg_idx::serial_no);
			std::cout << " machine serial no : [" << serial_no << "]" << std::endl;

			serial_no = 54321;
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::serial_no, TinyC::json_t{ serial_no }));

			std::string name = config_data.GetData<std::string>(conf_jt::data_reg_idx::machine);
			std::cout << " machine name  : [" << name << "]" << std::endl;
			name = "changed_name";
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::machine, TinyC::json_t{ name }));

			bool is_approve = config_data.GetData<bool>(conf_jt::data_reg_idx::approve);
			std::cout << " machine approve  : [" << (is_approve ? "true" : "false") << "]" << std::endl;
			is_approve = false;
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::approve, TinyC::json_t{ is_approve }));

			double digit_no = config_data.GetData<double>(conf_jt::data_reg_idx::digit);
			std::cout << " machine digit_no  : [" << std::to_string(digit_no) << "]" << std::endl;
			digit_no = 8.4356;
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::digit, TinyC::json_t{ digit_no }));


			std::vector<int> nums = config_data.GetData<std::vector<int>>(conf_jt::data_reg_idx::numbers);
			std::cout << " machine numbers  : [";
			for (const auto& elm : nums)
			{
				std::cout << elm << ", ";
			}
			std::cout << "]" << std::endl;
			nums[1] = 10;

			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::numbers, TinyC::json_t{ nums }));

			std::vector<bool> btns = config_data.GetData<std::vector<bool>>(conf_jt::data_reg_idx::checks);
			std::cout << " machine checkts  : [";
			for (const auto& elm : btns)
			{
				std::cout << (elm ? "true" : "false") << ", ";
			}
			std::cout << "]" << std::endl;

			btns[0] = true;
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::checks, TinyC::json_t{ btns }));


			std::vector<std::string> names = config_data.GetData<std::vector<std::string>>(conf_jt::data_reg_idx::members);
			std::cout << " machine members  : [";
			for (const auto& elm : names)
			{
				std::cout << elm << ", ";
			}
			std::cout << "]" << std::endl;
			names[0] = "LEE";
			config_data.m_json.replace_value(config_data.make_json_obj(conf_jt::data_reg_idx::members, TinyC::json_t{ names }));





			using ret_obj = TinyC::json_obj_t;
			ret_obj ret1 = config_data.GetData<ret_obj>(conf_jt::data_reg_idx::common);

			for (const auto& [key, value] : ret1)
			{
				std::cout << " elm 1 : key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
				// auto& data = std::get<TinyC::json_t>(value);
			}

			//config_data.m_json.replace_value(ret1);


			ret_obj ret2 = config_data.GetData<ret_obj>(conf_jt::data_reg_idx::operation);
			std::stringstream ss;
			for (const auto& [key, value] : ret2)
			{
				std::cout << " elm 2 : key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
				// auto& data = std::get<TinyC::json_t>(value);
			}

			//using ret_obj_1 = TinyC::json_obj_t;
			ret_obj ret3 = config_data.GetData<ret_obj>(conf_jt::data_reg_idx::serial_comm);

			for (const auto& [key, value] : ret3)
			{
				std::cout << " elm 3 : key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
				// auto& data = std::get<TinyC::json_t>(value);
			}

			ret_obj ret4 = config_data.GetData<ret_obj>(conf_jt::data_reg_idx::socket_net);

			for (const auto& [key, value] : ret4)
			{
				std::cout << " elm 4 : \n key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
				// auto& data = std::get<TinyC::json_t>(value);
			}

			ret_obj ret5 = config_data.GetData<ret_obj>(conf_jt::data_reg_idx::configs);

			for (const auto& [key, value] : ret5)
			{
				std::cout << " elm 5 : \n key [" << key << "] value [" << value.toJsonString() << "]" << std::endl;
				// auto& data = std::get<TinyC::json_t>(value);
			}

			std::cout << " all data print : \n" << config_data.toJsonString() << std::endl;

			if (config_data.SaveToFile() == false)
				std::cout << "fail save file" << std::endl;
			else
				std::cout << "success save file" << std::endl;

		}


	}
	
	{
		uint32_t id = HW_SERIAL_BASE + INST_IDX::_SERIAL::uart_com13;
		using data_t = attribute_st;
		data_t ret = get_object_data<data_t>(id);
		uarts[INST_IDX::_SERIAL::uart_com13] = std::make_shared<ap_core::uart_t>(ret);
		if (ap_core::regist_object(id, (IObj*)uarts[INST_IDX::_SERIAL::uart_com13].get()) == ERROR_SUCCESS)
		{
			std::cout << "RegisterComponent Result Success!(HL_SERIAL_BASE, uart13)" << std::endl;
		}
		else
		{
			std::cout << "RegisterComponent Result Fail!(HL_SERIAL_BASE, uart13)" << std::endl;
		}

	  id = HW_SERIAL_BASE + INST_IDX::_SERIAL::uart_com5;
		ret = get_object_data<data_t>(id);
		uarts[INST_IDX::_SERIAL::uart_com5] = std::make_shared<ap_core::uart_t>(ret);
		if (ap_core::regist_object(id, (IObj*)uarts[INST_IDX::_SERIAL::uart_com5].get()) == ERROR_SUCCESS)
		{
			std::cout << "RegisterComponent Result Success!(HL_SERIAL_BASE, uart5)" << std::endl;
		}
		else
		{
			std::cout << "RegisterComponent Result Fail!(HL_SERIAL_BASE, uart5)" << std::endl;
		}
	}


	{
		uint32_t id = EN_LCD_BASE + INST_IDX::_LCD::nextion;
		using data_t = attribute_st;
		data_t ret = get_object_data<data_t>(id);
		nextion_comm = std::make_shared<uart_lcd_t>(ret);

		if (ap_core::regist_object(id, (IObj*)nextion_comm.get()) == ERROR_SUCCESS)
		{
			std::cout << "RegisterComponent Result Success!(EN_LCD_BASE, nextion_comm)" << std::endl;

			uart_nextion<uint8_t>::cfg_t cfg{};
			cfg.port_name = "COM13";
			cfg.baudrate = 115200;
			cfg.ptr_uart = uarts[INST_IDX::_SERIAL::uart_com13].get();

			if (nextion_comm->Init(cfg) != ERROR_SUCCESS)
			{
				ERR_PRINT("nextion_comm->Init(cfg) Fail!");
				//return -1;
			}
			else
			{
				LOG_PRINT("nextion_comm->Init(cfg) Success!");
			}

		}
		else
		{
			std::cout << "RegisterComponent Result Fail!(EN_LCD_BASE, nextion_comm)" << std::endl;
		}

	}
	{
		uint32_t id = EN_LCD_BASE + INST_IDX::_LORDCELL::hx711_0;
		using data_t = attribute_st;
		data_t ret = get_object_data<data_t>(id);
		lordcells[INST_IDX::_LORDCELL::hx711_0] = std::make_shared<ap_core::uart_hx711_t>(ret);

		if (ap_core::regist_object(id, (IObj*)lordcells[INST_IDX::_LORDCELL::hx711_0].get()) == ERROR_SUCCESS)
		{
			std::cout << "RegisterComponent Result Success!(EN_LORDCELL_BASE, loadcell 0)" << std::endl;

			uart_hx711<uint8_t>::cfg_t cfg{};
			cfg.port_name = "COM5";
			cfg.baudrate = 115200;
			cfg.ptr_uart = uarts[INST_IDX::_SERIAL::uart_com5].get();

			if (lordcells[INST_IDX::_LORDCELL::hx711_0]->Init(cfg) != ERROR_SUCCESS)
			{
				ERR_PRINT("loadcell->Init(cfg) Fail!");
				//return -1;
			}
			else
			{
				LOG_PRINT("loadcell->Init(cfg) Success!");
			}

		}
		else
		{
			std::cout << "RegisterComponent Result Fail!(EN_LORDCELL_BASE, loadcell)" << std::endl;
		}

	}


#ifdef _USE_HW_CLI

	// test cli
	TinyC::Cli::registerCallback("uart_1", std::bind(&uart_t::testCB_func, uarts[0].get(), std::placeholders::_1, std::placeholders::_2));
	TinyC::Cli::registerCallback("uart_2", std::bind(&uart_t::testCB_func, uarts[1].get(), std::placeholders::_1, std::placeholders::_2));
	TinyC::Cli::registerCallback("lcd", std::bind(&uart_lcd_t::testCB_func, nextion_comm.get(), std::placeholders::_1, std::placeholders::_2));
	TinyC::Cli::registerCallback("hx711", std::bind(&uart_hx711_t::testCB_func, lordcells[0].get(), std::placeholders::_1, std::placeholders::_2));

#endif


	return ERROR_SUCCESS;
}

void  ap_core::main(void)
{
	LOG_OUT("core thread run ");

	ap_thread_run();
}



void  ap_core::exit(void)
{
	LOG_OUT("exit, thread_stop");
	ap_thread_stop();
}

void de_init()
{
	uarts[ap_core::INST_IDX::_SERIAL::uart_com13]->DeInit();
	uarts[ap_core::INST_IDX::_SERIAL::uart_com5]->DeInit();
}

//errno_t ap_core::deinit(void)
//{
//	return errno_t();
//}

std::shared_ptr<ap_core::uart_nextion<uint8_t>> ap_core::get_lcd(void)
{
	//return std::shared_ptr<uart_nextion<uint8_t>>();
	return nextion_comm;
}

std::shared_ptr<ap_core::uart_hx711_t> ap_core::get_hx711(INST_IDX::_LORDCELL obj_idx)
{
	return lordcells[obj_idx];
}


void apISR_1ms(void* arg1, void* arg2)
{
	/*
	주기성 작업을 수행하는 함수

	*/
}

void apISR_50ms(void* arg1, void* arg2)
{
	/*
주기성 작업을 수행하는 함수

*/
}

void ap_thread_run()
{

#ifdef _USE_HW_CLI
	TinyC::Cli::threadRun();
#endif

}

void ap_thread_stop()
{
	de_init();


#ifdef _USE_HW_CLI
	TinyC::Cli::threadStop();
#endif

}


