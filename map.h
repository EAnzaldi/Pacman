#ifndef __MAP_H
#define __MAP_H

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "../constants.h"
#include "timer/timer.h"
#include <stdio.h>

extern uint8_t mat[R][C];
//extern volatile int timer, score, life;
extern volatile uint8_t door_x, door_y, open;

void coordinates(int x, int y, int* xpix, int* ypix);
void drawWall(int x, int y, uint16_t color);
void drawStandardPill(int x, int y, uint16_t color);
void drawPowerPill(int x, int y, uint16_t color);
void drawPacman(int x, int y);
void drawMap();
void eraseCell(int x, int y);
void spawnStandardPills();
void spawnPowerPills();
void initTimer();
void eraseText(int r, int c);
void drawPause();
void erasePause();
void drawVictory();
void drawGOver();
void drawTimer(int timer);
void drawScore(int score);
void drawLife(int life);
void drawBlinky(int x, int y, uint8_t status);
void drawDoor(int x, int y);
void eraseDoor();

#endif
