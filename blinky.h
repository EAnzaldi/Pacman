#ifndef __BL_H
#define __BL_H

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "../constants.h"
#include "../map.h"
#include <stdio.h>
#include "./pacman.h"
#include <stdlib.h>

typedef struct blinky_t{
	int x, y;					 	//coordinate attuali
	int prev_x, prev_y;	//coordinate precedenti
	int spawned;
} blinky_t;

extern blinky_t b;
extern volatile uint8_t b_status;
extern volatile uint8_t x_door, y_door, open;

void initBlinky();
void moveBlinky();
void chase();
void flee();
void changeStatus(uint8_t status);
void scareBlinky();
int handleCollision();

#endif