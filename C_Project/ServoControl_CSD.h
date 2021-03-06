#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

//	USER DEFINES	//

#define SERVO1	0x01
#define ROTATE_DOWN		1150
#define CONTACT_SWITCH_ADDR	0x0
#define CONTACT_SWITCH_DDR_ADDR	0x4
#define STOP	0x0

#define TEST_SPEED 10
#define DELAY_BETWEEN_MEASURMENTS (1000*1000)
#define NUMBER_OF_MEASURMENTS 75

#define HINGE_DISTANCE 			((double)2.5)		//Distance from threaded rod to hinge in [in]
#define THREADS_PER_INCH 		32					//Thread count per inch of the threaded rod
#define INCHS_PER_THREAD		((double)0.03125)
#define COUNTS_PER_360 		4096					//servo encoder counter per full revolution 
#define OMEGA							((double)0.19)	//Turn rate of servo in rev/sec

#define PI 	3.14159265354252		//Yummy

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
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"
//----------------------------------------------------------------------//
//	FUNCTION PROTOTYPES	//

void Get_Zero_Pos(void * Contact_Addr, void * Contact_DDR);
void Run_Static_Test(void * Contact_Addr, void * Contact_DDR);
void Continous_Mode_Test(void * Contact_Addr, void * Contact_DDR);

#endif