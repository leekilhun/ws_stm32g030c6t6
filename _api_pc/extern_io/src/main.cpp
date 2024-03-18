#include "pch.h"
#include "core/ap/ap.hpp"

/*
cl /EHsc /std:c++20 main.cpp /Fe: m.exe
cl /EHsc /std:c++20 main.cpp .\core\sys\sys.cpp .\core\hw\hw.cpp .\core\common\util\utils.cpp .\core\ap\ap.cpp /Fe: m.exe /I .\ /I .\core\ /I .\core\ap\ /I .\core\sys\ /I .\core\hw\ /I .\core\common\ /I .\core\common\util\ /I .\mcc_lib /I .\core\common\_inc_hw\ 



 프로젝트
 └─ src/
     ├── core/
     │   ├── ap/
     │   │   ├── _inc/
     │   │   │   ├── uart_hx711.hpp
     │   │   │   
     │   │   ├── api/
     │   │   ├── cn/
     │   │   ├── en/
     │   │   ├── ap_def.hpp
     │   │   ├── ap.hpp
     │   │   ├── ap.cpp
     │   │   └── def_obj.hpp
     │   │ 
     │   ├── common/
     │   │   ├── _inc_hw/
     │   │   │   ├── uart.hpp
     │   │   │   
     │   │   ├── util/
     │   │   │   ├── mcc_cli.hpp
     │   │   │   ├── mcc_json.hpp
     │   │   │   ├── mcc_log.hpp
     │   │   │   ├── sequence_terminal.hpp
     │   │   │   ├── utils.hpp
     │   │   │   └── utils.cpp
     │   │   └── def.hpp
     │   │ 
     │   ├── hw/
     │   │   ├── drive/
     │   │   ├── hw_def.hpp
     │   │   ├── hw.hpp
     │   │   └── hw.cpp
     │   │ 
     │   └── sys/
     │       ├── sys_data.hpp
     │       ├── sys_def.hpp
     │       ├── sys_error.hpp
     │       ├── sys.hpp
     │       └── sys.cpp
     │   
     ├── _data
     │   
     ├── mcc_lib
     │    ├── mcc_conversion.hpp
     │    ├── mcc_types.hpp
     │    └── mcc_utils.hpp
     │   
     └── main.cpp

*/
#ifdef USE_SEQUENCE_TERMINAL
 bool  TinyC::sequence_terminal::is_enabled = false;
#endif

void intialize()
{
  	// create console window
#ifdef USE_SEQUENCE_TERMINAL
    auto &vt = TinyC::sequence_terminal::get_instance();
#endif
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
#ifdef USE_SEQUENCE_TERMINAL
    TinyC::sequence_terminal::disable_sequence_terminal_processing();
#endif
    return 0;
}

