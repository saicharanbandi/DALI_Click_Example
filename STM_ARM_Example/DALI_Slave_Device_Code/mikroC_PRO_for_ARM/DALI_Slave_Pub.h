/**************************************************************************************************
* File: Dali_Slave_Pub.h
* File Type: C - Header File
* Project Name: DALI definitions
* Company: (c) mikroElektronika, 2012
* Revision History:
*       - initial release;
* Description:
*     This project contains definitions used by API functions for DALI protocol4.
*
* Target:
*       MCU:             STM32F407VG
*       Dev.Board:       EasyMx PRO v7 for STM32 ARM

*       Oscillator:      168MHz
*       SW:              mikroC PRO for ARM

* NOTES:
*     This project will be used to test Dali
**************************************************************************************************/
#ifndef DALI_H
#define DALI_H

#define LOW_POWER_TIMEOUT       2000 // 2 seconds to go to sleep/halt

#define DALI_START_BIT_PULSE        0
#define DALI_END_BIT_PULSE          1

//Constants for dali_state
#define DALI_IDLE                        0  // DALI sender: idle mode
#define DALI_SEND_START                  1  // DALI sender: send start condition
#define DALI_SEND_ADDRESS                2  // Send address bits
#define DALI_SEND_DATA                   3  // Send data bits
#define DALI_SEND_STOP                   4  // Send stop bits
#define DALI_SEND_SETTLING               5  // Wait settling time
#define DALI_SEND_WAIT                   6  // Waiting for answer frame

//Constants for dali_receive_status
#define DALI_READY_TO_RECEIVE            0
#define DALI_NEW_FRAME_RECEIVED          1
#define DALI_RECEIVE_OVERFLOW            2

//Constants for dali_error
#define DALI_NO_ERROR                    0
#define DALI_INTERFACE_FAILURE_ERROR     1

#define DALI_REPETITION_WAIT             120 // Command repetition timeout (ms).
                                             // datasheet timeout time min ~100ms

#define ADDR_BALLAST_MEM                 0x20004000

//for ballast memory - offsets
#define BALLAST_SHORT_ADDRESS            0x00
#define BALLAST_ACTUAL_DIM_LEVEL         0x01
#define BALLAST_POWER_ON_LEVEL           0x02
#define BALLAST_SYSTEM_FAILURE_LEVEL     0x03
#define BALLAST_MIN_LEVEL                0x04
#define BALLAST_MAX_LEVEL                0x05
#define BALLAST_FADE_RATE                0x06
#define BALLAST_FADE_TIME                0x07
#define BALLAST_SEARCH_ADDRESS_H         0x08
#define BALLAST_SEARCH_ADDRESS_M         0x09
#define BALLAST_SEARCH_ADDRESS_L         0x0A
#define BALLAST_RANDOM_ADDRESS_H         0x0B
#define BALLAST_RANDOM_ADDRESS_M         0x0C
#define BALLAST_RANDOM_ADDRESS_L         0x0D
#define BALLAST_GROUP_0_7                0x0E
#define BALLAST_GROUP_8_15               0x0F
#define BALLAST_SCENE_01                 0x10
#define BALLAST_SCENE_02                 0x11
#define BALLAST_SCENE_03                 0x12
#define BALLAST_SCENE_04                 0x13
#define BALLAST_SCENE_05                 0x14
#define BALLAST_SCENE_06                 0x15
#define BALLAST_SCENE_07                 0x16
#define BALLAST_SCENE_08                 0x17
#define BALLAST_SCENE_09                 0x18
#define BALLAST_SCENE_10                 0x19
#define BALLAST_SCENE_11                 0x1A
#define BALLAST_SCENE_12                 0x1B
#define BALLAST_SCENE_13                 0x1C
#define BALLAST_SCENE_14                 0x1D
#define BALLAST_SCENE_15                 0x1E
#define BALLAST_SCENE_16                 0x1F
#define BALLAST_STATUS_INFORMATION       0x20
#define BALLAST_VERSION_NUMBER           0x21
#define BALLAST_PHYSICAL_MIN_LEVEL       0x22

#define BALLAST_SLAVE_OFFSET             0x23

#define TRUE                      1
#define FALSE                     0

//DALI states
#define NO_ACTION                        0
#define SENDING_DATA                     1
#define RECEIVING_DATA                   2
#define ERR                              3
#define SETTLING_FF_TO_BF                4
#define SETTLING_FF_TO_FF                5
#define BACKWARD_FRAME_RECEIVED          6
#define FORWARD_FRAME_SENT               7
#define SETTLING_FF_TO_BF_FINISHED       8
#define SETTLING_FF_TO_FF_FINISHED       9
#define WAIT_FOR_BACKCHANNEL_TO_RECEIVE  10

