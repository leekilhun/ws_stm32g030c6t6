#include "pch.h"
#include "core/ap/ap.hpp"

/*
cl /EHsc /std:c++20 main.cpp /Fe: m.exe
cl /Zi /EHsc /std:c++20 /nologo /Isrc /Isrc/core /Isrc/core/ap /Isrc/core/sys /Isrc/core/hw /Isrc/core/common /Isrc/core/common/util /Isrc/mcc_lib /Isrc/core/common/_inc_hw /Fo"./build/" /Fd"./build/vc140.pdb" /Fe"./build/hx711.exe" src/main.cpp src/core/core.cpp src/core/ap/ap.cpp src/core/hw/hw.cpp src/core/sys/sys.cpp src/core/common/util/utils.cpp /link /INCREMENTAL:NO

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

