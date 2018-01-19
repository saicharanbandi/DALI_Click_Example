#include <DALI-pub.h>
#include <Timer.h>
#include <DALI_defs.h>

unsigned char *ptrAddr;

unsigned char dali_state;

volatile unsigned char dali_array_cmd[17];
volatile unsigned char dali_array_receive_buffer[9];

volatile unsigned char expect_backchannel;
volatile unsigned char expected_response;

volatile unsigned char tmpg;

unsigned char actual_val;
unsigned char former_val;

volatile LightObjectType lightLeds[10];

/*******************************************************************************
Function:       DALI_Init
Description:    Initialise DALI network

Note:           Main function used for DALI initialisation.
*******************************************************************************/

void DALI_init(void)
{
  //set start/default values for flags
  dali_state = NO_ACTION;

  //Initialise Timer module
  Timer_DALI_Init();
  //Initialise memory space and default values
  DALI_Mem_Init();
}

/*******************************************************************************
Function:       DALI_Mem_Init
Description:    Initialise memory space in SRAM where ballasts data will be stored

NOTE:           Initialise memory for ballasts and groups. To change start location
                change variable ADDR_BALLASR_MEM, defined in DALI_pub.h
*******************************************************************************/

void DALI_Mem_Init(void)
{
  unsigned char i, tmp;
  
  //set pointer to the start location of memory space
  ptrAddr = ADDR_BALLAST_MEM;
  //loop through all 64 ballasts
  for( i = 1; i < 65; i++){
    //reset address value
    tmp = 0;
    //shift address value to left -> YAAA AAAS 0-63
    //A - represents address bits
    tmp = i << 1;
    //write address value to memory
    *ptrAddr = tmp;
    //increment pointer location
    ptrAddr++;
    //reset pointer value - write default value for ballast data
    *ptrAddr = 0;
    //increment pointer location for next iteration
    ptrAddr++;
  }
  //loop through all 16 groups
  for ( i = 1; i < 17; i++){
    //reset group value
    tmp = 0;
    //shift address value to left -> YAAA AAAS
    //A - represents address bits
    tmp = i << 1;
    //set correct format for group address -> 100A AAAS 0-15
    tmp = GROUP_ADDRESS + tmp;
    //write address value to memory
    *ptrAddr = tmp;
    //increment pointer location
    ptrAddr++;
    //write default value for group data
    *ptrAddr = 0;
    //increment pointer location for next iteration
    ptrAddr++;
  }

}
/*******************************************************************************
Function:       DALI_READ
Description:    Read DALI data from memory

Inputs:         * addrToRead - Address of ballast which is going to be read

Output:         stored value
*******************************************************************************/
unsigned char DALI_Read(unsigned char addrToRead)
{
  unsigned char loc;
  //set pointer to start location in memory for ballast data
  ptrAddr = ADDR_BALLAST_MEM;
  //find location for specific ballast
  loc = addrToRead * 2 + 1;
  //set pointer to that location
  ptrAddr += loc;
  //return stored data
  return *ptrAddr;
}
/*******************************************************************************
Function:       DALI_Write
Description:    Write DALI data to memory

Inputs:         * addrToWrite - Address of the ballast
                * dataToWrite - Data to write
*******************************************************************************/

