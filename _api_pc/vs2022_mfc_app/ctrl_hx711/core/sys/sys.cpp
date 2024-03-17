/*****************************************************************//**
 * \file   sys.cpp
 * \brief
 * ap system 구성에 필요한 객체 정의 및 정보를 생성한다.
 *
 * \author gns2.lee (gns2.lee@samsung.com)
 * \date   March 2024
 *********************************************************************/

 // if this is mfc , then include the precompiled header
#include "pch.h"

#include "sys.hpp"

using obj_t = ap_core::ObjDataDef_t<ap_core::ObjType>;
using info_t = ap_core::attribute_st;
static std::vector<obj_t> obj_datas{};

static obj_t* init_sys_information();




// 템플릿 함수 구현 (기본)
template <typename T>
T ap_core::get_object_data(uint32_t idx) {
	return T(); // 기본 생성자로 초기화된 인스턴스 반환
}

// obj_t 타입에 대한 특수화
template <>
obj_t ap_core::get_object_data<obj_t>(uint32_t idx)
{

	std::cout << "get_object_data obj_t : " << std::to_string(idx) << std::endl;
	for (auto& data : obj_datas)
	{
		if ((data.baseAddr + data.instanceNo) == idx)
			return data;
	}
	return obj_t{ ObjType::NONE };
}

// info_t 타입에 대한 특수화
template <>
info_t ap_core::get_object_data<info_t>(uint32_t idx)
{
	info_t obj_info{};

	std::cout << "get_object_data info_t : " << std::to_string(idx) << std::endl;
	for (auto& data : obj_datas)
	{
		if ((data.baseAddr + data.instanceNo) == idx)
		{
			obj_info.object_no = data.baseAddr;
			obj_info.instance_no = data.instanceNo;
			obj_info.logKeeping_days = data.logKeepingDays;
			obj_info.obj_name = data.objName;
			obj_info.logFile_name = data.logFileName;
			return obj_info;
		}
	}
	return obj_info;
}

// obj_t* 타입에 대한 특수화
template<>
obj_t* ap_core::get_object_data<obj_t*>(uint32_t idx)
{
	for (auto& data : obj_datas)
	{
		if ((data.baseAddr + data.instanceNo) == idx)
			return &data;
	}
	return (obj_t*)nullptr;
}

// 명시적 템플릿 인스턴스화
//template info_t ap_core::get_object_data<info_t>(uint32_t idx);

///////////////////////////////////////////////////////////////////////////////////////

bool ap_core::sys_init(void)
{
	std::cout << "sys_init" << std::endl;
	init_sys_information();
	return false;
}




errno_t ap_core::regist_object(uint32_t idx, IObj* ptr_obj)
{
	obj_t* p_obj = get_object_data<obj_t*>(idx);
	if (p_obj == nullptr)
		return -1;
	if (p_obj->dataType == ObjType::NONE)
		return -1;

	p_obj->ptrObj = ptr_obj;
	p_obj->is_Reg = true;
	return ERROR_SUCCESS;
}

obj_t* init_sys_information()
{
	using namespace ap_core;

	/* object information data 생성 방법 1*/
	std::vector<obj_t> objs{
		{ ObjType::HL_SERIAL, HW_SERIAL_BASE, INST_IDX::_SERIAL::uart_com13, "COM13", "uart/uart.log" }, // default path ./Log/
		{ ObjType::HL_SERIAL, HW_SERIAL_BASE, INST_IDX::_SERIAL::uart_com5, "COM5", "uart/uart.log" }, // default path ./Log/
		{ ObjType::EN_LCD, EN_LCD_BASE, INST_IDX::_LCD::nextion, "NEXTION", "lcd/lcd.log" },
		{ ObjType::EN_LORDCELL, EN_LORDCELL_BASE, INST_IDX::_LORDCELL::hx711_0, "HX711_LORDCELL", "lordcell/lordcell.log" }

	};

	obj_datas = objs;


	/* object information data 생성 방법 2*/
	//obj_datas.reserve(100);
	//obj_datas.emplace_back(ObjType::HL_SERIAL, HW_SERIAL_BASE, 1, "COM13", "uart/uart.log");
	//obj_datas.shrink_to_fit();


	return obj_datas.data();
}
