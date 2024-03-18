/*****************************************************************//**
 * \file   sys_def.hpp
 * \brief  
 * 
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/

#pragma once
#ifndef _CORE_SYS_SYS_DEF_HPP
#define _CORE_SYS_SYS_DEF_HPP

#include "core/common/def.hpp"
#include "common/util/utils.hpp"

namespace ap_core
{

	/*
6�ڸ��� �����Ǹ�  object type [xxx] + ���ǵ� error no [xxx]
*/
	typedef int errcode;

	//9�ڸ��� �����Ǹ� ������ �ڵ�
	//object type[xxx] + error no[xxx] + instance_no [xxx] 
	typedef int obj_errno;


	////////////////////////////////////////////////////////////////////////////////////////

	/**
	 *  application peripheral base address.
	 */
	constexpr auto APP_PERIPH_BASE = 0x00000000UL;// ���� �� 0x00100000UL~;


	/*!< peripherals> */


	//Hardware Layer 
	constexpr auto HW_IO_BASE = (APP_PERIPH_BASE + 0x00000010UL); // max 240 object
	constexpr auto HW_SERIAL_BASE = (APP_PERIPH_BASE + 0x00000100UL); // max 32 object
	constexpr auto HW_SOCKET_BASE = (APP_PERIPH_BASE + 0x00000120UL); // max 16 object
	constexpr auto HW_MOTION_LIB_BASE = (APP_PERIPH_BASE + 0x00000130UL);

	//Engine Layer
	constexpr auto EN_CYLINDER_BASE = (APP_PERIPH_BASE + 0x00000200UL); // max 512 object
	constexpr auto EN_VACUUM_BASE = (APP_PERIPH_BASE + 0x00000400UL); // max 512 object
	constexpr auto EN_MOTOR_BASE = (APP_PERIPH_BASE + 0x00000600UL);// max 256 object
	constexpr auto EN_LCD_BASE = (APP_PERIPH_BASE + 0x00000700UL);
	constexpr auto EN_LORDCELL_BASE = (APP_PERIPH_BASE + 0x00000710UL);

	//Control Layer 
	constexpr auto CN_OPPANEL_BASE = (APP_PERIPH_BASE + 0x00001000UL);
	constexpr auto CN_AUTOMANAGER_BASE = (APP_PERIPH_BASE + 0x00001010UL);

	//Application Programming Interface Layer 
	constexpr auto API_MODULE_XX_BASE = (APP_PERIPH_BASE + 0x00002000UL);

	////////////////////////////////////////////////////////////////////////////////////////

	enum class ObjType
	{
		NONE,

		// Hardware Layer
		HL_IO = 100,
		HL_SERIAL,
		HL_ETHERNET,

		// Engine Layer    
		EN_CYLINDER = 200,
		EN_VACUUM,
		EN_MOTOR,
		EN_LCD,
		EN_LORDCELL,

		// Control Layer 
		CN_OP_PANEL = 300,
		CN_AUTO_MANAGER,

		// API Layer
		API_XX = 400,

	};



	struct attribute_st
	{
		uint32_t        object_no{};
		uint32_t		instance_no{};
		uint32_t		logKeeping_days{};
		std::string	    obj_name{};
		std::string	    logFile_name{};

		attribute_st() = default;
		attribute_st(uint32_t objno, uint32_t ins_no, uint32_t keepday, std::string obj_str, std::string log_file)
		{
			this->object_no = objno;
			this->instance_no = ins_no;
			this->logKeeping_days = keepday;
			this->obj_name = obj_str;
			this->logFile_name = log_file;
		}

		~attribute_st() = default;
		// copy constructor
		attribute_st(const attribute_st& rhs) = default;
		// copy assignment operator
		attribute_st& operator=(const attribute_st& rhs) = default;
		// move constructor
		attribute_st(attribute_st&& rhs) = default;
		// move assignment operator
		attribute_st& operator=(attribute_st&& rhs) = default;



	};
	// end of struct attribute_st

	struct IObj
	{
		using log_t = TinyC::Log;
		/** �Ӽ� ������*/
		attribute_st attribute{};

		/** Log */
		std::unique_ptr<log_t> log{};

		IObj(const attribute_st& f) : attribute{ f }, log{ std::make_unique<log_t>() } {}
		IObj() = default;
		~IObj() = default;
		// copy assignment operator
		IObj& operator=(const IObj& rhs)
		{
			if (this != &rhs)
			{
				attribute = rhs.attribute;
				log = std::make_unique<log_t>();
			}
			return *this;
		}
		// move constructor
		IObj(IObj&& rhs) = default;
		// move assignment operator
		IObj& operator=(IObj&& rhs) noexcept
		{
			attribute = rhs.attribute;
			log = std::move(rhs.log);

			return *this;
		}


		inline bool writeLog(TinyC::Log::level level, const char* func,
			const char* file,
			const int line,
			const char* fmt, ...) const
		{		
			bool ret{};
			va_list args;
			va_start(args, fmt);
			std::string message{};
			message.reserve(1024); // ���� ũ�⸦ ����
			// ������ ũ�⸦ �������� ����
			int size = vsnprintf(nullptr, 0, fmt, args);
			message.resize(size);
			// ���۸� �����ϰ� vsnprintf �Լ��� ����Ͽ� ���ڿ��� �ۼ�
			vsnprintf(&message[0], size + 1, fmt, args);
			va_end(args);

			int obj_no = attribute.object_no + attribute.instance_no;
			 ret = log->WriteLog(level, obj_no, attribute.logFile_name.c_str()
				,  file, func, line, message);
			 ret;
			return ret;
		}

		/// <summary>
		 /// objectID + �����ڵ� �����Ͽ� ��ȯ
		 /// </summary>
		 /// <param name="err_code">object type [xxx] + ���ǵ� error no [xxx]</param>
		 /// <returns>err_code[xxxxxx] + instance_no [xxx]</returns>
		inline obj_errno generateErrorCode(errcode err_code) const
		{
			obj_errno	obj_type_errno = ERROR_SUCCESS;
			/** err_code�� SUCCESS�� �ƴϸ� �ڵ� ���� */
			if ((errno_t)err_code != 0)
			{
				obj_type_errno = (err_code * 1'000) + attribute.instance_no;
			}

			return obj_type_errno;
		}

	};

	template <typename P>
	concept obj_c = std::same_as<P, ObjType>;


	template <typename DataType>
		requires obj_c<DataType>
	struct ObjDataDef_t
	{
		DataType            dataType{};
		uint32_t            baseAddr{};
		uint32_t            instanceNo{};
		std::string	        objName{};
		std::string	        logFileName{};
		IObj* ptrObj{};
		uint32_t            logKeepingDays{};
		bool                is_Reg{};

		ObjDataDef_t() = default;
		ObjDataDef_t(DataType t) : dataType{ t }, baseAddr{}, instanceNo{}, objName{}, logFileName{}, ptrObj{}, logKeepingDays{}, is_Reg{ false } {};
		ObjDataDef_t(DataType t, uint32_t addr, uint32_t i_no, const char* name, const char* l_name) :
			dataType{ t }, baseAddr{ addr }, instanceNo{ i_no }, objName{ name }, logFileName{ l_name }, ptrObj{}, logKeepingDays{}, is_Reg{ false } {};
		~ObjDataDef_t() = default;

	};
	// end of Obj_st
	template <typename DataType>
	ObjDataDef_t(DataType t) -> ObjDataDef_t<DataType>;



	struct log_info_t
	{
		std::string file{};
		std::string func_name{};
		int line_no{};

		//default constructor
		log_info_t(const char* f, const char* n, int line) :file{ f }, func_name{ n }, line_no{ line } {}
		log_info_t() = default;
		//copy constructor
		log_info_t(const log_info_t& rhs) = default;
		//copy assignment operator
		log_info_t& operator=(const log_info_t& rhs) = default;
		//move constructor
		log_info_t(log_info_t&& rhs) = default;
		//move assignment operator
		log_info_t& operator=(log_info_t&& rhs) = default;

		log_info_t& operator()(const char* f, const char* n, int line)
		{
			this->file = f;
			this->func_name = n;
			this->line_no = line;
			return *this;
		}

	};

	struct IComm : virtual  IObj
	{
		//virtual int Init() = 0;
		virtual bool write_log(TinyC::Log::level level, log_info_t& log, const char* fmt, ...) = 0;
	};

}
// namespace ap_core



#endif //_CORE_SYS_SYS_DEF_HPP
