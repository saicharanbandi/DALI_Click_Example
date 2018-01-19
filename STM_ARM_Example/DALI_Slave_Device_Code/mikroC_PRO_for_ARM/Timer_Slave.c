#include <Timer_Slave.h>
#include <DALI_Slave_Pub.h>

volatile unsigned int tick_count;
volatile unsigned char bit_count;

extern volatile unsigned char fadeBorder;

extern volatile unsigned char dali_slave_array_response[9];
extern volatile unsigned char dali_slave_array_receive_buffer[17];

extern volatile unsigned char count_fade_steps;

extern sfr sbit _OUT_LINE;
extern sfr sbit _IN_LINE;

/*******************************************************************************
Function:       Timer_DALI_Init

Description:    Initialise Timer for DALI network


Note:           Timer 2 is in use
*******************************************************************************/

void Timer_DALI_Init()
{
  //reset out/in line pins
  _OUT_LINE = 1;
  _IN_LINE  = 1;

  //reset tick and bit counters
  tick_count = 0;
  bit_count = 0;

  //timer 2 register settings
  //interrupt time ~ 104us
  RCC_APB1ENRbits.TIM2EN = 1;
  TIM2_CR1bits.CEN = 0;
  TIM2_PSC = 2900;
  TIM2_ARR = 5;
  NVIC_IntEnable(IVT_INT_TIM2);
  TIM2_DIERbits.UIE = 1;

}

/*******************************************************************************
Function:       Timer_DALI_Fade_Init()
Description:    Stop timer if it is active.

*******************************************************************************/

void Timer_DALI_Fade_Init()
{
  //timer 4 register settings
  //interrupt time ~ 104us
  RCC_APB1ENRbits.TIM4EN = 1;
  TIM4_CR1bits.CEN = 0;
  TIM4_PSC = 580;
  TIM4_ARR = 5;
  NVIC_IntEnable(IVT_INT_TIM4);
  TIM4_DIERbits.UIE = 1;
}


/*******************************************************************************
Function:       Timer4_interrupt
Description:    Interrupt function for timer 2

*******************************************************************************/

void Timer4_interrupt() iv IVT_INT_TIM4 {

  unsigned char i;
  volatile unsigned char pulsePosition;

  //reset interrupt flag
  TIM4_SRbits.UIF = 0;

  if(count_fade_steps < fadeBorder)
  {
    DALI_Slave_Fade_Step_PWM();
    
    count_fade_steps++;
  }
  else
  {
    //reset count steps value
    count_fade_steps = 0;
    //turn off timer
    Timer_Fade_Stop();
  }
}

/*******************************************************************************
Function:       Timer_Fade_Stop
Description:    Stop timer if it is active.

*******************************************************************************/

void Timer_Fade_Stop(void)
{
  if(TIM2_CR1bits.CEN == 1)
    TIM2_CR1bits.CEN = 0;
}


/*******************************************************************************
Function:       Timer_Fade_Start
Description:    Start timer if it is stopped

*******************************************************************************/

void Timer_Fade_Start(void)
{
  if(TIM4_CR1bits.CEN == 0)
    TIM4_CR1bits.CEN = 1;

  //reset tick
  count_fade_steps = 0;
}


/*******************************************************************************
Function:       Timer_Stop
Description:    Stop timer if it is active.

*******************************************************************************/

void Timer_Stop(void)
{
  if(TIM2_CR1bits.CEN == 1)
    TIM2_CR1bits.CEN = 0;
}


/*******************************************************************************
Function:       Timer_Start
Description:    Start timer if it is stopped

*******************************************************************************/

void Timer_Start(void)
{
  if(TIM2_CR1bits.CEN == 0)
    TIM2_CR1bits.CEN = 1;
   
  //reset tick and bit counters
  tick_count = 0;
  bit_count = 0;
}

/*******************************************************************************
Function:       Timer_Set_Settling_Time
Description:    Check and set dali state for settling time.

Parameters:     settlingTimeType - type of the settling time

*******************************************************************************/

void Timer_Set_Settling_Time(unsigned char settlingTimeType)
{
  //check if timer is OFF
  if(TIM2_CR1bits.CEN == 0)
    TIM2_CR1bits.CEN = 1;
  
  //check if backward frame was sent
  if(dali_state == BACKWARD_FRAME_SENT)
  {
    //reset timer tick counter
    tick_count = 0;
    
    //set type of settling time
    //forward frame -> backward frame
    if (settlingTimeType == SETTLING_FF_TO_BF)
      dali_state = SETTLING_FF_TO_BF;
  }
}

/*******************************************************************************
Function:       Timer2_interrupt
Description:    Interrupt function for timer 2

*******************************************************************************/

void Timer2_interrupt() iv IVT_INT_TIM2 {

  unsigned char i;
  volatile unsigned char pulsePosition;

  //reset interrupt flag
  TIM2_SRbits.UIF = 0;
  
  //if dali state is not idle
  if(dali_state == NO_ACTION)
  {
    former_val = actual_val;
    actual_val = _IN_LINE;
  
    if(former_val != actual_val)
    {
      //set state to RECEIVING
      dali_state = RECEIVING_DATA;
      tick_count = 0;
      bit_count  = 0; //add start bit
    }
  }
  else
  {
    //check if data is sending
    if(dali_state == SENDING_DATA)
    {
      DALI_Slave_Sending_Data();
    }
    //receiving state - ready and receiving backward frame - 11 bits
    if(dali_state == RECEIVING_DATA)
    {
      //check and receive data from IN line
      DALI_Slave_Receiving_Data();
    }
  }
  
  //settling time between forward -> backward ~ 7 to 22Te
  //master device sent a message, and that message is received
  if(dali_state == SETTLING_FF_TO_BF)
  {
    //settling time finished (7Te = 28 IT)
    if(tick_count > 28) // -16
      dali_state = SETTLING_FF_TO_BF_FINISHED;
    tick_count++;
  }
}