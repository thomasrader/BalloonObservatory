//Zach's Awesome I2C Library Header File
//---------------------------------------------------------------------------
//Zachary Frazee
//18 October, 2015
//For EELE 488R: High Altitude Balloon Based Observatory Payload
//---------------------------------------------------------------------------
//Designed for use with DE0-Nano_SOC by terasIC
//---------------------------------------------------------------------------


//Includes
//----------------------------------------------------------------------------
#define soc_cv_av

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "hwlib.h"
#include "soc_cv_av/socal/socal.h"
#include "soc_cv_av/socal/hps.h"
#include "soc_cv_av/socal/alt_gpio.h"
#include "hps_0.h"

#include "I2C_Library.h"



//Basic I2C Functions
//----------------------------------------------------------------------------

//SDA Function
//Sets or Clears SDA
void SDA(char setValue, void *dataReg){
	if(setValue == HIGH){
		//Set SDA Data Bit
		*(uint32_t *)dataReg |= SDA_SET;
	} else {
		//Clear SDA Data Bit
		*(uint32_t *)dataReg &= SDA_CLEAR;
	}
}

//SCL Function
//Sets or Clears SCL
void SCL(char setValue, void *dataReg){
	if(setValue == HIGH){
		//Set SCL Data Bit
		*(uint32_t *)dataReg |= SCL_SET;
	} else {
		//Clear SCL Data Bit
		*(uint32_t *)dataReg &= SCL_CLEAR;
	}
}

//SDA_DDR Function
//Sets SDA's Data Direction as IN or OUT
void SDA_DDR(char setValue, void *DDReg){
	if(setValue == OUT){
		//Set SDA DDR Bit
		*(uint32_t *)DDReg |= SDA_DDR_WRITE;
	} else {
		//Clear SDA DDR Bit
		*(uint32_t *)DDReg &= SDA_DDR_READ;
	}
}

//SCL_DDR Function
//Sets SCL's Data Direction as IN or OUT
void SCL_DDR(char setValue, void *DDReg){
	if(setValue == OUT){
		//Set SCL DDR Bit
		*(uint32_t *)DDReg |= SCL_DDR_WRITE;
	} else {
		//Clear SCL DDR Bit
		*(uint32_t *)DDReg &= SCL_DDR_READ;
	}
}

//SDA_READ Function
//Reads the value present on the SDA line
//ASSUMES THAT THE DATA DIRECTION HAS BEEN SET!!!!
uint8_t SDA_READ(void *dataReg){
	return ((*(uint32_t *)dataReg & SDA_READ_MASK) >> SDA_READ_SHIFT);
}

//I2C_Delay_Short
//Provides a ~1ms (or whatever is specified in the header file) delay
void I2C_Delay_Short(){
	usleep( 100 );
}

//I2C_Delay_Long
//Provides a ~5ms (or whatever is specified in the header file) delay
void I2C_Delay_Long(){
	usleep( 200 );
}



//I2C Operations Functions
//----------------------------------------------------------------------------

//I2C_Start Function
//Sends an I2C Start Condition
void I2C_Start(void *dataReg, void *DDReg){
	//Set Data Directions
	SCL_DDR(OUT, DDReg);
	SDA_DDR(OUT, DDReg);
	
	//Pull Both Lines to HIGH one at a time
	SDA(HIGH, dataReg);
	I2C_Delay_Short();
	SCL(HIGH, dataReg);
	I2C_Delay_Short();
	
	//Pull SDA low
	SDA(LOW, dataReg);
	I2C_Delay_Short();
	
	//Pull SCL low
	SCL(LOW, dataReg);
	I2C_Delay_Short();
	
	//Delay
	I2C_Delay_Long();
}

//I2C_Stop Function
//Sends an I2C Stop Condition
void I2C_Stop(void *dataReg, void *DDReg){
	//Set Data Directions
	SCL_DDR(OUT, DDReg);
	SDA_DDR(OUT, DDReg);
	
	//Pull SDA Low and SCL High
	SDA(LOW, dataReg);
	SCL(HIGH, dataReg);
	
	//Delay
	I2C_Delay_Short();
	
	//Pull SDA High
	SDA(HIGH, dataReg);
	
	//Delay
	I2C_Delay_Long();
}

//I2C_Clock Function
//Clocks the SCL Line
void I2C_Clock(void *dataReg, void *DDReg){
	//Set Data Direction
	SCL_DDR(OUT, DDReg);
	
	//Pull SCL High
	SCL(HIGH, dataReg);
	
	//Delay
	I2C_Delay_Short();
	
	//Pull SCL Low
	SCL(LOW, dataReg);
	
	//Delay
	I2C_Delay_Short();	
}

//I2C_SendAddressAndRW Function
//Sends the Address and RW bit to the devices on the I2C bus
void I2C_SendAddressAndRW(uint8_t address, uint8_t RW, void *dataReg, void *DDReg){
	//Data Variable
	uint8_t I2C_Data;	
	
	//Load the data variable with the Address and RW bit
	I2C_Data = address;
	I2C_Data = I2C_Data << 1;
	I2C_Data &= 0b11111110;
	I2C_Data |= RW;
	
	//Send the data
	I2C_SendData(I2C_Data, dataReg, DDReg);
}

