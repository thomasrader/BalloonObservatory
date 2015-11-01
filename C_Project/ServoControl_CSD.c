//	USER INCLUDES	//
#include "ServoControl_CSD.h"
#include "DynamixelMX12.h"
//-----------------------------------------------------------------------//

//GLOBAL VARIABLES
uint8_t Data_out[15];

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
	
	//----------------------------------------------------------------------//
	//	TEST PROCEDURE CODE	//
	//while(1){
		//printf("Data: %d: \n", *(uint32_t *) Contact_Addr);
	//}
	
	Change_Mode(SERVO1, WHEEL, Data_out);
	
	printf("Set Servo to wheel mode\n");
	
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	
	usleep(3000*1000);
	
	Change_Mode(SERVO1, MULTI, Data_out);
	
	Set_Position(SERVO1, 1400, Data_out);
	
	usleep(3000*1000);
	
	Change_Mode(SERVO1, WHEEL, Data_out);
	
	Set_Rate(SERVO1, 50, Data_out);
	
	usleep(3000*1000);
	
	Set_Rate(SERVO1, 0, Data_out);
	
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
void Get_Zero_Pos(void * Contact_Addr, void * Contact_DDR)
{
	*(uint32_t *) Contact_DDR = *(uint32_t  *) Contact_DDR & 0xFB;
	Set_Rate(SERVO1, ROTATE_DOWN, Data_out);
	uint8_t Contact_status;
	printf("Stated Turning Motor\n");
	do {
		Contact_status = *(uint32_t *) Contact_Addr;
	} while((Contact_status & 0x04));
	
	printf("Stated Turning Motor %d\n",  (Contact_status));
	
	Set_Rate(SERVO1, STOP, Data_out);
}

