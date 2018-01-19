#include <DALI_Slave_Pub.h>
#include <DALI_defs.h>
#include <DALI_Slave_Defs.h>
#include <Timer_Slave.h>

// Initialise all 64 Ballasts with offset in a memory.
unsigned char *ptrAddr;

unsigned char dali_state;

volatile unsigned char dali_array_cmd[17];
volatile unsigned char dali_array_receive_buffer[9];

////volatile unsigned char settling_state;

volatile unsigned char expect_backchannel;
volatile unsigned char expected_response;

volatile unsigned char tmpg;

//for slave device
volatile unsigned char dali_slave_array_receive_buffer[17];
volatile unsigned char dali_slave_array_response[9];

volatile unsigned char dali_slave_answer;

volatile unsigned char slave_addr_byte_received;
volatile unsigned char slave_cmd_byte_received;

volatile unsigned char fadeBorder;
volatile unsigned char count_fade_steps;

//
volatile unsigned char redrawUI;

//
unsigned char actual_val;
unsigned char former_val;

volatile LightObjectType lightLeds[10];

void DALI_Slave_Group_Cmd(unsigned char addrByte, LightObjectType *ptrLight);

unsigned char DALI_Slave_getAddressFromByte(unsigned char addressByte);
void DALI_Slave_WriteArcValue(unsigned char ballastAddress, unsigned char arcValue);

void DALI_Slave_Set_Fade(LightObjectType *ptrLights, unsigned char addrByte, unsigned char fadeTypeDir);

void DALI_Slave_TurnOff_Ballast(LightObjectType *lightObjs, unsigned char ObjId);
void DALI_Slave_Set_Arc_Level(LightObjectType *lightObjs, unsigned char ObjId,
                                unsigned char valueType, unsigned char ledOn);
void DALI_Slave_Set_Arc_Level_Direct(LightObjectType *lightObjs, unsigned char ObjId, unsigned char arcValue);
unsigned char DALI_Slave_Has_Group(LightObjectType *lightObjs, unsigned char ObjId, unsigned char addrByte);


/*******************************************************************************
Function:       DALI_Slave_Fade_PWM
Description:    Fade led for 200ms

*******************************************************************************/

void DALI_Slave_Fade_PWM(void)
{
  Timer_DALI_Fade_Init();
}

/*******************************************************************************
Function:       DALI_Slave_Fade_Step_PWM
Description:    Fade leds for one step. Called from timer interrupt.

*******************************************************************************/

void DALI_Slave_Fade_Step_PWM()
{
  unsigned char i;

  for (i = 0; i < 10; i++)
  {
    if(lightLeds[i]._to_fade != FADE_NONE)
    {
      //fade up for 1 step; watch for max arc level value
      if(lightLeds[i]._to_fade == FADE_UP && lightLeds[i]._arcLevel < 254)
      {
        lightLeds[i]._arcLevel += 1;
      }
      //fade down for 1 step; watch for min arc level value
      if(lightLeds[i]._to_fade == FADE_DOWN && lightLeds[i]._arcLevel > 1)
        lightLeds[i]._arcLevel -= 1;
    }
  }
}


/*******************************************************************************
Function:       DALI_Slave_Init
Description:    Initialise slave device; all ballasts etc

*******************************************************************************/

void DALI_Slave_Init(void)
{
  unsigned char i;
  //set default value for state flag
  dali_state = NO_ACTION;
  //initialise Timer module
  Timer_DALI_Init();
  //initialise memory space and default values
  DALI_Slave_Mem_Init();
  
  former_val = 1;
  actual_val = 1;
  ptrAddr = ADDR_BALLAST_MEM;
  //inicialisation of leds on system board
  for(i = 0; i < 10; i++)
  {
    lightLeds[i]._address = i;
    lightLeds[i]._on = TRUE;
    lightLeds[i]._arcLevel = 254;
    lightLeds[i]._group0_7 = *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_GROUP_0_7);
    lightLeds[i]._group8_15 = *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_GROUP_8_15);
    lightLeds[i]._to_fade  = FADE_NONE;
  }
}

/*******************************************************************************
Function:       DALI_Slave_Mem_Init
Description:    Initialise memory space in SRAM where ballasts data will be stored

NOTE:           Initialise memory for ballasts.
*******************************************************************************/

void DALI_Slave_Mem_Init(void)
{
  unsigned char i;

  ptrAddr = ADDR_BALLAST_MEM;
  //for every ballast. Default values by protocol
  for(i = 0; i < 64; i++)
  {
    //write short address to memory
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SHORT_ADDRESS)         = i;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_ACTUAL_DIM_LEVEL)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_POWER_ON_LEVEL)        = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SYSTEM_FAILURE_LEVEL)  = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_MIN_LEVEL)             = 1;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_MAX_LEVEL)             = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_FADE_RATE)             = 7;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_FADE_TIME)             = 1;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SEARCH_ADDRESS_H)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SEARCH_ADDRESS_M)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SEARCH_ADDRESS_L)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_RANDOM_ADDRESS_H)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_RANDOM_ADDRESS_M)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_RANDOM_ADDRESS_L)      = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_GROUP_0_7)             = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_GROUP_8_15)            = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_01)              = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_02)              = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_03)              = 190;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_04)              = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_05)              = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_06)              = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_07)              = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_08)              = 80;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_09)              = 190;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_10)              = 200;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_11)              = 50;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_12)              = 30;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_13)              = 254;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_14)              = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_15)              = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_SCENE_16)              = 100;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_STATUS_INFORMATION)    = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_VERSION_NUMBER)        = 0;
    *(ptrAddr + BALLAST_SLAVE_OFFSET * i + BALLAST_PHYSICAL_MIN_LEVEL)    = 1;
  }
  //only for example
  *(ptrAddr + BALLAST_GROUP_0_7)                                          = 0x2D;
  *(ptrAddr + BALLAST_SLAVE_OFFSET + BALLAST_GROUP_0_7)                   = 0x35;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 2 + BALLAST_GROUP_0_7)               = 0x11;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 3 + BALLAST_GROUP_0_7)               = 0x15;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 4 + BALLAST_GROUP_0_7)               = 0x29;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 5 + BALLAST_GROUP_0_7)               = 0x2D;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 6 + BALLAST_GROUP_0_7)               = 0x2A;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 7 + BALLAST_GROUP_0_7)               = 0x2A;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 8 + BALLAST_GROUP_0_7)               = 0x2A;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * 9 + BALLAST_GROUP_0_7)               = 0x3A;
}

