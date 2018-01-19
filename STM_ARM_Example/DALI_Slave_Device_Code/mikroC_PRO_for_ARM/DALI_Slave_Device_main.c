/*
 * Project name:
     DALI_Slave_Device.vtft
 * Generated by:
     Visual TFT
 * Date of creation
     01/11/2012
 * Time of creation
     15:09:56
 * Test configuration:
     MCU:             STM32F407VG
     Dev.Board:       EasyMx_PRO_v7_for_STM32_ARM
                      http://www.mikroe.com/eng/products/view/852/easymx-pro-v7-for-stm32/
                      
                      ac:DALI
     Oscillator:      168 Hz
     SW:              mikroC PRO for ARM
                      http://www.mikroe.com/eng/products/view/752/mikroc-pro-for-arm/
 */

/*
   Master            Leds Slave
----------------------------------
  Circle01              PE9
  Circle02              PA9
  Circle03              PA10
  Circle04              PE14
  Circle05              PA6
  Circle06              PB5
  Box01                 PC8
  Box02                 PC9
  Box03                 PC6
  Box04                 PC7
*/

// Overall there is not much to understand because it is just initialization of GPIOs, timers, network pins and UI 



// ?? Define exactly what the includes are ?? 


#include "DALI_Slave_Device_objects.h"
#include "DALI_Slave_Pub.h"
#include "DALI_defs.h"
#include "Timer_Slave.h"
#include "DALI_Slave_Defs.h"

// ?? I think this is the configuration of Tx, Rx pins for Visual TFT or the EasyMx_PRO_v7 board

sbit _OUT_LINE  at GPIOC_ODR.B2;            //Tx line
sbit _IN_LINE   at GPIOD_IDR.B10;           //Rx line

void main() {

  // ?? Here he is initializing the GPIO pins
  
 //set input/output pins for DALI protocol
  GPIO_Digital_Input (&GPIOD_BASE, _GPIO_PINMASK_10); // Set PORTD as digital input
  GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_2);

  // Not sure what exaclty the this DALI network is
  
  //initialise DALI network - slave device
  DALI_Slave_Init();

  Timer_Start();

  Start_TP();

  // ?? initialization of touch of UI
  redrawUI = FALSE;

  //init pwm
  DALI_Slave_PWM_TIM1_Init();
  DALI_Slave_PWM_TIM3_Init();
  DALI_Slave_PWM_TIM8_Init();

  while (1) {
    //Check_TP();

    if(redrawUI)
    {
      IntToStr(slave_addr_byte_received, lblCommandAddr.Caption);
      IntToStr(slave_cmd_byte_received, lblCommandCmd.Caption);
      //IntToStr(dali_slave_answer, lblAnswer.Caption);

      DrawBox(&lblCmdBoxClear);
      //DrawBox(&lblAnsBoxClear);
      DrawLabel(&lblCommandAddr);
      DrawLabel(&lblCommandCmd);
      //DrawLabel(&lblAnswer);
      redrawUI = FALSE;
    }

    dali_state = DALI_Slave_Status();
  }

}