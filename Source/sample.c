/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "../constants.h"
#include "../map.h"
#include "../pacman.h"
#include "CAN/CAN.h"
#include "RIT/RIT.h"
#include "adc/adc.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
										 
int main(void){
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();												/* BUTTON Initialization              */
	joystick_init();											/* Joystick Initialization            */
	LCD_Initialization();
	CAN_Init();
	ADC_init();

	init_RIT(100000000*1/5);							/* RIT Initialization 0.2 s      	    */
	init_timer(2,25000000*1);						//cicli = T*F = 25 MHz*1 s
	enable_timer(2);
	
	drawMap();
	initPacman();
	spawnStandardPills();
	initBlinky();
	enable_RIT();													/* RIT enabled												*/
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);			

	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);	
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
