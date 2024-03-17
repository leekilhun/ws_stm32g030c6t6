#include "pch.h"
#include "core/ap/ap.hpp"

/*
cl /EHsc /std:c++20 main.cpp /Fe: m.exe
cl /EHsc /std:c++20 main.cpp .\core\sys\sys.cpp .\core\hw\hw.cpp .\core\common\util\utils.cpp .\core\ap\ap.cpp /Fe: m.exe /I .\ /I .\core\ /I .\core\ap\ /I .\core\sys\ /I .\core\hw\ /I .\core\common\ /I .\core\common\util\ /I .\mcc_lib /I .\core\common\_inc_hw\ 


*/

//bool  TinyC::sequence_terminal::is_enabled = false;
void intialize()
{
  	// 콘솔 창 생성
	//auto& vt = TinyC::sequence_terminal::get_instance();
	ap_core::sys_init();
	ap_core::hw_init();
	ap_core::init();
	ap_core::main();

}


int main()
{
    intialize();

    while(1)
    {
        
    }

    ap_core::exit();
   // TinyC::sequence_terminal::disable_sequence_terminal_processing();
    return 0;
}