/******************************************************************************
Function:       DALI_Send_Cmd
Description:    Send DALI command over DALI protocol

Inputs:         * ballastAddr - Address of the ballast (dimmer)

                * cmd         - Command which is going to be sent (see DALI_defs.h for list
                              of commands)
                              
                * typeOfCmd   - It's used to define type of address:
                                - BROADCAST_DIRECT
                                - BROADCAST_CMD
                                - SHORT_ADDRESS
                                - GROUP_ADDRESS
                              
                * followingType - Status of the last bit in address byte.
                                  - FOLLOWING_DIRECT_ARC_POWER_LVL
                                  - FOLLOWING_COMMAND
                                  
Output:      TRUE
******************************************************************************/
unsigned char DALI_Send_Cmd(unsigned char ballastAddr, unsigned char cmd,
                            unsigned char typeOfCmd, unsigned char followingType)
{
  unsigned char data_array[2];
  unsigned char i;

  //how much time the command will be repeated - for test
  //dali_cmd_repeat_time = 3;
  
  //set output pin to 0
  _OUT_LINE = 1;
  
  tick_count = 0;
  bit_count  = 0;
  //set DALI state to send data
  dali_state = SENDING_DATA;
  //fetch ballast address and command
  data_array[0] = (char)ballastAddr;
  data_array[1] = (char)cmd;
  
  //reset dali_array_cmd values
  for (i = 0; i < 17; i++)         //16
    dali_array_cmd[i] = 0;
  
  //prepare address byte to be sent
  PrepareAddressByte(data_array, typeOfCmd, 0, followingType);
  
  //encode data - Manchester encoding
  PrepareDataToSend(data_array, dali_array_cmd, 2);

  //check type of command
  //set backchannel
  if((cmd >= 0x00) && (cmd <= 0x1F)) //Indirect arc power control commands
  {
    expect_backchannel   = FALSE;
  }
  if((cmd >= 20) && (cmd <= 0x80)) //Configurations commands
  {
    expect_backchannel = FALSE;
  }
  if((cmd >= 0x90))                //Query commands
  {
    expect_backchannel = TRUE;     //set status to expect Backchannel. Posible answer:
                                   //1111 1111                             - YES
                                   //no response; no ba1ckchannel received  - NO
                                   //8bit info                             - 8 bit
  }
  //check for special command
  if(DALI_Check_Special_Cmd(data_array[0]))
  {
    expect_backchannel = TRUE;
    //
    if(data_array[0] == TERMINATE_H_BITS || data_array[0] == DTR)
      expect_backchannel = FALSE;
    else if(data_array[0] == VERIFY_SHORT_ADDRESS || data_array[0] == QUERY_SHORT_ADDRESS_H)
      expect_backchannel = TRUE;
    else
      expect_backchannel = FALSE;
    

  }

  //start timer
  Timer_Start();
  
  return TRUE;
}

unsigned char DALI_Check_Special_Cmd(unsigned char addrByte)
{
  volatile unsigned char addrToCheck;
  
  addrToCheck = addrByte;    //get address byte
  if ((addrToCheck == 0x90) || (addrToCheck == 0xA0)) //check for 1010 or 1011
  {
    if(addrToCheck & 0x01) //LSB must be 1
      return TRUE;
    else
      return FALSE;
  }
  else
  {
    return FALSE;
  }
}

/*******************************************************************************
Function:       DALI_Slave_Receiving_Data
Description:    Check status of IN line and write to array

Note:           Manchester encoding

*******************************************************************************/

void DALI_Slave_Receiving_Data(void)
{
  unsigned char pulsePosition;
  //forward frame - 17 bits to receive - last 2 don't change phase
  //first bit is start bit (1), ignore, also last 2 bits are stop bits
  //FF - BF settlling time 7Te - 22Te (2Te = 8 interrupt intervals)
  //when change on pin is detected, tick_count is restarted.

    if(tick_count == (bit_count * 8 + 2))
    {
      if(_IN_LINE == 1)
        dali_slave_array_receive_buffer[bit_count] = 0;
      else
        dali_slave_array_receive_buffer[bit_count] = 1;
    }

  //increment ticks
  tick_count++;

  if(tick_count % 8 == 0)
    bit_count++;

  //transfer completed
  if(bit_count > 16)
  {
    //set dali state
    dali_state = FORWARD_FRAME_RECEIVED;
  }
}

/*******************************************************************************
Function:       DALI_Slave_Sending_Data
Description:    Check status of IN line and send data to master device

Note:           Manchester encoding

*******************************************************************************/

void DALI_Slave_Sending_Data(void)
{
  unsigned char pulsePosition;

  if(tick_count < 8)
  {
    if(tick_count < 4)
      _OUT_LINE = 0;
    else
      _OUT_LINE = 1;
  }
  else
  if(bit_count < 9)
  {
    if(tick_count % 4 == 0)
    {
      pulsePosition = tick_count / 4;
      if(pulsePosition % 2 == 0)
      {
        if(dali_slave_array_response[bit_count] == DALI_START_BIT_PULSE)
          _OUT_LINE = 1;
        else
          _OUT_LINE = 0;
      }
      else
      {
        if(dali_slave_array_response[bit_count] == DALI_START_BIT_PULSE)
          _OUT_LINE = 0;
        else
          _OUT_LINE = 1;
      }
    }
  }
  tick_count++;

  if(tick_count % 8 == 0)
    bit_count++;

  if(bit_count > 8)
  {
    dali_state = BACKWARD_FRAME_SENT;
    _OUT_LINE  = 1;
    _IN_LINE   = 1;
  }
}

/*******************************************************************************
Function:       PrepareDataToSend
Description:    Prepare command array to be encoded and create new array where
                every element is a bit.

Parameters:     * commandArray - Array of bytes values

                * tx_array     - Return array. Each element represents bit state
                
                * bytesInCmd   - Number of bytes in command array


Note:           Manchester encoding

*******************************************************************************/

