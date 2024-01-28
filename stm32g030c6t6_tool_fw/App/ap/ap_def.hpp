/*
 * ap_def.h
 *
 *  Created on: 2023. 6. 9.
 *      Author: gns2l
 */

#ifndef AP_AP_DEF_HPP_
#define AP_AP_DEF_HPP_


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
#ifndef ERROR_FAIL
#define ERROR_FAIL -1
#endif

#define GPIO_IN_STATE_ON  GPIO_PIN_RESET
#define GPIO_IN_STATE_OFF  GPIO_PIN_SET
#define GPIO_OUT_STATE_ON  GPIO_PIN_SET
#define GPIO_OUT_STATE_OFF  GPIO_PIN_RESET


#define MaKEWORD(a, b)      ((uint16_t)(((uint8_t)(((uint32_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint32_t)(b)) & 0xff))) << 8))
#define MaKEDWORD(a, b)     ((uint32_t)(((uint16_t)(((uint32_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint32_t)(b)) & 0xffff))) << 16))
#define LoWORD(l)           ((uint16_t)(((uint32_t)(l)) & 0xffff))
#define HiWORD(l)           ((uint16_t)((((uint32_t)(l)) >> 16) & 0xffff))
#define LoBYTE(w)           ((uint8_t)(((uint16_t)(w)) & 0xff))
#define HiBYTE(w)           ((uint8_t)((((uint16_t)(w)) >> 8) & 0xff))

#include "def_obj.hpp"
#include "def_err.hpp"

typedef void (*evt_cb)(void* obj, void* w_parm, void* l_parm);



//#define _USE_AP_MACHINE_TEST
#define  _USE_APP_ENBTN
#define  _USE_APP_CNTASKS



#endif /* AP_AP_DEF_HPP_ */