//I2C_SendData Function
//Sends a byte of data over the I2C bus
void I2C_SendData(uint8_t I2C_Data, void *dataReg, void *DDReg){
	//Set the Data Directions
	SCL_DDR(OUT, DDReg);
	SDA_DDR(OUT, DDReg);
	
	//Send the data one bit at a time
	int i; 
	for(i = 0; i < 8; i++){
		//Load Data
		uint8_t  I2C_Data_Temp = I2C_Data;

		//Mask off lower bits of data
		I2C_Data_Temp &= 0b10000000;
		
		//Shift Data so that the bit of interest is the LSB
		I2C_Data_Temp = I2C_Data_Temp >> 7;
		
		//Set SDA as appropriate
		if(I2C_Data_Temp == 1){
			SDA(HIGH, dataReg);
		} else {
			SDA(LOW, dataReg);
		}
		
		//Clock in the data
		I2C_Clock(dataReg, DDReg);
		
		//Shift the data so that the next bit is transmitted
		I2C_Data = I2C_Data << 1;
	}
	
	//Clear SDA
	SDA(LOW, dataReg);
	
	//Delay
	I2C_Delay_Long();
}


//I2C_ReadData Function
//Reads a byte of data from the slave device
uint8_t I2C_ReadData(void *dataReg, void *DDReg){
	//Set the Data Directions
	SCL_DDR(OUT, DDReg);
	SDA_DDR(IN, DDReg);
	
	//Data Variable
	uint8_t I2C_Data = 0;
	
	int i;
	for(i = 0; i < 8; i++){
		//Shift data
		I2C_Data = I2C_Data << 1;
	
		//Pull SCL High
		SCL(HIGH, dataReg);
		
		//Delay
		I2C_Delay_Short();
		
		//Read data from SDA
		I2C_Data |= SDA_READ(dataReg);
		
		//Pull SCL Low
		SCL(LOW, dataReg);
		
		//Delay
		I2C_Delay_Short();
	}
	
	//Delay
	I2C_Delay_Long();
	
	return I2C_Data;
}

//I2C_ACK Function
//Send an ACK
void I2C_ACK(void *dataReg, void *DDReg){
	//Set the Data Direction
	SDA_DDR(OUT, DDReg);
	
	//Pull SDA to a low
	SDA(LOW, dataReg);
	
	//clock
	I2C_Clock(dataReg, DDReg);
}

//I2C_NACK Function
//Send an NACK
void I2C_NACK(void *dataReg, void *DDReg){
	//Set the Data Direction
	SDA_DDR(OUT, DDReg);
	
	//Pull SDA to a High
	SDA(HIGH, dataReg);
	
	//delay
	I2C_Delay_Short();
	
	//clock
	I2C_Clock(dataReg, DDReg);
	
	//Pull SDA back down
	SDA(LOW, dataReg);
	
	//delay
	I2C_Delay_Long();
}

//I2C_getACK
//Reads an ACK(0) or a NACK(1) from the slave
uint8_t I2C_getACK(void *dataReg, void *DDReg){
	//Set the Data Direction
	SDA_DDR(IN, DDReg);
	SCL_DDR(OUT, DDReg);
	
	//Pull SCL High
	SCL(HIGH, dataReg);
	
	//Delay
	I2C_Delay_Short();
	
	//Read SDA
	uint8_t ack = SDA_READ(dataReg);
	
	//Pull SCL Low
	SCL(LOW, dataReg);
	
	//Delay
	I2C_Delay_Short();

	return ack;
}


//I2C High-Level Functions
//----------------------------------------------------------------------------

//I2C_ReadByte Function
//Reads a byte of data given a slave and register address
uint8_t I2C_ReadByte(uint8_t slaveAddress, uint8_t registerAddress, void *dataReg, void *DDReg, uint8_t * failure){
	//data variable
	uint8_t I2C_Data = 0;
	
	//Send Start Condition
	I2C_Start(dataReg, DDReg);
	
	//Send Slave Address and Write bits
	I2C_SendAddressAndRW(slaveAddress, 0, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Send Register Address
	I2C_SendData(registerAddress, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Repeated Start Condition
	I2C_Start(dataReg, DDReg);
	
	//Send Slave Address and Read bits
	I2C_SendAddressAndRW(slaveAddress, 1, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Read Data
	I2C_Data = I2C_ReadData(dataReg, DDReg);
	
	//Send NACK
	I2C_NACK(dataReg, DDReg);
	
	//Stop Transmission
	I2C_Stop(dataReg, DDReg);
	
	//return data
	return I2C_Data;
}

void I2C_WriteByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t data,void *dataReg, void *DDReg, uint8_t * failure){
	//Send Start Condition
	I2C_Start(dataReg, DDReg);
	
	//Send Slave Address and Write bits
	I2C_SendAddressAndRW(slaveAddress, 0, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Send Register Address
	I2C_SendData(registerAddress, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Send data
	I2C_SendData(data, dataReg, DDReg);
	
	//Send ACK
	*failure |= I2C_getACK(dataReg, DDReg);
	
	//Stop Transmission
	I2C_Stop(dataReg, DDReg);
}






