void PrepareDataToSend(unsigned char *commandArray, unsigned char *tx_array, 
                       unsigned char bytesInCmd)
{
  //set default valur for the mask
  unsigned char mask = 0x80;
  //variable which hold one byte value - one element from commandArray
  unsigned char dummy;
  //number of bytes in command
  unsigned char bytes_counter;
  unsigned char i;
  //number of active bit
  unsigned char bitCounter;
  //set default value
  bitCounter = 0;
  
  for (i = 0; i < 9; i++)
  {
    tx_array[0] = 0;
  }
  
  //loop through all bytes in commandArray
  for(bytes_counter = 0; bytes_counter < bytesInCmd; bytes_counter++)
  {
    //assign byte for use
    dummy = commandArray[bytes_counter];
    //set mask to default value
    mask = 0x80;
    //increment number of active bit
    bitCounter++;

    //check if active bit is the first one
    if(bitCounter == 1)
    {
      //start bit is always 1 - in manchester that is END_BIT_PULSE
      tx_array[0] = DALI_END_BIT_PULSE;
    }
    //2 byte command
    //go through all bytes and use Manchester
    for(i = 1; i < 9; i++) //1 & 9
    {
      //check if bit is one
      if(dummy & mask)
      {
        //assign pulse value - manchester
        tx_array[i + (8 * bytes_counter)] = DALI_END_BIT_PULSE;
      }
      else
      {
        //assign pulse value - manchester
        tx_array[i + (8 * bytes_counter)] = DALI_START_BIT_PULSE;
      }
      //check mask value
      if(mask == 0x01)
        mask <<= 7;     //shift mask bit to MSB
      else
        mask >>= 1;     //shift mask bit to 1 right
    }
  }
  //tx_array[17] = DALI_END_BIT_PULSE;
  
  //add 2 stop bits at the end
/*for (i = 1; i < 3; i++)
  {
    //assign pulse value - manchester
    tx_array[16 + i] = DALI_END_BIT_PULSE;
  }*/
}

/*******************************************************************************
Function:       DALI_Slave_PWM_Set_Duty
Description:    Set 1 step fade

*******************************************************************************/

void DALI_Slave_PWM_Set_Duty(unsigned char arclevel, unsigned char addressBallast)
{
  unsigned int currentDuty;
  
  currentDuty = arclevel;
  
  switch(addressBallast)
  {
    case 0 : {
              PWM_TIM1_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL1);
              break;
             }
    case 1 : {
              PWM_TIM1_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL2);
              break;
             }
    case 2 : {
              PWM_TIM1_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL3);
              break;
             }
    case 3 : {
              PWM_TIM1_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL4);
              break;
             }
    case 4 : {
              PWM_TIM3_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL1);
              break;
             }
    case 5 : {
              PWM_TIM3_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL2);
              break;
             }
    case 6 : {
              PWM_TIM3_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL3);
              break;
             }
    case 7 : {
              PWM_TIM3_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL4);
              break;
             }
    case 8 : {
              PWM_TIM8_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL1);
              break;
             }
    case 9 : {
              PWM_TIM8_Set_Duty(currentDuty, _PWM_NON_INVERTED, _PWM_CHANNEL2);
              break;
             }
  }
}


/*******************************************************************************
Function:       DALI_Slave_PWM_TIM1_Init
Description:    Initialisa PWM module

*******************************************************************************/

void DALI_Slave_PWM_TIM1_Init(void)
{
  PWM_TIM1_Init(656250);  //256 steps
  
  PWM_TIM1_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL4);
  PWM_TIM1_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL3);
  PWM_TIM1_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL2);
  PWM_TIM1_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL1);
  PWM_TIM1_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM1_CH1_PE9);
  PWM_TIM1_Start(_PWM_CHANNEL2, &_GPIO_MODULE_TIM1_CH2_PA9);
  PWM_TIM1_Start(_PWM_CHANNEL3, &_GPIO_MODULE_TIM1_CH3_PA10);
  PWM_TIM1_Start(_PWM_CHANNEL4, &_GPIO_MODULE_TIM1_CH4_PE14);
}

/*******************************************************************************
Function:       DALI_Slave_PWM_TIM3_Init
Description:    Initialisa PWM module

*******************************************************************************/

void DALI_Slave_PWM_TIM3_Init(void)
{
  PWM_TIM3_Init(656250);  //256 steps

  PWM_TIM3_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL4);
  PWM_TIM3_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL3);
  PWM_TIM3_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL2);
  PWM_TIM3_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL1);
  PWM_TIM3_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM3_CH1_PA6);
  PWM_TIM3_Start(_PWM_CHANNEL2, &_GPIO_MODULE_TIM3_CH2_PB5);
  PWM_TIM3_Start(_PWM_CHANNEL3, &_GPIO_MODULE_TIM3_CH3_PC8);
  PWM_TIM3_Start(_PWM_CHANNEL4, &_GPIO_MODULE_TIM3_CH4_PC9);
}

/*******************************************************************************
Function:       DALI_Slave_PWM_TIM3_Init
Description:    Initialisa PWM module

*******************************************************************************/

void DALI_Slave_PWM_TIM8_Init(void)
{
  PWM_TIM8_Init(656250);  //256 steps

  PWM_TIM8_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL1);
  PWM_TIM8_Set_Duty(250, _PWM_NON_INVERTED, _PWM_CHANNEL2);
  PWM_TIM8_Start(_PWM_CHANNEL1, &_GPIO_MODULE_TIM8_CH1_PC6);
  PWM_TIM8_Start(_PWM_CHANNEL2, &_GPIO_MODULE_TIM8_CH2_PC7);
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_OFF
Description:    Turn off ballasts.

*******************************************************************************/

