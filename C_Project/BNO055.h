#ifndef BNO055_h
#define BNO055_h

//	USER DEFINES	//
#define EULER_PITCH_H	0x1F
#define	EULER_PITCH_L		0x1E
#define EULER_ROLL_H		0x1D
#define EULER_ROLL_L		0x1C
#define EULER_YAW_H		0x1B
#define EULER_YAW_L		0x1A

#define GYRO_X_L				0x14
#define GYRO_X_H			0x15
#define GYRO_Y_L				0x16
#define GYRO_Y_H			0x17
#define GYRO_Z_L				0x18
#define GYRO_Z_H			0x19

#define ACCEL_X_L			0x08
#define ACCEL_X_H			0x09
#define ACCEL_Y_L			0x0A
#define ACCEL_Y_H			0x0B
#define ACCEL_Z_L			0x0C
#define ACCEL_Z_H			0x0D

#define MAG_X_L				0x0E
#define MAG_X_H				0x0F
#define MAG_Y_L				0x10
#define MAG_Y_H				0x11
#define MAG_Z_L				0x12
#define MAG_Z_H				0x13

#define GYRO_WEIGHT		((float) 0.98)

#ifndef PI
	#define PI 	((float)3.14159265354252)	
#endif
	
//	SYSTEM DEFINES	//

#define soc_cv_av
//----------------------------------------------------------------------//
//	SYSTEM INCLUDES	//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define PIO_Data_ADDR 0x00000000
#define PIO_DDIR_ADDR 0x00000004

#include "I2C_Library.h"
//----------------------------------------------------------------------//
//	FUNCTION PROTOTYPE DECLERATIONS	//

void BNO055_Map(void* virtual_base);
void Get_Orientation(int IMU_data[]);
void Set_NDOF_mode();
void Get_Complementarty_Orientation(int IMU_data[], float Delta_time);
//----------------------------------------------------------------------//
#endif