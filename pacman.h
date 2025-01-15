#ifndef __PM_H
#define __PM_H

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "../constants.h"
#include "../map.h"
#include <stdio.h>
#include "./blinky.h"

#define KEEP 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct pacman_t{
	int x, y, dir;
} pacman_t;

extern pacman_t p;
extern volatile int score, eatp, life, tonewlife, victory, draw;
extern volatile uint8_t b_status;

void initPacman();
void movePacman();
void stopPacman();
void restorePacman();
void setScore(int x, int y);

#endif