void DALI_Slave_Cmd_OFF(unsigned char addrByte)
{
unsigned char i;
  unsigned char addrBallast;

  if(addrByte == BROADCAST_CMD)
  {
    for(i = 0; i < 10; i++)
    {
      DALI_Slave_TurnOff_Ballast(lightLeds, i);
    }
  }
  else if ((addrByte & 0x80))
  {
    for (i = 0; i < 10; i++)
    {
      if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
      {
        DALI_Slave_TurnOff_Ballast(lightLeds, i);
      }
    }
  }
  else
  {
    //get address
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    DALI_Slave_TurnOff_Ballast(lightLeds, addrBallast);
  }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_RECALL_MIN_LEVEL
Description:    Set the actual arc power level to the ‘MIN LEVEL’ without fading.
                If the lamp is off it shall be ignited with this command.

*******************************************************************************/

void DALI_Slave_Cmd_RECALL_MIN_LEVEL(unsigned char addrByte)
{
  unsigned char i;
  unsigned char addrBallast;

  ptrAddr = ADDR_BALLAST_MEM;

  if(addrByte == BROADCAST_CMD)
  {
    for(i = 0; i < 10; i++)
    {
      DALI_Slave_Set_Arc_Level(lightLeds, i, BALLAST_MIN_LEVEL, TRUE);
      DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
    }
  }
   else if((addrbyte & 0x80))
  {
    for (i=0; i < 10; i++)
    {
      if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
      {
        DALI_Slave_Set_Arc_Level(lightLeds, i, BALLAST_MIN_LEVEL, TRUE);
        DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
      }
    }
  }
  else
  {
    //get address
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    DALI_Slave_Set_Arc_Level(lightLeds, addrBallast, BALLAST_MIN_LEVEL, TRUE);
    //pwm to arc level
    DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
  }
}

/*******************************************************************************
Function:       DALI_Slave_Group_Cmd
Description:    Execute command on ballast with specific group

*******************************************************************************/
void DALI_Slave_Group_Cmd(unsigned char addrByte, LightObjectType *ptrLight)
{
unsigned char groupAddress;
    unsigned char i;

    groupAddress = addrByte << 3;
    groupAddress = groupAddress >> 4;

    for(i = 0; i < 10; i++)
    {
       if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
       {
         DALI_Slave_Set_Arc_Level(ptrLight, i, BALLAST_MAX_LEVEL, TRUE);
       }
       else
         DALI_Slave_TurnOff_Ballast(ptrLight, i);
    }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_RECALL_MAX_LEVEL
Description:    Set the actual arc power level to the ‘MAX LEVEL’ without fading.
                If the lamp is off it shall be ignited with this command.

*******************************************************************************/

void DALI_Slave_Cmd_RECALL_MAX_LEVEL(unsigned char addrByte)
{
  unsigned char i;
  unsigned char addrBallast;

  ptrAddr = ADDR_BALLAST_MEM;

  if(addrByte == BROADCAST_CMD)
  {
    for(i = 0; i < 10; i++)
    {
      DALI_Slave_Set_Arc_Level(lightLeds, i, BALLAST_MAX_LEVEL, TRUE);
      DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
    }
  }
  //group command
  else if ((addrByte & GROUP_ADDRESS) == 0x80)
  {
    for (i=0; i < 10; i++)
    {
      if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
      {
        DALI_Slave_Set_Arc_Level(lightLeds, i, BALLAST_MAX_LEVEL, TRUE);
        DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
      }
    }
  }
  else
  {
    //get address
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    DALI_Slave_Set_Arc_Level(lightLeds, addrBallast, BALLAST_MAX_LEVEL, TRUE);
    //pwm
    DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
  }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_GO_TO_SCENE
Description:    Set the actual arc power level to the value stored for scene XXXX
                using the actual fade time. If the ballast does not belong to scene
                XXXX, the arc power level remain unchanged. If the lamp is off it
                shall be ignited with this command.

*******************************************************************************/

void DALI_Slave_Cmd_GO_TO_SCENE(unsigned char addrByte, unsigned char scene)
{
  unsigned char i;
  unsigned char addrBallast;
  unsigned char sceneLevel;

  ptrAddr = ADDR_BALLAST_MEM;
  
  //get scene from byte
  sceneLevel = scene & 0x0F;
  
  if(addrByte == BROADCAST_CMD)
  {
    for (i = 0; i < 10; i++)
    {
      //set arc level to scene level
      DALI_Slave_Set_Arc_Level(lightLeds, i, (BALLAST_SCENE_01 + sceneLevel), TRUE);
      DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
    }
  }
  else if ((addrByte & GROUP_ADDRESS) == 0x80)
  {
    for (i=0; i < 10; i++)
    {
      if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
      {
        DALI_Slave_Set_Arc_Level(lightLeds, i, (BALLAST_SCENE_01 + sceneLevel), TRUE);
        DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
      }
    }
  }
  else
  {
     //get address
     addrBallast = DALI_Slave_getAddressFromByte(addrByte);
     DALI_Slave_Set_Arc_Level(lightLeds, addrBallast, (BALLAST_SCENE_01 + sceneLevel), TRUE);
     DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
  }
}

/*******************************************************************************
Function:       DALI_Slave_getAddressFromByte
Description:    Get the address from address byte.

*******************************************************************************/

unsigned char DALI_Slave_getAddressFromByte(unsigned char addressByte)
{
  unsigned char addressToReturn;
  
  addressToReturn = addressByte << 1;
  addressToReturn = addressToReturn >> 2;
  
  return addressToReturn;
}

/*******************************************************************************
Function:       DALI_Slave_WriteArcValue
Description:    Write arc value to memory for specific led

*******************************************************************************/

void DALI_Slave_WriteArcValue(unsigned char ballastAddress, unsigned char arcValue)
{
  ptrAddr = ADDR_BALLAST_MEM;
  *(ptrAddr + BALLAST_SLAVE_OFFSET * ballastAddress + BALLAST_ACTUAL_DIM_LEVEL) = arcValue;
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_STEP_UP
Description:    Set the actual arc power level one step higher immediately
                without fading.

*******************************************************************************/

void DALI_Slave_Cmd_STEP_UP(unsigned char addrByte)
{
  unsigned char i;
  unsigned char addrBallast;
  unsigned char stepUpValue;
  
  ptrAddr = ADDR_BALLAST_MEM;
  
  if(addrByte == BROADCAST_CMD || (addrByte & GROUP_ADDRESS))
  {
    for (i = 0; i < 10; i++)
    {
      //only active lights
      if (lightLeds[i]._on)
      {
        //get FADE RATE
        stepUpValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_FADE_RATE);
        //check max level
        if((lightLeds[i]._arcLevel + stepUpValue) < *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MAX_LEVEL))
        {
          //add one step
          lightLeds[i]._arcLevel += stepUpValue;
        }
        else
        {
          //set max level
          lightLeds[i]._arcLevel = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MAX_LEVEL);
        }
        //write back to memory
        DALI_Slave_WriteArcValue(lightLeds[i]._address, lightLeds[i]._arcLevel);

        if(addrByte == BROADCAST_CMD)
          DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);

      }
    }
    if(((addrByte & GROUP_ADDRESS) == 0x80) && (addrByte != BROADCAST_CMD) && (addrByte != BROADCAST_DIRECT))
      DALI_Slave_Group_Cmd(addrByte, lightLeds);
  }
  else
  {
    //only active light
      addrBallast = DALI_Slave_getAddressFromByte(addrByte);
      if (lightLeds[addrBallast]._on)
      {
        //get FADE RATE
        stepUpValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_FADE_RATE);
        if((lightLeds[addrBallast]._arcLevel + stepUpValue) < *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_MAX_LEVEL))
        {
          //add one step
          lightLeds[addrBallast]._arcLevel += stepUpValue;
        }
        else
        {
          //set maximum arc level
          lightLeds[addrBallast]._arcLevel = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_MAX_LEVEL);
        }
        //write to memory
        DALI_Slave_WriteArcValue(lightLeds[addrBallast]._address, lightLeds[addrBallast]._arcLevel);
        //pwm deo
        DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
      }
  }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_STEP_DOWN
