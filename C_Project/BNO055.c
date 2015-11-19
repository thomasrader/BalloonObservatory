#include "BNO055.h"


/* UART_map allows the required registers to be mapped by the UART library
* 	INPUT:	virtual_base	-- the base address of the virtually mapped memory
*/

void * PIO_Data;
void * PIO_DDIR;

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
	//Failure flag
	uint8_t failed = 0;
	
	//Read Roll
	IMU_data[0] = (I2C_ReadByte(0x28, EULER_PITCH_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, EULER_PITCH_L, PIO_Data, PIO_DDIR, &failed));
	IMU_data[1] = (I2C_ReadByte(0x28, EULER_ROLL_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, EULER_ROLL_L, PIO_Data, PIO_DDIR, &failed));
	IMU_data[2] = (I2C_ReadByte(0x28, EULER_YAW_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, EULER_YAW_L, PIO_Data, PIO_DDIR, &failed));
}

void Get_Complementarty_Orientation(int IMU_data[], float Delta_time){
	static float pitch = 0;
	static float roll = 0;
	static float yaw = 0;
	
	//Failure flag
	uint8_t failed = 0;
	
	uint16_t Accel_x_int	= (I2C_ReadByte(0x28, ACCEL_X_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, ACCEL_X_L, PIO_Data, PIO_DDIR, &failed));
	uint16_t Accel_y_int	= (I2C_ReadByte(0x28, ACCEL_Y_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, ACCEL_Y_L, PIO_Data, PIO_DDIR, &failed));
	uint16_t Accel_z_int	= (I2C_ReadByte(0x28, ACCEL_Z_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, ACCEL_Z_L, PIO_Data, PIO_DDIR, &failed));
	
	//printf("Accel_x_u: %d\n", Accel_x_int);
	//printf("Accel_y_u: %d\n", Accel_y_int);
	//printf("Accel_z_u: %d\n", Accel_z_int);
	
	uint16_t Mag_x_int 	= (I2C_ReadByte(0x28, MAG_X_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, MAG_X_L, PIO_Data, PIO_DDIR, &failed));
	uint16_t Mag_y_int 	= (I2C_ReadByte(0x28, MAG_Y_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, MAG_Y_L, PIO_Data, PIO_DDIR, &failed));
	uint16_t Mag_z_int 	= (I2C_ReadByte(0x28, MAG_Z_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, MAG_Z_L, PIO_Data, PIO_DDIR, &failed));
	
	//printf("Mag_x_u: %d\n", Mag_x_int);
	//printf("Mag_y_u: %d\n", Mag_y_int);
	//printf("Mag_z_u: %d\n", Mag_z_int);
	
	uint16_t Gyro_x_int	= (I2C_ReadByte(0x28, GYRO_X_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, GYRO_X_L, PIO_Data, PIO_DDIR, &failed)); 
	uint16_t Gyro_y_int	= (I2C_ReadByte(0x28, GYRO_Y_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, GYRO_Y_L, PIO_Data, PIO_DDIR, &failed)); 
	uint16_t Gyro_z_int	= (I2C_ReadByte(0x28, GYRO_Z_H, PIO_Data, PIO_DDIR, &failed) << 8) | (I2C_ReadByte(0x28, GYRO_Z_L, PIO_Data, PIO_DDIR, &failed));
	
	//printf("Gyro_x_u: %d\n", Gyro_x_int);
	//printf("Gyro_y_u: %d\n", Gyro_y_int);
	//printf("Gyro_z_u: %d\n", Gyro_z_int);
	
	int Accel_x = ((int16_t)Accel_x_int);
	int Accel_y = ((int16_t)Accel_y_int);
	int Accel_z = ((int16_t)Accel_z_int);
	
	//printf("Accel_x: %d\n", Accel_x);
	//printf("Accel_y: %d\n", Accel_y);
	//printf("Accel_z: %d\n", Accel_z);
	
	int Mag_x 	= ((int16_t)Mag_x_int);
	int Mag_y 	= ((int16_t)Mag_y_int);
	int Mag_z 	= ((int16_t)Mag_z_int);
	
	float Gyro_x = ((float)((int16_t)Gyro_x_int)/(float)16) * (PI/(float)180);
	float Gyro_y = ((float)((int16_t)Gyro_y_int)/(float)16) * (PI/(float)180);
	float Gyro_z = ((float)((int16_t)Gyro_z_int)/(float)16) * (PI/(float)180);  

	//printf("Gyro_x: %lf\n", Gyro_x);
	//printf("Gyro_y: %lf\n", Gyro_y);
	//printf("Gyro_z: %lf\n", Gyro_z);
	
	roll =  GYRO_WEIGHT * (roll + Gyro_x * Delta_time) + (1 - GYRO_WEIGHT)*atan2((double)Accel_y,(double)Accel_z);
	
	//printf("Roll: %lf\t", roll);
	
	pitch = GYRO_WEIGHT * (pitch + Gyro_y * Delta_time) + (1 - GYRO_WEIGHT)*atan2(-1*Accel_x, (Accel_y*sin(roll) + Accel_z*cos(roll))); 
	
	//printf("Pitch: %lf\t", pitch);
	
	float Mag_heading = atan2(Mag_z * sin(roll) - Mag_y*cos(roll), (Mag_x*cos(pitch) + Mag_y*sin(pitch)*sin(roll) + Mag_z*sin(pitch)*cos(roll)));
	
	yaw = GYRO_WEIGHT * (yaw + Gyro_z * Delta_time) + (1 - GYRO_WEIGHT)*Mag_heading;
	
	//printf("Yaw: %lf\n", yaw);
	
	IMU_data[0]  = (int)((pitch*(180/PI))*16);
	IMU_data[1]  = (int)((roll*(180/PI))*16);
	IMU_data[2]  = (int)((yaw*(180/PI))*16);
}

//----------------------------------------------------------------------------------//
/* Set_IMU_mode sets the IMU int Relative orientation Euler angle mode
*	
*/

void Set_NDOF_mode()
{
	//Failure flag
	uint8_t failedWrite = 0;
	
	//Set IMU to NDOF mode
	I2C_WriteByte(0x28, 0x3d, 0b00001100, PIO_Data, PIO_DDIR, &failedWrite);
}
//----------------------------------------------------------------------------------//
