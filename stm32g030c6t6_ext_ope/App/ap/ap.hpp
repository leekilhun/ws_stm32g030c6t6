/*
 * ap.hpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#pragma once
#ifndef INCLUDE_AP_HPP_
#define INCLUDE_AP_HPP_

/*

#include "utils.h"
#include "uart.h"
#include "led.h"
*/


#include "ap_def.hpp"

/* ap object*/
#include "ap_utils.hpp"
#include "ap_dat.hpp"

#include "ap_reg.hpp"
#include "ap_io.hpp"
#include "enLed.hpp"
#include "enBtn.hpp"

#include "enOp.hpp"
#include "enWheel.hpp"

void  apInit(void);
void  apMain(void);

#endif /* INCLUDE_AP_HPP_ */
