/*
 * ap.hpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_AP_HPP_
#define INCLUDE_AP_HPP_

/*

#include "utils.h"
#include "uart.h"
#include "led.h"
*/


#include "ap_def.h"

#include "def_obj.hpp"

/* ap object*/
#include "ap_utils.hpp"
#include "ap_dat.hpp"
#include "ap_log.hpp"

/* ap object */
/* register -> communication -> basic unit(engine) -> combined unit(engine) -> control */
//register + io_manager
//basic
#include "ap_reg.hpp"
#include "ap_io.hpp"

#include "uart_cmd.hpp"

// engine
#include "enBtn.hpp"
#include "enLed.hpp"
#include "enOp.hpp"
#include "enStepMotor.hpp"


//control
#include "cnAuto.hpp"
#include "cnTasks.hpp"

//machine test
#include "api_cmd.hpp"



void  apInit(void);
void  apMain(void);

#endif /* INCLUDE_AP_HPP_ */
