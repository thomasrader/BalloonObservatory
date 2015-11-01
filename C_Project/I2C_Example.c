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

//I2C Library
#include "I2C_Library.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

//PIO Block
#define PIO_Data_ADDR 0x00000000
#define PIO_DDIR_ADDR 0x00000004

//uART
#define UART_DATA_ADDR 0x00000020
#define UART_CONTROL_ADDR 0x00000024

int main() {

	// Initialize Port Registers 
	//---------------------------------------------------------------

	void *virtual_base;
	int fd;
	void *PIO_Data;
	void *PIO_DDIR;
	
	void *UART_DATA;
	void *UART_CONTROL;


	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	
	PIO_Data = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PIO_Data_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
	PIO_DDIR = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + PIO_DDIR_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
	
	UART_DATA = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_DATA_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
	UART_CONTROL = virtual_base + ( ( unsigned long  )( ALT_LWFPGASLVS_OFST + UART_CONTROL_ADDR ) & ( unsigned long)( HW_REGS_MASK ) );
	
	/*
	while(1){
		
		uint32_t temp = *(uint32_t *)UART_DATA;
		
		temp &= 0xFFFFFF00;
		
		*(uint32_t *)UART_DATA = (temp | 0x000000AA);
		
		usleep(2000);
	}*/
	
	
	//My code
	//--------------------------------------------------------------
	uint8_t failedWrite;
	
	//Set IMU to IMU mode
	I2C_WriteByte(0x28, 0x3d, 0b00001000, PIO_Data, PIO_DDIR, &failedWrite);
	
	
	
	//Read the entire memory from 0x0 to 0x3b of the imu
	int i; 
	for(i = 0x0; i < 0x3b; i++){
		uint8_t failed = 0;
		
		//Read Data From Device
		int data = I2C_ReadByte(0x28, i, PIO_Data, PIO_DDIR, &failed);
		
		
		if(failed){
			printf("Failed\n");
		}
			
			
		printf("Data(0x%02x): %d\n",i ,data);
		
		//I2C_Start(PIO_Data, PIO_DDIR);
		
		usleep(10000);
	} 
	
	
	// clean up our memory mapping and exit
	//-----------------------------------------------
	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );
}








































