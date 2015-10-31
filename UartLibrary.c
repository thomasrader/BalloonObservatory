//	USER INCLUDES	//

#include "UartLibrary.h"
//----------------------------------------------------------------------//
//	MEMORY MAPPED ADDRESSES	// (Need to be cast to uint32_t before dereference)

void* Data_out_reg;		// Data from UART
void* Data_in_reg;		// Data into UART

void* Flag_reg;		/* Bit order
					* | 3 | 2| 1| 0|
					* |R/W|Re|We|Ce|	-- MUX ReadWrite/ Read Enable/ Write enable/ Chip enable
					*/

void* Addr_reg;		/*
					* [00] => Data buffer, both transmission and reception.
					* [01] => Status/control register (r/w).
					* [10] => Bit period register, low byte.
					* [11] => Bit period register, high byte.
					*/
//----------------------------------------------------------------------//
/* UART_map allows the required registers to be mapped by the UART library
* 	INPUT:	virtual_base	-- the base address of the virtually mapped memory
*/

void UART_Map(void* virtual_base)
{
	Data_in_reg  = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_DATA_IN_REG ) & ( unsigned long)( HW_REGS_MASK ) );
	Data_out_reg = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_DATA_OUT_REG ) & ( unsigned long)( HW_REGS_MASK ) );
	Flag_reg	 = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_FLAG_REG ) & ( unsigned long)( HW_REGS_MASK ) );
	Addr_reg	 = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_ADDR_REG ) & ( unsigned long)( HW_REGS_MASK ) );
}

//----------------------------------------------------------------------//
/* UART_Write will write a single bit into the UART TX buffer
*	INPUTS:	data	-- This is an 8bit data byte to be written into the buffer
*/

int UART_Write(uint8_t data)
{
	*(uint32_t *) Addr_reg =  STATUS_ADDR;
	
	uint8_t status;
	do 	// check TX busy flag
	{
		status = *(uint32_t *)Data_out_reg;
	}while(!(status & 0x01));
	
	*(uint32_t *) Addr_reg =  DATA_ADDR;
	
	*(uint32_t *)Data_in_reg = data;
	
	*(uint32_t *)Flag_reg = WRITE_START_FLAGS;
	
	*(uint32_t *)Flag_reg = WRITE_STOP_FLAGS;	// Toggle Write enable flag
	
	return 0;
}
//----------------------------------------------------------------------//
/* UART_WriteRead will write a given packet to the UART register one byte at a time, and then read any device response
*	INPUTS:	data[]	-- an array of bytes to be sent
			Write_Length	-- the length of the array of bytes to be written
			Read_Length		-- expected length of device's response
			Data_array[]	-- an empty array in which to store the devices response
*/

void UART_WriteRead(uint8_t data[], int Write_length, int Read_length, uint8_t Data_array[])
{
	// Write Bytes
	int i;
	volatile uint8_t status;
	for(i = 0; i< Write_length +1 ; i++)
	{
		UART_Write(data[i]);
	}
	
	// Read Bytes
	for(i = 0; i<Read_length+PACKET_OVERHEAD; i++)
	{
		*(uint32_t *)Addr_reg = STATUS_ADDR;
		*(uint32_t *)Flag_reg = READ_TOGGLE_ON;
		*(uint32_t *)Flag_reg = READ_TOGGLE_OFF;
		do 	// check RX busy flag
		{
			printf("Checking RxRdy\n");
			status = *(uint32_t *)Data_out_reg;
		}while(!(status & 0x02));
		
		*(uint32_t *) Addr_reg =  DATA_ADDR;
		Data_array[i] = *(uint32_t *) Data_out_reg;
	}
}
//----------------------------------------------------------------------//
/* UART_SetBaud is currently not implimented due to hardcoded baude rate in UART VHDL block
*
*/

/*
void UART_SetBaud(uint32_t baudRate){
	int bitPeriod = 50000000 / baudRate;
	
	*(uint32_t *) Addr_reg =  BIT_PERIOD_L ;
	
	*(uint32_t *)Data_in_reg = bitPeriod & 0xFF;
	
	*(uint32_t *)Flag_reg = WRITE_START_FLAGS;
	
	*(uint32_t *)Flag_reg = WRITE_STOP_FLAGS;

	
	*(uint32_t *) Addr_reg =  BIT_PERIOD_H;
	
	*(uint32_t *)Data_in_reg = bitPeriod >> 8;
	
	*(uint32_t *)Flag_reg = WRITE_START_FLAGS;
	
	*(uint32_t *)Flag_reg = WRITE_STOP_FLAGS;
	}
*/
//----------------------------------------------------------------------//
