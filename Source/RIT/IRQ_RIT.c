/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../../pacman.h"
#include "../../map.h"
#include "../CAN/CAN.h"
#include "../timer/timer.h"
#include "../../blinky.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0, count=START*2, pause=0, timer=START, victory=0, draw=0, slowness=5, loss=0, first=1;
//meccanismo di temporizzazione: se tick_i == 0 --> evento
volatile int tick1=5, tick2=3, tick3=15, tick4=5, tick5=25, tick6=50, tick7=15, tick8=4;

extern volatile uint8_t b_status;

// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1


//SHORTENING UNDERTALE: TOO MANY REPETITIONS
NOTE song[] = 
{
	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{mus_pause, time_semicroma},
	{a3b, time_semicroma},
	{mus_pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{mus_pause, time_semicroma},
	{a3b, time_semicroma},
	{mus_pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{mus_pause, time_semicroma},
	{a3b, time_semicroma},
	{mus_pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{mus_pause, time_semicroma},
	{a3b, time_semicroma},
	{mus_pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},	
};

void RIT_IRQHandler (void)
{	
	draw=0;
	
	if(!pause){
		if(tick1==0){
			tick1=5;
			timer--;//decrementa il timer ogni secondo
			draw=1;
			//drawTimer();
			if(timer==0){
				pause=1;
				drawGOver();
				disable_RIT();//ferma il RIT
				return;
			}
		} else{
			tick1--;//aspetta
		}
	}	
	
	/* button management */
	if(down>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			switch(down){				
				case 2:				/* pay attention here: please see slides 19_ to understand value 2 */
				if(!pause){
					pause=1;//metto in pausa
					stopPacman();
					drawPause();
				} else {
					pause=0;//esco dalla pausa
					restorePacman();
					erasePause();
				}
					break;
				default:
					break;
			}
			down++;
		}
		else {	/* button released */
			down=0;			
			NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}
	
	if(!pause){
			/* joystick management */
			if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
				/* Joytick DOWN pressed */
				p.dir=UP;
			}
			if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
				/* Joytick LEFT pressed */
				p.dir=LEFT;
			}
			if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
				/* Joytick RIGHT pressed */
				p.dir=RIGHT;
			}
			if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
				/* Joytick UP pressed */
				p.dir=DOWN;
			}
		movePacman();//dopo 3 sec
		
		/* AI management */
		if(tick3==0 && !open && first){//Blinky parte dopo 3 sec
			eraseDoor();
			first=0;
		} else if(tick3>0){
			tick3--;
		}
		
		if(b.spawned==0){
			if(tick7==0){//Spawna Blinki dopo 3 sec
				tick7=15;
				initBlinky();
				eraseDoor();
			} else if (tick7>0){
				tick7--;
			}
		}
		
		if(b.spawned==1 && tick3==0){
			if(b_status==CHASE){//Velocità incrementale
				if(slowness>0){
					if(tick5==0){
						tick5=25;
						slowness--;//Incrementa la velocità ogni 5 sec fino a raggiungere quella di Pacman
					} else if(tick5>0)
						tick5--;
				}
				if(tick4==0){
					tick4=slowness;
					moveBlinky();//Blinky si muove ogni 0.2*slowness sec
				} else if(tick4>0)
					tick4--;
			} else if(b_status==FRIGHT){//Velocità fissa
				if(tick6==0)
					changeStatus(CHASE);//Dop 10 sec il fantasma ritorna nello stato CHASE
				else if(tick6>0)
					tick6--;
				if(tick8==0){
					tick8=4;
					moveBlinky();		
				} else if(tick8>0)
					tick8--;
			}	
		}
		
	}		
	
	if(!pause && draw){//disegna life, score e time solo se modificati
		//CAN
		volatile uint32_t message;
		volatile uint8_t buf[4];
		volatile int i;
	
		message = score | (life << 16) | (timer << 24);

		buf[0] = ((message >> 24) & 0xFF);
		buf[1] = ((message >> 16) & 0xFF);
		buf[2] = ((message >> 8) & 0xFF);
		buf[3] = (message & 0xFF);
  
		for(i=0; i<4; i++){
			CAN_TxMsg.data[i] = buf[i];
		}
		CAN_TxMsg.id = 2;
		CAN_TxMsg.len=4;
		CAN_TxMsg.format=STANDARD_FORMAT;
		CAN_TxMsg.type=DATA_FRAME;
	
		CAN_wrMsg (1, &CAN_TxMsg);               /* transmit message */
	}

	if(loss){
		drawGOver();
		disable_RIT();//ferma il RIT
		return;
	}
	
	if(life==0){
		loss=1;//al prossimo ciclo di RIT viene visualizzata la perdita	
		pause=1;
	}
	
	if(victory){
		drawVictory();
		disable_RIT();//ferma il RIT
	}
	
	if(!pause){
		static int currentNote = 0;
		static int ticks = 0;
		if(!isNotePlaying()){
			++ticks;
			if(ticks == UPTICKS){
				ticks = 0;
				playNote(song[currentNote++]);
			}
		}
	
		if(currentNote == (sizeof(song) / sizeof(song[0])))
			currentNote=0;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
