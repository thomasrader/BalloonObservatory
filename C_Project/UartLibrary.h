#ifndef UART_LIBRARY_H
#define UART_LIBRARY_H

//----------------------------------------------------------------------//
//	SYSTEM DEFINES	//

#ifndef soc_cv_av
	#define soc_cv_av
#endif

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
//---------------------------------------------------------------------//
//	FPGA UART BLOCK ADDRESSES	//

#define UART_DATA_IN_REG		0x80
#define UART_DATA_OUT_REG 		0x60
#define	UART_FLAG_REG			0x40
#define	UART_ADDR_REG			0x20
//---------------------------------------------------------------------//
//	UART BLOCK	//

#define BIT_PERIOD_L 		0x10
#define BIT_PERIOD_H 		0x11
#define STATUS_ADDR			0x01
#define DATA_ADDR			0x00

#define WRITE_START_FLAGS	0b1011	// MUX write, Read Enable, Write Enable, Chip Enable
#define WRITE_STOP_FLAGS	0b1001	// MUX write, Read Enable, Write Enable, Chip Enable

#define READ_TOGGLE_ON		0b0101
#define READ_TOGGLE_OFF		0b0011			

#define PACKET_OVERHEAD 	0x05
//---------------------------------------------------------------------//
// FUNCTION PROTOTYPE DECLARATIONS	//

int UART_Write(uint8_t data);
void UART_WriteRead(uint8_t data[], int Write_length, int Read_length, uint8_t Data_array[]);
void UART_Map(void* virtual_base);
void UART_SetBaud(uint32_t baudRate);
//----------------------------------------------------------------------//
#endif