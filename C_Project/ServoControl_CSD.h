#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

//	USER DEFINES	//

#define SERVO1	0x01
//-----------------------------------------------------------------------//
//	USER INCLUDES	//

#include "UartLibrary.h"
//----------------------------------------------------------------------//
//	SYSTEM DEFINES	//

#define soc_cv_av
#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
//----------------------------------------------------------------------//
//	SYSTEM INCLUDES	//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"
//----------------------------------------------------------------------//

#endif