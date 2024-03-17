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
//#include "ap_log.hpp"

/* ap object */
/* register -> communication -> basic unit(engine) -> combined unit(engine) -> control */
//register + io_manager
//basic
#include "ap_reg.hpp"
#include "ap_io.hpp"

#ifdef _USE_EXHW_IICOMM_DEV
#include "iic_cmd.hpp"
#else
#include "uart_cmd.hpp"
#endif

//#include "iic_loadcell.hpp"

// engine
#include "enLed.hpp"
//#include "enBtn.hpp"
// #include "enOp.hpp"
// #include "enStepMotor.hpp"
#include "enLoadCell.hpp"

#ifndef _USE_EXHW_IICOMM_DEV
//device
#include "dev_io.hpp"
#endif
//control
// #include "cnMotors.hpp"
//#include "cnAuto.hpp"
// #include "cnTasks.hpp"

//machine test
#ifndef _USE_EXHW_IICOMM_DEV
// #include "api_cmd.hpp"
#endif


void  apInit(void);
void  apMain(void);

#endif /* INCLUDE_AP_HPP_ */
