//	USER INCLUDES	//
#include "ServoControl_CSD.h"
#include "DynamixelMX12.h"
//-----------------------------------------------------------------------//

int main() {
	
	printf("Starting\n");
	//----------------------------------------------------------------------//
	//	INITIALIZE PORT REGISTERS	//
	
	void *virtual_base;
	int fd;	
	
	if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
		printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}

	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );

	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
		//Test
	}
	//----------------------------------------------------------------------//
	
	//	ADDITIONAL MEMORY MAPPING	//
	
	UART_Map(virtual_base);
	//----------------------------------------------------------------------//
	
	// LOCAL VARIABLES	//
	uint8_t Data_out[15];
	//----------------------------------------------------------------------//
	//	FUNCTIONAL CODE	//
	
	while(1)
	{
		Set_Position(SERVO1,200,Data_out);
		usleep(10000);
		Get_Position(SERVO1,Data_out);
		printf("DataOut: %s\n",Data_out);
	}
	
	//----------------------------------------------------------------------//
	//	CLEAN UP MEMORY MAPPING	//
	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );
	//----------------------------------------------------------------------//
	return( 0 );
}