Description:    Set the actual arc power level one step lower immediately without
                fading.

*******************************************************************************/

void DALI_Slave_Cmd_STEP_DOWN(unsigned char addrByte)
{
  unsigned char i;
  unsigned char addrBallast;
  unsigned char stepDownValue;

  ptrAddr = ADDR_BALLAST_MEM;

  if(addrByte == BROADCAST_CMD || (addrByte & GROUP_ADDRESS))
  {
    for (i = 0; i < 16; i++)
    {
      //only active lights
      if (lightLeds[i]._on)
      {
        //get FADE RATE
        stepDownValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_FADE_RATE);
        //check max level
        if((lightLeds[i]._arcLevel - stepDownValue) > *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MIN_LEVEL))
        {
          //one step down
          lightLeds[i]._arcLevel -= stepDownValue;
        }
        else
        {
          //set min level
          lightLeds[i]._arcLevel = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MIN_LEVEL);
        }
        //write back to memory
        DALI_Slave_WriteArcValue(lightLeds[i]._address, lightLeds[i]._arcLevel);
        //pwm
        if(addrByte == BROADCAST_CMD)
          DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
      }
    }
    if(((addrByte & GROUP_ADDRESS) == 0x80) && (addrByte != BROADCAST_CMD) && (addrByte != BROADCAST_DIRECT))
      DALI_Slave_Group_Cmd(addrByte, lightLeds);
  }
  else
  {
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    
    if(lightLeds[addrBallast]._on)
    {
      //get FADE RATE
      stepDownValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_FADE_RATE);
      if((lightLeds[addrBallast]._arcLevel - stepDownValue) > *(ptrAddr + BALLAST_SLAVE_OFFSET * addrBallast + BALLAST_MIN_LEVEL))   //lightLeds[addrBallast]._address + BALLAST_MIN_LEVEL))
      {
        //one step down
        lightLeds[addrBallast]._arcLevel -= stepDownValue;
      }
      else
      {
        //set minimum arc level
        lightLeds[addrBallast]._arcLevel = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_MIN_LEVEL);
      }
      //write to memory
      DALI_Slave_WriteArcValue(lightLeds[addrBallast]._address, lightLeds[addrBallast]._arcLevel);
      //pwm deo
      DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
    }
  }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_DOWN
Description:    Dim down 200 ms using the selected FADE RATE

*******************************************************************************/

void DALI_Slave_Cmd_DOWN(unsigned char addrByte)
{
  fadeBorder = 45;
  DALI_Slave_Set_Fade(lightLeds, addrByte, FADE_DOWN);
  DALI_Slave_Fade_PWM();
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_UP
Description:    Dim up 200ms using selected fade rate

*******************************************************************************/
void DALI_Slave_Cmd_UP(unsigned char addrByte)
{
  fadeBorder = 45;
  DALI_Slave_Set_Fade(lightLeds, addrByte, FADE_UP);
  DALI_Slave_Fade_PWM();
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_STEP_DOWN_AND_OFF
Description:    Set the actual arc power level one step lower immediately without 
                fading. If the actual arc power level is already at the ‘MIN LEVEL’ 
                the lamp shall be switched off by this command.

*******************************************************************************/

void DALI_Slave_Cmd_STEP_DOWN_AND_OFF(unsigned char addrByte)
{
  unsigned char i;
  unsigned char addrBallast;
  unsigned char stepDownValue;
  unsigned char minArcValue;
  unsigned char mask;
  
  ptrAddr = ADDR_BALLAST_MEM;
  
  if(addrByte == BROADCAST_CMD || (addrByte & GROUP_ADDRESS))
  {
    for(i = 0; i < 10; i++)
    {
      stepDownValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_FADE_RATE);
      //get min arc value for ballast
      minArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MIN_LEVEL);
      //below min value, set to min
      if((lightLeds[i]._arcLevel - stepDownValue) < minArcValue)
      {
        lightLeds[i]._arcLevel = minArcValue;
      }
      else if(lightLeds[i]._arcLevel == minArcValue) //turn off light
      {
        lightLeds[i]._on = FALSE;
        lightLeds[i]._arcLevel = 0;
      }
      else if((lightLeds[i]._arcLevel - stepDownValue) > minArcValue)
      {
        //step down
        lightLeds[i]._arcLevel -= stepDownValue;
      }
      //write to memory
      DALI_Slave_WriteArcValue(lightLeds[i]._address, lightLeds[i]._arcLevel);
      if(lightLeds[i]._on)
      {
        //pwm
        if(addrByte == BROADCAST_CMD)
          DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, Lightleds[i]._address);
      }
    }
    if(((addrByte & GROUP_ADDRESS) == 0x80) && (addrByte != BROADCAST_CMD) && (addrByte != BROADCAST_DIRECT))
      DALI_Slave_Group_Cmd(addrByte, lightLeds);
  }
  else
  {
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    stepDownValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_FADE_RATE);
    //get min arc value for ballast
    minArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MIN_LEVEL);
    if((lightLeds[addrBallast]._arcLevel - stepDownValue) < minArcValue)
      {
        lightLeds[addrBallast]._arcLevel = minArcValue;
      }
      else if(lightLeds[addrBallast]._arcLevel == minArcValue) //turn off light
      {
        lightLeds[addrBallast]._on = FALSE;
        lightLeds[addrBallast]._arcLevel = 0;
      }
      else if((lightLeds[addrBallast]._arcLevel - stepDownValue) > minArcValue)
      {
        //step down
        lightLeds[addrBallast]._arcLevel -= stepDownValue;
      }
      //write to memory
      DALI_Slave_WriteArcValue(lightLeds[addrBallast]._address, lightLeds[addrBallast]._arcLevel);
      if(lightLeds[addrBallast]._on)
      {
        //pwm deo
        DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
      }
  }
}

/*******************************************************************************
Function:       DALI_Slave_Cmd_ON_AND_STEP_UP
Description:    Set the actual arc power level one step higher immediately without
                fading. If the lamp is switched off the lamp shall be ignited with 
                this command and shall be set to the ‘MIN LEVEL’.

*******************************************************************************/

