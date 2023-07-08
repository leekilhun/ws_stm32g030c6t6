/*
 * ap_def.h
 *
 *  Created on: 2023. 6. 9.
 *      Author: gns2l
 */

#ifndef AP_AP_DEF_H_
#define AP_AP_DEF_H_


#include "exhw.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <string>

#ifndef errno_t
#define errno_t int
#endif


#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0
#endif

#include "def_obj.hpp"

typedef void (*evt_cb)(void* obj, void* w_parm, void* l_parm);



//#define _USE_AP_MACHINE_TEST




#endif /* AP_AP_DEF_H_ */
