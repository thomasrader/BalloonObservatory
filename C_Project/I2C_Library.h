//Zach's Awesome I2C Library Header File
//---------------------------------------------------------------------------
//Zachary Frazee
//18 October, 2015
//For EELE 488R: High Altitude Balloon Based Observatory Payload
//---------------------------------------------------------------------------
//Designed for use with DE0-Nano_SOC by terasIC
//---------------------------------------------------------------------------

#ifndef I2C_LIBRARY_H_
#define I2C_LIBRARY_H_

//Definitions
//----------------------------------------------------

//Signals
#define HIGH 	1
#define LOW 	0
#define IN 		0
#define OUT 		1

//Read/Write Masks
#define SDA_SET			0b00000001
#define SDA_CLEAR 		0b11111110
#define SCL_SET 			0b00000010
#define SCL_CLEAR		0b11111101

#define SDA_READ_MASK		0b00000001
#define SDA_READ_SHIFT		0

#define SDA_DDR_WRITE		0b00000001
#define SDA_DDR_READ		0b11111110
#define SCL_DDR_WRITE		0b00000010
#define SCL_DDR_READ		0b11111101

//Timing
#define ShortDelay_uS		(1000*1000)
#define LongDelay_uS		(5000*1000)

//Function Prototypes
//----------------------------------------------------

//I2C Basic Functions
void SDA(char setValue, void *dataReg);
void SCL(char setValue, void *dataReg);

void SDA_DDR(char setValue, void *DDReg);
void SCL_DDR(char setValue, void *DDReg);

uint8_t SDA_READ(void *dataReg);

void I2C_Delay_Short();
void I2C_Delay_Long();

//I2C_Operations
void I2C_Start(void *dataReg, void *DDReg);
void I2C_Stop(void *dataReg, void *DDReg);
void I2C_Clock(void *dataReg, void *DDReg);

void I2C_SendAddressAndRW(uint8_t address, uint8_t RW, void *dataReg, void *DDReg);
void I2C_SendData(uint8_t I2C_Data, void *dataReg, void *DDReg);
uint8_t I2C_ReadData(void *dataReg, void *DDReg);

void I2C_ACK(void *dataReg, void *DDReg);
void I2C_NACK(void *dataReg, void *DDReg);
uint8_t I2C_getACK(void *dataReg, void *DDReg);

//High-Level Operations
uint8_t I2C_ReadByte(uint8_t slaveAddress, uint8_t registerAddress, void *dataReg, void *DDReg, uint8_t * failure);
void I2C_WriteByte(uint8_t slaveAddress, uint8_t registerAddress, uint8_t data,void *dataReg, void *DDReg, uint8_t * failure);

#endif //I2C_LIBRARY_H_ 