void DALI_Slave_Cmd_ON_AND_STEP_UP(unsigned char addrByte)
{
  unsigned char i;
  unsigned char minArcValue;
  unsigned char maxArcValue;
  unsigned char stepUpValue;
  unsigned char addrBallast;
  
  ptrAddr = ADDR_BALLAST_MEM;
  
  if(addrByte == BROADCAST_CMD)
  {
    for(i = 0; i < 10; i++)
    {
      //get fade value
      stepUpValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_FADE_RATE);
      //get min arc level value
      minArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MIN_LEVEL);
      //get max arc level value
      maxArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[i]._address + BALLAST_MAX_LEVEL);
      //turn on light and set to min arc level
      if(lightLeds[i]._on == FALSE)
      {
        lightLeds[i]._on = TRUE;                     //turn on led
        lightLeds[i]._arcLevel = minArcValue;        //set arc level
      }
      else
      {
        if((lightLeds[i]._arcLevel + stepUpValue) < maxArcValue)
        {
          lightLeds[i]._arcLevel += stepUpValue;
        }
        else
        {
          lightLeds[i]._arcLevel = maxArcValue;
        }
      }
      //write to memory
      DALI_Slave_WriteArcValue(lightLeds[i]._address, lightLeds[i]._arcLevel);
      if(addrByte == BROADCAST_CMD)
        DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
    }
    //pwm deo
    if(((addrByte & GROUP_ADDRESS) == 0x80) && (addrByte != BROADCAST_CMD) && (addrByte != BROADCAST_DIRECT))
      DALI_Slave_Group_Cmd(addrByte, lightLeds);

  }
  else if((addrByte & 0x80) == 0)
  {
    //get address
    addrBallast = DALI_Slave_getAddressFromByte(addrByte);
    //get fade value
    stepUpValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_FADE_RATE);
    //get min arc level value
    minArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_MIN_LEVEL);
    //get max arc level value
    maxArcValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightLeds[addrBallast]._address + BALLAST_MAX_LEVEL);
    
    if(lightLeds[i]._on == FALSE)
    {
      lightLeds[i]._on = TRUE;
      lightLeds[i]._arcLevel = minArcValue;
    }
    else
    {
      if((lightLeds[i]._arcLevel + stepUpValue) < maxArcValue)
      {
        lightLeds[addrBallast]._arcLevel += stepUpValue;
      }
      else
      {
        lightLeds[addrBallast]._arcLevel = maxArcValue;
      }
    }
    //write to memory
    DALI_Slave_WriteArcValue(lightLeds[addrBallast]._address, lightLeds[addrBallast]._arcLevel);
    //pwm
    DALI_Slave_PWM_Set_Duty(lightLeds[addrBallast]._arcLevel, lightLeds[addrBallast]._address);
  }
}

/*******************************************************************************
Function:       DALI_Slave_Execute_command
Description:    Prepare ballast data to send or execute specific no-response command.

Parameters:     * addrByte - address byte from master device

                * cmdByte  - command byte from master device
                

*******************************************************************************/