#define BACKWARD_FRAME_SENT              11
#define FORWARD_FRAME_RECEIVED           12

#define FADE_NONE        0
#define FADE_UP          1
#define FADE_DOWN        2

extern unsigned char dali_state;

extern sfr sbit _OUT_LINE;
extern sfr sbit _IN_LINE;

//UI
extern volatile unsigned char redrawUI;

//encoded dali cmd data. Ready for send
extern volatile unsigned char dali_array_cmd[17]; //regular DALI command
extern volatile unsigned char dali_array_receive_buffer[9];

//for slave device
extern volatile unsigned char dali_slave_array_receive_buffer[17];
extern volatile unsigned char dali_slave_array_response[9];

extern volatile unsigned char fadeBorder;
extern volatile unsigned char count_fade_steps;

extern volatile unsigned char dali_slave_answer;

typedef struct LightObject
{
  unsigned char _address;
  unsigned char _on;
  unsigned char _arcLevel;
  unsigned char _group0_7;
  unsigned char _group8_15;
  unsigned char _to_fade;
}
LightObjectType;

typedef struct LightCommandStruct
{
  unsigned char _ballastAddress;
  unsigned char _command;
  unsigned char _typeOfCommand;
  unsigned char _followingType;
}
LightCommandType;

//set settling flag status
//extern volatile unsigned char settling_state;
extern volatile unsigned char expect_backchannel;
//slave device
extern volatile unsigned char expected_response;
extern volatile unsigned char wait_for_second_cmd;      //used when slave device needs to wait for the second same command to arrive
/////
extern volatile unsigned char actual_val;
extern volatile unsigned char former_val;

/////
extern volatile unsigned char slave_addr_byte_received;
extern volatile unsigned char slave_cmd_byte_received;

//initialilse DALI slave device; all ballast
void DALI_Slave_Init();

//
void DALI_Slave_Prepare_Answer();

//initialise slave mem space
void DALI_Slave_Mem_Init(void);
//pwm init
void DALI_Slave_PWM_TIM1_Init(void);
void DALI_Slave_PWM_TIM3_Init(void);
void DALI_Slave_PWM_TIM8_Init(void);

//send cmd to ballast
unsigned char DALI_Send_Cmd(unsigned char ballastAddr, unsigned char cmd,
                            unsigned char typeOfCmd, unsigned char followingType);
                            
unsigned char DALI_Send_Cmd_New(unsigned char ballastAddr, unsigned char cmd, unsigned char followingCmd);

unsigned char DALI_Check_Special_Cmd(unsigned char addrByte);

unsigned char DALI_Get_Ballast_Answer(void);

void DALI_Slave_Sending_Data(void);
void DALI_Slave_Receiving_Data(void);
void DALI_Slave_Check_Received_Cmd(unsigned char command);

void DALI_Slave_Fade_PWM(void);

unsigned char DALI_Slave_Exeecute_Command(unsigned char addrByte, unsigned char cmdByte);

void DALI_Slave_Cmd_OFF(unsigned char addrByte);
void DALI_Slave_Cmd_DOWN(unsigned char addrByte);
void DALI_Slave_Cmd_STEP_UP(unsigned char addrByte);
void DALI_Slave_Cmd_STEP_DOWN(unsigned char addrByte);
void DALI_Slave_Cmd_RECALL_MAX_LEVEL(unsigned char addrByte);
void DALI_Slave_Cmd_RECALL_MIN_LEVEL(unsigned char addrByte);
void DALI_Slave_Cmd_STEP_DOWN_AND_OFF(unsigned char addrByte);
void DALI_Slave_Cmd_ON_AND_STEP_UP(unsigned char addrByte);
void DALI_Slave_Cmd_GO_TO_SCENE(unsigned char addrByte, unsigned char scene);
void DALI_Slave_Cmd_SET_ARC_LEVEL(unsigned char addrByte, unsigned char arcLevel);
void PrepareDataToSend(unsigned char *commandArray, unsigned char *tx_array,
                 unsigned char butesInCmd);
void PrepareAddressByte(unsigned char *commandArray, unsigned char addressType,
                        unsigned char byteAddressPosition, unsigned char followingType);

//unsigned char DALI_Master_Status(void);
unsigned char DALI_Slave_Status(void);
void DALI_Slave_Fade_Step_PWM(void);
#endif