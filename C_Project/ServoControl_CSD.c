//	USER INCLUDES	//
#include <math.h>
#include "ServoControl_CSD.h"
#include "DynamixelMX12.h"
#include "I2C_Library.h"
#include "BNO055.h"
#include "I2C_Library.h"
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
	
	BNO055_Map(virtual_base);
	UART_Map(virtual_base);
	
	Set_NDOF_mode();
	
	Change_Mode(SERVO1, WHEEL, Data_out);
	
	
	/*
	uint8_t Error_array[15];
	uint8_t Check_sum = (~(SERVO1 + MX_GOAL_LENGTH + MX_WRITE_DATA + 20 + 0x40 + 0xA2)) & 0xFF;
	uint8_t Data_packet_2[9] = {MX_START,MX_START,SERVO1,MX_GOAL_LENGTH,MX_WRITE_DATA, 20, 0x40, 0xA2,Check_sum};
	UART_WriteRead(Data_packet_2, sizeof(Data_packet_2), ONE_BYTE_READ, Error_array);*/
	
	
	/*
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	
	//Change_Mode(SERVO1, MULTI, Data_out);
	
	
	Change_Mode(SERVO1, MULTI, Data_out);
	
	usleep(1000*3000);
	
	Change_Mode(SERVO1, MULTI, Data_out);
	
	int i;
	for(i = 2; i <=10; i++){
		Set_Position(SERVO1, i * 1000, Data_out);
		
		usleep(1000*3000);
	}
	
	*/
	
	/*
	while(1){
		int Euler_data[3];
		Get_Orientation(Euler_data);
		
		printf("Pitch,%d\nRoll,%d\nYaw,%d\n", Euler_data[0],Euler_data[1],Euler_data[2]);
		
		usleep(500* 1000);
	}*/
	 
	 /*
	void * PIO_Data = Contact_Addr;
	void * PIO_DDIR = Contact_DDR;
	 
	uint8_t failedWrite;
	I2C_WriteByte(0x28, 0x3d, 0b00001000, PIO_Data, PIO_DDIR, &failedWrite);
	
	
	
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
	} */
	 
	
	//----------------------------------------------------------------------//
	
	// LOCAL VARIABLES	//
	
	//----------------------------------------------------------------------//
	//	TEST PROCEDURE CODE	//
	//while(1){
		//printf("Data: %d: \n", *(uint32_t *) Contact_Addr);
	//}
	
	//Continous_Mode_Test(Contact_Addr, Contact_DDR);
	Run_Static_Test(Contact_Addr, Contact_DDR);
	
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
	//printf("Stated Turning Motor\n");
	do {
		Contact_status = *(uint32_t *) Contact_Addr;
	} while((Contact_status & 0x04));
	
	//printf("Stated Turning Motor %d\n",  (Contact_status));
	
	Set_Rate(SERVO1, STOP, Data_out);
}


/* Continous_Mode_Test runs through the continuous mode test for the csd
*	INPUTS:
*		Contact_Addr, the address of the contact pio controller
*		Contact_DDR, the address of the pio controllers data direction register
*/
void Continous_Mode_Test(void * Contact_Addr, void * Contact_DDR){
	
	int Euler_data[3];
	
	 //Local variables
	 struct timeval Start_time, Time_stamp[NUMBER_OF_MEASURMENTS];
	 int IMU_roll_data[NUMBER_OF_MEASURMENTS],  IMU_pitch_data[NUMBER_OF_MEASURMENTS],  IMU_yaw_data[NUMBER_OF_MEASURMENTS];
	
	//Set servo to wheel mode
	Change_Mode(SERVO1, WHEEL, Data_out);
	
	//Reset servo position
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	
	//Start turning the servo at a set rate
	Set_Rate(SERVO1, TEST_SPEED, Data_out);
	
	//start timer
	gettimeofday(&Start_time, NULL);
	
	//start taking measuremnts 
	int i = 0;
	for(i = 0; i < NUMBER_OF_MEASURMENTS; i++){
		//delay between 
		usleep(DELAY_BETWEEN_MEASURMENTS);
		
		Get_Orientation(Euler_data);
		
		//Sample IMU here
		IMU_roll_data[i] = Euler_data[1];
		IMU_pitch_data[i] = Euler_data[0];
		IMU_yaw_data[i] = Euler_data[2];
		
		//save timestamp
		gettimeofday(&Time_stamp[i], NULL);
	}
	
	//Stop the servo
	Set_Rate(SERVO1, STOP, Data_out);
	
	for(i = 0; i < NUMBER_OF_MEASURMENTS; i++){
		printf("%d,", IMU_pitch_data[i]);
		printf("%d,", IMU_roll_data[i]);
		printf("%d,", IMU_yaw_data[i]);
		
		printf("%d,", (int) Time_stamp[i].tv_sec);
		printf("%d,", (int) Time_stamp[i].tv_usec);
	}
	
}