unsigned char DALI_Slave_Execute_Command(unsigned char addrByte, unsigned char cmdByte)
{
  unsigned char i;
  //ballast number from address byte
  unsigned char numberOfBallast;
  //value from memory
  unsigned char returnData;
  volatile unsigned char addr;
  addr = addrByte;
  //get ballast number
  numberOfBallast = addrByte << 1;
  numberOfBallast = numberOfBallast >> 2;
  //start location for pointer - location of SHORT ADDRESS
  ptrAddr = ADDR_BALLAST_MEM;

  //redraw UI
  redrawUI = TRUE;

  //direct arc level command - short address, arc level following
  if((addr & FOLLOWING_COMMAND) == 0)
  {
    //short address
    if((addrByte & GROUP_ADDRESS) == 0)
    {
      DALI_Slave_Set_Arc_Level_Direct(lightLeds, numberOfBallast, cmdByte);
      DALI_Slave_PWM_Set_Duty(lightLeds[numberOfBallast]._arcLevel, lightLeds[numberOfBallast]._address);
    }
    else                    //group address
    {
      for (i = 0; i < 10; i++)
      {
        DALI_Slave_TurnOff_Ballast(lightLeds, i);
        if(DALI_Slave_Has_Group(lightLeds, i, addrByte))
        {
          DALI_Slave_Set_Arc_Level_Direct(lightLeds, i, cmdByte);
          DALI_Slave_PWM_Set_Duty(lightLeds[i]._arcLevel, lightLeds[i]._address);
        }
      }
    }
  }
  else  //other commands, queries etc.
  {
    switch(cmdByte)
    {
    //OFF
    case OFF : {
                  DALI_Slave_Cmd_OFF(addrByte);
                  return 0;
                  break;
                }
    //UP
    case UP  : {
                  DALI_Slave_Cmd_UP(addrByte);
                  return 0;
                  break;
               }
    //DOWN
    case DOWN : {
                  DALI_Slave_Cmd_DOWN(addrByte);
                  return 0;
                  break;
                }
    //STEP UP
    case STEP_UP : {
                     DALI_Slave_Cmd_STEP_UP(addrByte);
                     return 0;
                     break;
                   }
    //STEP DOWN
    case STEP_DOWN : {
                       DALI_Slave_Cmd_STEP_DOWN(addrByte);
                       return 0;
                       break;
                     }
    //RECALL MAX LEVEL
    case RECALL_MAX_LEVEL : {
                              DALI_Slave_Cmd_RECALL_MAX_LEVEL(addrByte);
                              return 0;
                              break;
                            }
    //RECALL MIN LEVEL
    case RECALL_MIN_LEVEL : {
                              DALI_Slave_Cmd_RECALL_MIN_LEVEL(addrByte);
                              return 0;
                              break;
                            }
    //STEP DOWN AND OFF
    case STEP_DOWN_AND_OFF : {
                               DALI_Slave_Cmd_STEP_DOWN_AND_OFF(addrByte);
                               return 0;
                               break;
                             }
    //ON AND STEP UP
    case ON_AND_STEP_UP : {
                            DALI_Slave_Cmd_ON_AND_STEP_UP(addrByte);
                            return 0;
                            break;
                          }
    //GO TO SCENE
    case GO_TO_SCENE01 :
    case GO_TO_SCENE02 :
    case GO_TO_SCENE03 :
    case GO_TO_SCENE04 :
    case GO_TO_SCENE05 :
    case GO_TO_SCENE06 :
    case GO_TO_SCENE07 :
    case GO_TO_SCENE08 :
    case GO_TO_SCENE09 :
    case GO_TO_SCENE10 :
    case GO_TO_SCENE11 :
    case GO_TO_SCENE12 :
    case GO_TO_SCENE13 :
    case GO_TO_SCENE14 :
    case GO_TO_SCENE15 :
    case GO_TO_SCENE16 : {
                           DALI_Slave_Cmd_GO_TO_SCENE(addrByte, cmdByte);
                           return 0;
                           break;
                         }
    //Query Status
    case 0x90 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_STATUS_INFORMATION);
                  break;
                }
    //Query Ballast
    case 0x91 : {
                  returnData = 255; //Ask if there is a ballast with the given address that is able to communicate. 255 - YES
                  break;
                }
    //Query Lamp Failure
    case 0x92 : {

                  break;
                }
    //Query Lamp Power On
    case 0x93 : {

                  break;
                }
    //Query Limit Error
    case 0x94 : {

                  break;
                }
    //Query Reset State
    case 0x95 : {

                  break;
                }
    //Query Missing Short Address
    case 0x96 : {

                  break;
                }
    //Query Version Number
    case 0x97 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_VERSION_NUMBER);
                  break;
                }
    //Query Content DTR
    case 0x98 : {

                  break;
                }
    //Query Device Type
    case 0x99 : {

                  break;
                }
    //Query Physical Minimum Level
    case 0x9A : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_PHYSICAL_MIN_LEVEL);
                  break;
                }
    //Query Power Failure
    case 0x9B : {

                  break;

                }
    //Query Actual Level
    case 0xA0 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_ACTUAL_DIM_LEVEL);
                  break;
                }
    //Query Max Level
    case 0xA1 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_MAX_LEVEL);
                  break;
                }
    //Query Min Level
    case 0xA2 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_MIN_LEVEL);
                  break;
                }
    //Query Power On Level
    case 0xA3 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_POWER_ON_LEVEL);
                  break;
                }
    //Query System Failure Level
    case 0xA4 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_SYSTEM_FAILURE_LEVEL);
                  break;
                }
    //Query Fade Time / Fade Rate
    case 0xA5 : {

                  break;
                }
    //Query Scene Level
    case 0xB0 :
    case 0xB1 :
    case 0xB2 :
    case 0xB3 :
    case 0xB4 :
    case 0xB5 :
    case 0xB6 :
    case 0xB7 :
    case 0xB8 :
    case 0xB9 :
    case 0xBA :
    case 0xBB :
    case 0xBC :
    case 0xBD :
    case 0xBE :
    case 0xBF : {
                  unsigned char tmp;
                  //get first 4 bits - scene number
                  tmp = (cmdByte & 0x0F);
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_SCENE_01 + tmp);
                  break;
                }
    //Query Groups 0-7
    case 0xC0 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_GROUP_0_7);
                  break;
                }
    //Query Groups 8-15
    case 0xC1 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_GROUP_8_15);
                  break;
                }
    //Query Random Address H
    case 0xC2 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_RANDOM_ADDRESS_H);
                  break;
                }
    //Query Random Address M
    case 0xC3 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_RANDOM_ADDRESS_M);
                  break;
                }
    //Query Random Address L
    case 0xC4 : {
                  returnData = *(ptrAddr + BALLAST_SLAVE_OFFSET * numberOfBallast + BALLAST_RANDOM_ADDRESS_L);
                  break;
                }

  }

  }
  
  return returnData;
}


/*******************************************************************************
Function:       PrepareAddressByte
Description:    Prepare address byte.

Parameters:     * commandArray - Array of bytes values

                * addressType  - It's used to define type of address:
                                 - BROADCAST_DIRECT
                                 - BROADCAST_CMD
                                 - SHORT_ADDRESS
                                 - GROUP_ADDRESS
                                 
                * byteAddressPosition - Index of element in array which holds address
                                        value

                * followingType       - value of the last bit in address byte. Defines
                                        if data byte holds command or direct arc value
                                        - FOLLOWING_DIRECT_ARC_POWER_LVL
                                        - FOLLOWING_COMMAND
*******************************************************************************/

void PrepareAddressByte(unsigned char *commandArray, unsigned char addressType,
                        unsigned char byteAddressPosition, unsigned char followingType)
{
  unsigned char addr_tmp;
  //broadcast command to all ballasts
  if(addressType == BROADCAST_CMD)
  {
    //set address byte to Broadcast command - value 0xFF
    commandArray[byteAddressPosition] = BROADCAST_CMD;
  }
  else
  {
    //fetch address value from array to operate
    addr_tmp = commandArray[byteAddressPosition];

    if (addressType == BROADCAST_DIRECT)
      //broadcast direct arc level to all ballasts - value 0xFE
      commandArray[byteAddressPosition] = BROADCAST_DIRECT;
    else
    {
      // ?? Why we need this shift ??
      // ?? What is FOLLOWING_COMMAND ??
      // ?? When the SHORT_ADDRESS will be allotted ?

      
      //shift address value for 1 to left
      addr_tmp <<= 1;
      
      //check if the command byte is following address byte
      if(followingType == FOLLOWING_COMMAND)
        //set LSB
        addr_tmp |= 0x01;
      //if it is a group address
      if (addressType == GROUP_ADDRESS)
        //add group value to address byte
        addr_tmp |= GROUP_ADDRESS;
      //assign return value
      commandArray[byteAddressPosition] = addr_tmp;
    }
  }
}

/*******************************************************************************
Function:       DALI_Get_Ballast_Answer
Description:    Encode and write received data. Check in dali_array_receive_buffer

Output:         Return ballast answer
                - YES  : 1111 1111
                - NO   : 0
                - 8bit : XXXX XXXX - 8bit value

*******************************************************************************/

unsigned char DALI_Get_Ballast_Answer(void)
{
  unsigned char i;
  unsigned char receivedData;
  
  for (i = 0; i < 8; i++)
  {
    //shift bit to the right position
    dali_array_receive_buffer[i] <<= i;
    //add bit to the received byte
    receivedData |= dali_array_receive_buffer[i];
  }
  //return received byte
  return receivedData;
}

/*******************************************************************************
Function:       DALI_Slave_Status
Description:    DALI slave device main loop

Output:         Return DALI state

*******************************************************************************/