void DALI_Write(unsigned char addrToWrite, unsigned char dataToWrite)
{
  volatile unsigned char loc;
  //set pointer to start location in memory for ballast data
  ptrAddr = ADDR_BALLAST_MEM;
  //find location for specific ballast
  loc = addrToWrite * 2 + 1;
  //set pointer to that location
  ptrAddr += loc;
  //write data
  *ptrAddr = dataToWrite;
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
  
  ////write datballastAddrto memory address
  DALI_Write(ballastAddr, data_array[1]);

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
Function:       DALI_Receiving_Data
Description:    Check status of IN line and write to array

Note:           Manchester encoding

*******************************************************************************/

void DALI_Receiving_Data(void)
{
  unsigned char pulsePosition;
  //backward frame - 9 bits to receive - last 2 don't change phase
  //first bit is start bit (1), ignore, also last 2 bits are stop bits
  //FF - BF settlling time 7Te - 22Te (2Te = 8 interrupt intervals)
  //when change on pin is detected, tick_count is restarted.

  if(tick_count == (bit_count * 8 + 2))
  {
    if(_IN_LINE == 1)
      dali_array_receive_buffer[bit_count] = 0;
    else
      dali_array_receive_buffer[bit_count] = 1;
  }

  //increment ticks
  tick_count++;

  if((tick_count + 1) % 8 == 0)
  {
    bit_count++;
  }
  //transfer completed
  if(bit_count > 8)
  {
    //set dali state
    dali_state = BACKWARD_FRAME_RECEIVED;
  }
}

/*******************************************************************************
Function:       DALI_Sending_Data
Description:    Check status of IN line and write to array

Note:           Manchester encoding

*******************************************************************************/

void DALI_Sending_Data(void)
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
  if(bit_count < 17)
  {
    if(tick_count % 4 == 0)
    {
      pulsePosition = tick_count / 4;
      if(pulsePosition % 2 == 0)
      {
        if(dali_array_cmd[bit_count] == DALI_START_BIT_PULSE)
          _OUT_LINE = 1;
        else
          _OUT_LINE = 0;
      }
      else
      {
        if(dali_array_cmd[bit_count] == DALI_START_BIT_PULSE)
          _OUT_LINE = 0;
        else
          _OUT_LINE = 1;
      }
    }
  }
  tick_count++;
  
  if(tick_count % 8 == 0)
    bit_count++;
    
  if(bit_count > 16)
  {
    dali_state = FORWARD_FRAME_SENT;
    _OUT_LINE = 1;
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
Function:       DALI_Master_Status
Description:    DALI master device main loop

Output:         Return DALI state

*******************************************************************************/

unsigned char DALI_Master_Status(void)
{
  unsigned char i;

  if(dali_state == NO_ACTION)
    {
      //idle state
      //reset variables
      tick_count = 0;
      bit_count  = 0;
    }
    if(dali_state == SENDING_DATA)
    {
      //sending commands

    }
    //data can be received only if master device send a command
    if(dali_state == RECEIVING_DATA)
    {
      //backchannel
      //check for backchannel timeout; used for NO answer

    }
    //forward frame sent. Check for settling time
    if(dali_state == FORWARD_FRAME_SENT)
    {
      tick_count = 0;
      bit_count  = 0;

      //dali_cmd_repeat_time--;
      _OUT_LINE = 1;
      
      //set settling time
      if(expect_backchannel)
        dali_state = SETTLING_FF_TO_BF;
      else
        dali_state = SETTLING_FF_TO_FF;
    }

    //backward frame received. Set settling state
    if(dali_state == BACKWARD_FRAME_RECEIVED)
    {
      //backward frame full received
      //check for settling time
      //settling_state = SETTLING_FF_TO_FF; //FF -> FF & BF -> FF
      //dali_state = SETTLING_FF_TO_FF;

      dali_state = NO_ACTION;
      expect_backchannel = 0;
    }

    //check if settling is finished
    if(dali_state == SETTLING_FF_TO_FF_FINISHED || dali_state == SETTLING_FF_TO_BF_FINISHED)
    {
      dali_state = NO_ACTION;

      if(expect_backchannel)
      {
        dali_state = WAIT_FOR_BACKCHANNEL_TO_RECEIVE;
        
        former_val = 1;
        actual_val = 1;
      }
    }
    //if we wait for backchannel, check _IN_LINE
    if(dali_state == WAIT_FOR_BACKCHANNEL_TO_RECEIVE)
    {
      former_val = actual_val;
      actual_val = _IN_LINE;
      
      if(former_val != actual_val)
      {
        tick_count = 0;
        bit_count  = 0; //add start bit
        dali_state = RECEIVING_DATA;
      }
    }
    
    //error part
    if(dali_state == ERR)
    {
      _OUT_LINE  = 1;
      _IN_LINE   = 1;
    }
    
    return dali_state;
}
