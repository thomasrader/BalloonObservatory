//	THIS IS A TEST, IN A REAL EMERGENCY YOU WOULD BE SCREWED ALREADY	//
//	USER INCLUDES	//
#include "DynamixelMX12.h"
//----------------------------------------------------------------------//
/* Set_Rate generates the necessary packet to load a goal rotation rate into the servo
*	INPUTS:	ID			-- Id number of servo to communicate with
*			Rate		-- The rate to acheive 
*			Error_array	-- An array used to return any error codes produced by writing to the servo
*/
void Set_Rate(uint8_t ID, int Rate, uint8_t Error_array[])
{
	uint8_t Rate_L, Rate_H;
	uint8_t Check_sum;
	Rate_H = (Rate >> 8) & 0xFF;
	Rate_L = Rate & 0xFF;
	Checksum = (~(ID + MX_SPEED_LENGTH + MX_WRITE_DATA + MX_GOAL_SPEED_L + Rate_L + Rate_H)) & 0xFF;
	uint8_t Data_packet[9]	= {MX_START,MX_START,ID,MX_SPEED_LENGTH,MX_WRITE_DATA,MX_GOAL_SPEED_L,Rate_L,Rate_H,Check_sum};
	
	UART_WriteRead(Data_packet, sizeof(Data_packet), ONE_BYTE_READ, Error_array);
}
//----------------------------------------------------------------------//
/* Set_Position generates the necessary packet to load a goal position into the servo
*	INPUTS:	ID			-- Id number of servo to communicate with
*			Error_array	-- An array used to return any error codes produced by writing to the servo
*/
void Set_Position(uint8_t ID, int Position, uint8_t Error_array[])
{
	uint8_t Position_H, Position_L;
	uint8_t Check_sum;
	Position_H = (Position >> 8) & 0xFF;
	Position_L = Position & 0xFF;
	Check_sum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_GOAL_POSITION_L + Position_L + Position_H)) & 0xFF;
	uint8_t Data_packet[9] = {MX_START,MX_START,ID,MX_GOAL_LENGTH,MX_WRITE_DATA,MX_GOAL_POSITION_L,Position_L,Position_H,Check_sum};
	
	UART_WriteRead(Data_packet, sizeof(Data_packet), ONE_BYTE_READ, Error_array);
}
//----------------------------------------------------------------------//
/* Set_Change_Mode allows the user to set which mode the servo is operting in
*	INPUTS:	ID			-- Id number of servo to communicate with
*			Error_array	-- An array used to return any error codes produced by writing to the servo
*/
void Change_Mode(uint8_t ID, servomode Set_mode, uint8_t Error_array[])
{
	uint8_t Mode_L, Mode_H;
	uint8_t Check_sum;
	
	switch(Set_mode)
	{
		case(servomode.MULTI):
			Mode_L = 0xFF;
			Mode_H = 0x0F;
			Check_sum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_CCW_ANGLE_LIMIT_L + Mode_L + Mode_H + Mode_L + Mode_H)) & 0xFF;
			uint8_t Data_packet[11] = {MX_START,MX_START,ID,MX_CCW_CW_LENGTH,MX_WRITE_DATA,MX_CW_ANGLE_LIMIT_L,Mode_L,Mode_H,Mode_L,Mode_H,Check_sum};
			UART_WriteRead(Data_packet, sizeof(Data_packet), ONE_BYTE_READ, Error_array);
			break;
		case(servomode.WHEEL):
			Mode_L = 0x0;
			Mode_H = 0x0;
			Check_sum = (~(ID + MX_GOAL_LENGTH + MX_WRITE_DATA + MX_CCW_ANGLE_LIMIT_L + Mode_L + Mode_H + Mode_L + Mode_H)) & 0xFF;
			uint8_t Data_packet[11] = {MX_START,MX_START,ID,MX_CCW_CW_LENGTH,MX_WRITE_DATA,MX_CW_ANGLE_LIMIT_L,Mode_L,Mode_H,Mode_L,Mode_H,Check_sum};
			UART_WriteRead(Data_packet, sizeof(Data_packet), ONE_BYTE_READ, Error_array);
			break;
		case(servomode.JOINT):
			break;
		default:
			break;
	}
}
//----------------------------------------------------------------------//
/* Get_Position will return the current encoder position of the set
*	INPUTS:	ID	-- Id number of servo to communicate with
*
*	OUTPUTS: Position	-- Encoder read position value (mode dependent range)
*/
int Get_Position(uint8_t ID, uint8_t Data_array[])
{
	Check_sum = (~(ID + MX_GOAL_LENGTH + MX_READ_DATA + MX_PRESENT_POSITION_L + MX_BYTE_READ_POS)) & 0xFF;
	uint8_t Data_packet[5] = {ID, MX_GOAL_LENGTH, MX_READ_DATA, MX_PRESENT_POSITION_L, MX_BYTE_READ_POS};
	
	UART_WriteRead(Data_packet, sizeof(Data_packet), TWO_BYTE_READ, Data_array);
}
//----------------------------------------------------------------------//