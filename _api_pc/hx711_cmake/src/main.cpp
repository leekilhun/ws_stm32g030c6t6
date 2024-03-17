#include "core/ap/ap.hpp"



void intialize()
{
  
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
    return 0;
}


