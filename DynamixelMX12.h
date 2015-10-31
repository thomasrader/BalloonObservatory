#ifndef DynamixelSerial_h
#define DynamixelSerial_h
//	SYSTEM DEFINES	//

#define soc_cv_av
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
//----------------------------------------------------------------------//
//	USER DEFINES	//
enum servomode	{WHEEL,MULTI,JOINT};

#define TWO_BYTE_READ	2
#define ONE_BYTE_READ	1
//----------------------------------------------------------------------//
//	USER INCLUDES	//

#include "UartLibrary.h"
//----------------------------------------------------------------------//
//	SERVO REGISTER LOCATIONS	//


// EEPROM Area
#define MX_MODEL_NUMBER_L           0
#define MX_MODEL_NUMBER_H           1
#define MX_VERSION                  2
#define MX_ID                       3
#define MX_BAUD_RATE                4
#define MX_RETURN_DELAY_TIME        5
#define MX_CW_ANGLE_LIMIT_L         6
#define MX_CW_ANGLE_LIMIT_H         7
#define MX_CCW_ANGLE_LIMIT_L        8
#define MX_CCW_ANGLE_LIMIT_H        9
#define MX_SYSTEM_DATA2             10
#define MX_LIMIT_TEMPERATURE        11
#define MX_DOWN_LIMIT_VOLTAGE       12
#define MX_UP_LIMIT_VOLTAGE         13
#define MX_MAX_TORQUE_L             14
#define MX_MAX_TORQUE_H             15
#define MX_RETURN_LEVEL             16
#define MX_ALARM_LED                17
#define MX_ALARM_SHUTDOWN           18
#define MX_OPERATING_MODE           19
#define MX_DOWN_CALIBRATION_L       20
#define MX_DOWN_CALIBRATION_H       21
#define MX_RESOLUTION_DIV       	22	// resolution divider


// RAM Area
#define MX_TORQUE_ENABLE            24
#define MX_LED                      25
#define MX_D_COEF    				26	// D coef
#define MX_I_COEF				    27	// I coef
#define MX_P_COEF			        28	// P coef
#define MX_GOAL_POSITION_L          30
#define MX_GOAL_POSITION_H          31
#define MX_GOAL_SPEED_L             32
#define MX_GOAL_SPEED_H             33
#define MX_TORQUE_LIMIT_L           34
#define MX_TORQUE_LIMIT_H           35
#define MX_PRESENT_POSITION_L       36
#define MX_PRESENT_POSITION_H       37
#define MX_PRESENT_SPEED_L          38
#define MX_PRESENT_SPEED_H          39
#define MX_PRESENT_LOAD_L           40
#define MX_PRESENT_LOAD_H           41
#define MX_PRESENT_VOLTAGE          42
#define MX_PRESENT_TEMPERATURE      43
#define MX_REGISTERED_INSTRUCTION   44
#define MX_MOVING                   46
#define MX_LOCK                     47
#define MX_PUNCH_L                  48
#define MX_PUNCH_H                  49
#define MX_ACCLERATION_GOAL			73

//----------------------------------------------------------------------//
//	PACKET BUILDING BLOCKS	//

// Status Return Levels
#define MX_RETURN_NONE              0
#define MX_RETURN_READ              1
#define MX_RETURN_ALL               2

// Instruction Set
#define MX_PING                     1
#define MX_READ_DATA                2
#define MX_WRITE_DATA               3
#define MX_REG_WRITE                4
#define MX_ACTION                   5
#define MX_RESET                    6
#define MX_SYNC_WRITE               131

// Specials
#define OFF                         0
#define ON                          1
#define LEFT						0
#define RIGTH                       1
#define MX_BYTE_READ                1
#define MX_BYTE_READ_POS            2
#define MX_RESET_LENGTH				2
#define MX_ACTION_LENGTH			2
#define MX_ID_LENGTH                4
#define MX_LR_LENGTH                4
#define MX_SRL_LENGTH               4
#define MX_RDT_LENGTH               4
#define MX_LEDALARM_LENGTH          4
#define MX_SALARM_LENGTH            4
#define MX_TL_LENGTH                4
#define MX_VL_LENGTH                6
#define MX_CM_LENGTH                6
#define MX_CS_LENGTH                6
#define MX_CCW_CW_LENGTH            8
#define MX_BD_LENGTH                4
#define MX_TEM_LENGTH               4
#define MX_MOVING_LENGTH            4
#define MX_RWS_LENGTH               4
#define MX_VOLT_LENGTH              4
#define MX_LED_LENGTH               4
#define MX_TORQUE_LENGTH            4
#define MX_POS_LENGTH               4
#define MX_GOAL_LENGTH              5
#define MX_MT_LENGTH                5
#define MX_PUNCH_LENGTH             5
#define MX_SPEED_LENGTH             5
#define MX_GOAL_SP_LENGTH           7
#define MX_ACTION_CHECKSUM			250
#define BROADCAST_ID                254
#define MX_START                    255
#define MX_CCW_AL_L                 255 
#define MX_CCW_AL_H                 3
#define TIME_OUT                    10         // Este parametro depende de la velocidad de transmision
#define TX_DELAY_TIME				400        // Este parametro depende de la velocidad de transmision - pero pueden ser cambiados para mayor velocidad.
#define Tx_MODE                     1
#define Rx_MODE                     0
#define LOCK                        1
//----------------------------------------------------------------------//
// FUNCTION PROTOTYPE DECLARATIONS

void Set_Position(uint8_t ID, int Position, uint8_t Error_array[]);
void Set_Rate(uint8_t ID, int Rate, uint8_t Error_array[]);
int Get_Position(uint8_t ID, uint8_t Data_array[]);
void Change_Mode(uint8_t ID, servomode Set_mode, uint8_t Error_array[]);
//----------------------------------------------------------------------//
#endif