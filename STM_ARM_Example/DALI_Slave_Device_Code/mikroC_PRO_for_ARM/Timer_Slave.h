/**************************************************************************************************
* File: Timer_Slave.h
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
extern volatile unsigned char bit_count;
extern volatile unsigned char tick_count;

extern volatile unsigned char fadeBorder;

void Timer_DALI_Init(void);

void Timer_DALI_Fade_Init(void);

void Timer_Fade_Start(void);
void Timer_Fade_Stop(void);

void Timer_Stop(void);
void Timer_Start(void);

void Timer_Set_Settling_Time(unsigned char settlingTimeType);