void Run_Static_Test(void * Contact_Addr, void * Contact_DDR)
{
	int Cur_servo_pos = 1400;
	int Servo_step = 0;
	double Cur_angle = 0;
	double Angle_step = 0;
	int Current_Euler_data[3];
	int Prev_Euler_data[3];
	Change_Mode(SERVO1, WHEEL, Data_out);	
	Get_Zero_Pos(Contact_Addr, Contact_DDR);
	
	//printf("Finished Zeroing servo\n");
	
	usleep(1000*30);
	
	Change_Mode(SERVO1, MULTI, Data_out);
	
	usleep(1000*30);
	
	uint8_t Error_array[15];
	
	uint8_t Check_sum = (~(SERVO1 + MX_GOAL_LENGTH + MX_WRITE_DATA + 20)) & 0xFF;
	uint8_t Data_packet_2[9] = {MX_START,MX_START,SERVO1,MX_GOAL_LENGTH,MX_WRITE_DATA, 20, 0, 0,Check_sum};
	UART_WriteRead(Data_packet_2, sizeof(Data_packet_2), ONE_BYTE_READ, Error_array);
	
	printf("Step Size,Roll,Pitch,Yaw\n");
	
	double i;
	int j;
	for(i = 0.01; i < 0.5 ; i += 0.03)
	{
		Get_Orientation(Prev_Euler_data);
		
		Angle_step = i;
		for(j = 0; j<3;j++)
		{
			//printf("Entered second for loop\n");
			Cur_angle += Angle_step;
			
			//Find angles in radians
			double Cur_angle_rad  =  Cur_angle  * (PI/180);
			double Angle_step_rad =  Angle_step * (PI/180);
			
			Servo_step = HINGE_DISTANCE * THREADS_PER_INCH * COUNTS_PER_360 * (tan(Cur_angle_rad) - tan(Cur_angle_rad - Angle_step_rad));
			Cur_servo_pos += Servo_step;
			//printf("Did math\n");
			if(Cur_servo_pos > 24000){
				printf("It broke :(\n At %lf\n",i);
			}
			Set_Position(SERVO1, Cur_servo_pos, Data_out);

			usleep(3000*1000);
			Get_Orientation(Current_Euler_data);
			
			printf("%lf,%d,%d,%d\n",Angle_step,(Current_Euler_data[0] - Prev_Euler_data[0]),(Current_Euler_data[1] - Prev_Euler_data[1]),(Current_Euler_data[2] - Prev_Euler_data[2]));
			
			Prev_Euler_data[0] = Current_Euler_data[0];
			Prev_Euler_data[1] = Current_Euler_data[1];
			Prev_Euler_data[2] = Current_Euler_data[2];
			
		}
		
			Change_Mode(SERVO1, WHEEL, Data_out);	
			
			Get_Zero_Pos(Contact_Addr, Contact_DDR);
			
			//printf("Finished Zeroing servo\n");
			
			usleep(1000*300);
			
			Change_Mode(SERVO1, MULTI, Data_out);
			
			usleep(1000*300);
			
			Cur_servo_pos = 1400;
			Cur_angle = 0;	
			
	}
}
