#include "BNO055.h"


/* UART_map allows the required registers to be mapped by the UART library
* 	INPUT:	virtual_base	-- the base address of the virtually mapped memory
*/

void BNO055_Map(void* virtual_base)
{
	PIO_Data = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PIO_Data_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
	PIO_DDIR = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PIO_DDIR_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
}
//----------------------------------------------------------------------------------//
/* Get_Orientation retrieves the Euler angle Pitch Roll and Yaw
*	INPUT:	IMU_data[]	-- in order Pitch Roll Yaw
*/

void Get_Orientation(int IMU_data[])
{
	//Read Roll
	IMU_data[0] = (I2C_ReadByte(0x28, EULER_PITCH_H, PIO_Data, PIO_DDIR, &failed) << 8) & (I2C_ReadByte(0x28, EULER_PITCH_L, PIO_Data, PIO_DDIR, &failed);
	IMU_data[1] = (I2C_ReadByte(0x28, EULER_ROLL_H, PIO_Data, PIO_DDIR, &failed) << 8) & (I2C_ReadByte(0x28, EULER_ROLL_L, PIO_Data, PIO_DDIR, &failed);
	IMU_data[1] = (I2C_ReadByte(0x28, EULER_YAW_H, PIO_Data, PIO_DDIR, &failed) << 8) & (I2C_ReadByte(0x28, EULER_YAW_L, PIO_Data, PIO_DDIR, &failed);
}

//----------------------------------------------------------------------------------//
/* Set_IMU_mode sets the IMU int Relative orientation Euler angle mode
*	
*/

void Set_NDOF_mode()
{
	//Set IMU to NDOF mode
	I2C_WriteByte(0x28, 0x3d, 0b00001100, PIO_Data, PIO_DDIR, &failedWrite);
}
//----------------------------------------------------------------------------------//
