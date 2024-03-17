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

static void de_init();


errno_t  ap_core::init(void)
{
	//LOG_OUT("init");

	return ERROR_SUCCESS;
}

void  ap_core::main(void)
{
	//LOG_OUT("core thread run ");
}



void  ap_core::exit(void)
{
	//LOG_OUT("exit, thread_stop");
}

void de_init()
{

}

//errno_t ap_core::deinit(void)
//{
//	return errno_t();
//}





