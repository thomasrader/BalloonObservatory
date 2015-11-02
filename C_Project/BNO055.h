#ifndef BNO055_h
#define BNO055_h

1f-1a
//	USER DEFINES	//
#define EULER_PITCH_H	0x1F
#define	EULER_PITCH_L	0x1E
#define EULER_ROLL_H	0x1D
#define EULER_ROLL_L	0x1C
#define EULER_YAW_H		0x1B
#define EULER_YAW_L		0x1A

//	SYSTEM DEFINES	//

#define soc_cv_av
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
//	FUNCTION PROTOTYPE DECLERATIONS	//

void Get_Orientation(int IMU_data[])
void Set_NDOF_mode();
//----------------------------------------------------------------------//
#endif