unsigned char DALI_Slave_Status(void)
{
  if(dali_state == NO_ACTION)
  {
    //idle state
    //reset variables
    tick_count = 0;
    bit_count  = 0;

    former_val = actual_val;
    actual_val = _IN_LINE;

    if(former_val != actual_val)
    {
      tick_count = 0;
      bit_count  = 0; //add start bit
      dali_state = RECEIVING_DATA;
    }
  }
  
  if(dali_state == SENDING_DATA)
  {
    //sending commands

  }
  
  if(dali_state == RECEIVING_DATA)
  {
    //receiving data from master device

  }
  
  if(dali_state == FORWARD_FRAME_RECEIVED)
  {
      unsigned char i;
      volatile unsigned char anstosend[1];
      //reset address & cmd byte values
      slave_addr_byte_received = 0;
      slave_cmd_byte_received  = 0;

      //forward frame full received
      dali_state = SETTLING_FF_TO_BF;

      for (i = 1; i < 17; i++) //skip first bit - start bit
      {
        if(dali_slave_array_receive_buffer[i] == 1)
        {
          //address byte
          if(i < 9)
          {
            slave_addr_byte_received |= (1 << (8 - i));
          }
          //command byte
          else
          {
            slave_cmd_byte_received |= (1 << (7 - (i - 9)));
          }
        }
      }
      //check for response
      //DALI_Slave_Check_Received_cmd(slave_cmd_byte_received);
      
      //need to check return value
      dali_slave_answer = DALI_Slave_Execute_command(slave_addr_byte_received, slave_cmd_byte_received);
      anstosend[0] = dali_slave_answer;
      
      PrepareDataToSend(anstosend, dali_slave_array_response, 1);
  }
  
  if(dali_state == BACKWARD_FRAME_SENT)
  {
    tick_count = 0;
    bit_count  = 0;

    //dali_cmd_repeat_time--;
    _OUT_LINE = 1;
    _IN_LINE = 1;
    dali_state = NO_ACTION;
    //redrawUI = TRUE;
  }
  
  //check if settling is finished
  if(dali_state == SETTLING_FF_TO_BF_FINISHED)
  {
    dali_state = NO_ACTION;
    //does slave device need to response?
    if(slave_cmd_byte_received >= 0x90 && slave_cmd_byte_received <= 0xC4)
    {
      //set state to sending
      dali_state = SENDING_DATA;
      //expected_response = FALSE;
      tick_count = 0;
      bit_count  = 0;
    }
    former_val = 1;
    actual_val = 1;
  }
  //error occur
  if(dali_state == ERR)
  {
    _OUT_LINE  = 1;
    _IN_LINE   = 1;
  }
  
  return dali_state;
}

/*******************************************************************************
Function:       DALI_Slave_Check_Received_Cmd
Description:    DALI slave check for received command

*******************************************************************************/

void DALI_Slave_Check_Received_Cmd(unsigned char command)
{
  unsigned char i;
  
  expected_response = FALSE;
  
  //check if response is expected
  //query commands
  if((command >= 0x90 && command <= 0xC4))// || command == VERIFY_SHORT_ADDRESS || command == QUERY_SHORT_ADDRESS_H)
  {
    expected_response = TRUE;
  }
}

////////////////////////
/*******************************************************************************
Function:       DALI_Slave_Set_Fade
Description:    Prepare leds for fading

*************************************************************************/

void DALI_Slave_Set_Fade(LightObjectType *ptrLights, unsigned char addrByte, unsigned char fadeTypeDir)
{
 unsigned char i;
 unsigned char addrBallast;

  count_fade_steps = 0;
  for(i = 0; i < 10; i++)
    ptrLights[i]._to_fade = FADE_NONE;

  if(addrByte == BROADCAST_CMD)
  {
    for(i = 0; i < 10; i++)
    {
      ptrLights[i]._to_fade = fadeTypeDir;
    }
  }
  else if ((addrByte & GROUP_ADDRESS) == 0x80)
  {
    for (i=0; i < 10; i++)
    {
      if(DALI_Slave_Has_Group(ptrLights, i, addrByte))
      {
        ptrLights[i]._to_fade = fadeTypeDir;
      }
    }
  }
  else
  {
     //get address
     addrBallast = DALI_Slave_getAddressFromByte(addrByte);
     ptrLights[addrBallast]._to_fade = fadeTypeDir;
  }
}

unsigned char DALI_Slave_Has_Group(LightObjectType *lightObjs, unsigned char ObjId, unsigned char addrByte)
{
  unsigned char groupAddress;
  unsigned char tmpGroupValue;

  groupAddress = addrByte << 3;
  groupAddress = groupAddress >> 4;

  if(groupAddress < 8)
  {
    tmpGroupValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightObjs[ObjId]._address + BALLAST_GROUP_0_7);
    if(tmpGroupValue & (1 << groupAddress))
    {
      return TRUE;
    }
  }
  else
  {
    tmpGroupValue = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightObjs[ObjId]._address + BALLAST_GROUP_8_15);
    if(tmpGroupValue & (1 << (groupAddress - 8)))
    {
      return TRUE;
    }
  }
  return FALSE;
}

void DALI_Slave_Set_Arc_Level_Direct(LightObjectType *lightObjs, unsigned char ObjId, unsigned char arcValue)
{
  lightObjs[ObjId]._arcLevel = arcValue;
  DALI_Slave_WriteArcValue(lightObjs[ObjId]._address, lightObjs[ObjId]._arcLevel);
}

void DALI_Slave_Set_Arc_Level(LightObjectType *lightObjs, unsigned char ObjId,
                                unsigned char valueType, unsigned char ledOn)
{
  lightObjs[ObjId]._on = ledOn;
  lightObjs[ObjId]._arcLevel = *(ptrAddr + BALLAST_SLAVE_OFFSET * lightObjs[ObjId]._address + valueType);
  DALI_Slave_WriteArcValue(lightObjs[ObjId]._address, lightObjs[ObjId]._arcLevel);
}

void DALI_Slave_TurnOff_Ballast(LightObjectType *lightObjs, unsigned char ObjId)
{
  lightObjs[ObjId]._on = FALSE;
  lightObjs[ObjId]._arcLevel = 0;
  //for every ballast write value to Actual Dim Level
  DALI_Slave_WriteArcValue(lightObjs[ObjId]._address, lightObjs[ObjId]._arcLevel);
  DALI_Slave_PWM_Set_Duty(lightObjs[ObjId]._arcLevel, lightObjs[ObjId]._address);
}
