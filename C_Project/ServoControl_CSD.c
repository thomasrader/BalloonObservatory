//	USER INCLUDES	//
#include <math.h>
#include "ServoControl_CSD.h"
#include "DynamixelMX12.h"
#include "I2C_Library.h"
#include "BNO055.h";
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
/* Get_Zero_Pos will return the IMU tilt plate to its zero angle position
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


/* Continous_Mode_Test runs through the continuous mode test for the csd
*	INPUTS:
*		Contact_Addr, the address of the contact pio controller
*		Contact_DDR, the address of the pio controllers data direction register
*/
void Continous_Mode_Test(void * Contact_Addr, void * Contact_DDR){
	printf("Start dynamic test\n");
	
	 //Local variables
	 struct timeval Start_time, Time_stamp[NUMBER_OF_MEASURMENTS];
	 int IMU_roll_data[NUMBER_OF_MEASURMENTS],  IMU_pitch_data[NUMBER_OF_MEASURMENTS],  IMU_yaw_data[NUMBER_OF_MEASURMENTS];
	
	//Set servo to wheel mode
	Change_Mode(SERVO1, WHEEL, Data_out);
	
	//Reset servo position
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	
	//Start turning the servo at a set rate
	Set_Rate(SERVO1, TEST_SPEED, Data_out);
	
	printf("Servo speed set\n");
	
	//start timer
	gettimeofday(&Start_time, NULL);
	
	//start taking measuremnts 
	int i = 0;
	for(i = 0; i < NUMBER_OF_MEASURMENTS; i++){
		//delay between 
		usleep(DELAY_BETWEEN_MEASURMENTS);
		
		//Sample IMU here
		IMU_roll_data[i] = //
		IMU_pitch_data[i] = //
		IMU_yaw_data[i] = //
		
		//save timestamp
		gettimeofday(&Time_stamp[i], NULL);
	}
	
	printf("Test complete\n");
	
	//Stop the servo
	Set_Rate(SERVO1, STOP, Data_out);
	printf("Servo stopped\n");
	
	//write data points to file
	printf("Saving data to file\n");
	
void Run_Static_Test()
{
	int Cur_servo_pos = 1400;
	int Servo_step = 0;
	float Cur_angle, Angle_step = 0;
	Change_Mode(SERVO1, WHEEL, Data_out);	
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	Change_Mode(SERVO1, MULTI, Data_out);
	
	float i;
	int j;
	for(i = 0.01; i < 0.7 ; i += 0.03)
	{
		Angle_step = i;
		for(j = 0; j<3;j++)
		{
			Cur_angle += Angle_step;
			Servo_step = 2.73/(tan(Cur_angle)*0.00275);
			Cur_servo_pos += Servo_step;
			Set_Position(SERVO1, Cur_servo_pos, Data_out);
			
		}
	}
}
