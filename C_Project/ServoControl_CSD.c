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
	void* Contact_Addr	= virtual_base + ((unsigned long )( ALT_LWFPGASLVS_OFST + CONTACT_SWITCH_ADDR ) & ( unsigned long)( HW_REGS_MASK ));
	void* Contact_DDR	= virtual_base + ((unsigned long )( ALT_LWFPGASLVS_OFST + CONTACT_SWITCH_DDR_ADDR ) & ( unsigned long)( HW_REGS_MASK ));
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	//----------------------------------------------------------------------//
	
	//	ADDITIONAL MEMORY MAPPING	//
	
	UART_Map(virtual_base);
	//----------------------------------------------------------------------//
	
	// LOCAL VARIABLES	//
	uint8_t Data_out[15];
	//----------------------------------------------------------------------//
	//	TEST PROCEDURE CODE	//
	
	
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
/* Get_Zero_Pos will return the position to its zero angle position
*	
*/
void Get_Zero_Pos()
{
	*(void *) Contact_DDR = *(void *) Contact_DDR & 0xFB;
	Set_Rate(SERVO1, ROTATE_DOWN, Data_array);
	uint8_t Contact_status;
	do {
		Contact_status = *(void *) Contact_addr;
	} while(!(Contact_status) & 0x03);
	
	Set_Rate(SERVO1, STOP, Data_array);
}

