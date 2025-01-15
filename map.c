#include "map.h"

uint8_t mat[R][C]={
{1,1,1, 1,1,1, 1,1,1, 5,5,5, 1,0,1, 5,5,5, 1,1,1, 1,1,1, 1,1,1},
{1,0,0, 0,0,0, 0,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,0, 0,1,0, 0,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,0, 1,0,1},
{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 0,1,1, 1,0,1},
{1,0,1, 1,1,0, 0,0,1, 0,0,0, 0,0,0, 0,0,0, 0,0,1, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,1, 1,1,1, 1,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,5, 5,5,5, 5,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,5, 5,5,5, 5,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,0, 0,0,0, 1,0,0, 0,1,5, 5,5,5, 5,1,0, 1,0,0, 0,1,0, 0,0,1},
{1,1,1, 1,1,0, 1,1,1, 0,7,5, 5,5,5, 5,1,0, 1,1,1, 0,1,1, 1,0,1},
{1,0,0, 0,0,0, 1,0,0, 0,1,5, 5,5,5, 5,1,0, 1,0,0, 0,1,0, 0,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,5, 5,5,5, 5,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,5, 5,5,5, 5,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 0,1,1, 1,1,1, 1,1,0, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 0,0,1, 0,0,0, 0,0,0, 0,0,0, 0,0,1, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,1,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 0,1,1, 1,0,1},
{1,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 1,1,1, 1,0,1, 1,1,1, 1,0,1, 1,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,0,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,1,0, 1,0,1},
{1,0,1, 1,1,0, 1,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,1, 0,1,0, 1,0,1},
{1,0,0, 0,0,0, 0,0,1, 5,5,5, 1,0,1, 5,5,5, 1,0,0, 0,1,0, 0,0,1},
{1,1,1, 1,1,1, 1,1,1, 5,5,5, 1,0,1, 5,5,5, 1,1,1, 1,1,1, 1,1,1}
};

volatile int xoffset=(SWIDTH-R*CSIZE)/2;				//right offset = (screen width - game width)/2
//static int yoffset=(SHEIGHT-C*CSIZE)/2;			//left offset = (screen heigth - game heigth)/2
volatile int xborder=SWIDTH-(SWIDTH-R*CSIZE)/2;
volatile int xcenter=SWIDTH/2;
volatile int ycenter=SHEIGHT/2;
volatile uint8_t x_door=-1, y_door=-1, open=1;

void coordinates(int x, int y, int* xpix, int* ypix){
	*xpix=x*CSIZE+xoffset;
	*ypix=y*CSIZE+xoffset;
}
void eraseCell(int x, int y){
	int xpix, ypix, i;
	mat[x][y]=EMPTY;
	coordinates(x, y, &xpix, &ypix);
	for(i=1; i<8; i++)
		LCD_DrawLine(xpix+1, ypix+i, xpix+7, ypix+i, Black);
}
void eraseText(int r, int c){
	GUI_Text(r, c, (uint8_t *) "      ", White, Black);
}
void drawWall(int x, int y, uint16_t color){
	int xpix, ypix;
	mat[x][y]=WALL;
	coordinates(x, y, &xpix, &ypix);
	if(y==0 || mat[x][y-1]!=1){
		LCD_DrawLine(xpix, ypix, xpix+CSIZE, ypix, color);						//upper row
	}
	if(y>=C-1 || mat[x][y+1]!=1){
		LCD_DrawLine(xpix, ypix+CSIZE, xpix+CSIZE, ypix+CSIZE, color);//lower row
	}
	if(x==0 || mat[x-1][y]!=1){
		LCD_DrawLine(xpix, ypix, xpix, ypix+CSIZE, color);						//left col
	}
	if(x>=R-1 || mat[x+1][y]!=1){
		LCD_DrawLine(xpix+CSIZE, ypix, xpix+CSIZE, ypix+CSIZE, color);//right col
	}
}
void drawStandardPill(int x, int y, uint16_t color){
	int xpix, ypix;
	mat[x][y]=SPILL;
	coordinates(x, y, &xpix, &ypix);
	LCD_DrawLine(xpix+4, ypix+4, xpix+4, ypix+4, color);
	//LCD_DrawLine(xpix+3, ypix+3, xpix+5, ypix+3, color);
	//LCD_DrawLine(xpix+3, ypix+4, xpix+5, ypix+4, color);
	//LCD_DrawLine(xpix+3, ypix+5, xpix+5, ypix+5, color);
}
void drawPowerPill(int x, int y, uint16_t color){
	int xpix, ypix;
	mat[x][y]=PPILL;
	coordinates(x, y, &xpix, &ypix);
	LCD_DrawLine(xpix+3, ypix+2, xpix+5, ypix+2, color);
	LCD_DrawLine(xpix+2, ypix+3, xpix+6, ypix+3, color);
	LCD_DrawLine(xpix+2, ypix+4, xpix+6, ypix+4, color);
	LCD_DrawLine(xpix+2, ypix+5, xpix+6, ypix+5, color);
	LCD_DrawLine(xpix+3, ypix+6, xpix+5, ypix+6, color);
}
void drawPacman(int x, int y){
	int xpix, ypix;
	mat[x][y]=PACMAN;
	coordinates(x, y, &xpix, &ypix);//!!!!
	LCD_DrawLine(xpix+3, ypix+1, xpix+5, ypix+1, Yellow);
	LCD_DrawLine(xpix+2, ypix+2, xpix+6, ypix+2, Yellow);
	LCD_DrawLine(xpix+1, ypix+3, xpix+7, ypix+3, Yellow);
	LCD_DrawLine(xpix+1, ypix+4, xpix+7, ypix+4, Yellow);
	LCD_DrawLine(xpix+1, ypix+5, xpix+7, ypix+5, Yellow);
	LCD_DrawLine(xpix+2, ypix+6, xpix+6, ypix+6, Yellow);
	LCD_DrawLine(xpix+3, ypix+7, xpix+5, ypix+7, Yellow);
}
void drawMap(){
	int i, j;
		LCD_Clear(Black);
	GUI_Text(xoffset, 250, (uint8_t *) "LIVES: 1", White, Black);
	GUI_Text(xoffset, 280, (uint8_t *) "SCORE: 0", White, Black);
	GUI_Text(xborder-8*8, 250, (uint8_t *) "TIME: 60", White, Black);
	for(i=0; i<R; i++)
	  for(j=0; j<C; j++){
			switch(mat[i][j]){
				case EMPTY:  break;	
				case WALL:   drawWall(i, j, Blue);
 									   break;
				case SPILL:  drawStandardPill(i, j, White);
										 break;
				case PPILL:	 drawPowerPill(i, j, White);
										 break;
				case PACMAN: drawPacman(i, j);
										 break;
				case OUTMAP: break;
				case BLINKY: drawBlinky(i, j, CHASE);
										 break;
				case DOOR:   drawDoor(i, j);
										 x_door=i;
										 y_door=j;
										 break;
			  default: {};
			}
		}	
}
void spawnStandardPills(){
	volatile unsigned int seed;
	uint8_t ppills[NPP];
	volatile int i, j, np=0, npp=0, n, step;
	
	seed = LPC_TIM2->TC;//timer 2
	srand(seed);
	disable_timer(2);
	
	step=NSP/NPP;
	
	for(i=0; i<NPP; i++){
		n=rand()%step+step*i;
		ppills[i]=n;
	}
			
	//upper
	for(i=0; i<R && np<NSP; i++){
			for(j=0; j<C && np<NSP; j++){
				if(mat[j][i]==0){
					if(npp < 6 && np==ppills[npp]){
						drawPowerPill(j, i, White);
						npp++;
					} else {
						drawStandardPill(j, i, White);
					}
					np++;
				}
			}
	}
}
void spawnPowerPills(){
	int numero = rand() % 11;
}
void initTimer(){
	init_timer(0,25000000*1);						//cicli = T*F = 25 MHz*60s
	enable_timer(0);
}
void drawPause(){
	eraseText(xborder-5*8,280);
	GUI_Text(xborder-5*8, 280, (uint8_t *) "PAUSE", Blue, Yellow);
}
void erasePause(){
	eraseText(xborder-5*8,280);
}
void drawVictory(){
	LCD_Clear(Black);
	GUI_Text(xcenter-8*4, ycenter, (uint8_t *) "VICTORY!", Green, Black);
}
void drawGOver(){
	LCD_Clear(Black);
	GUI_Text(xcenter-10*4, ycenter, (uint8_t *) "GAME OVER!", Red, Black);
}
void drawTimer(int timer){
	char buf[20+1];
	sprintf(buf, "%d", timer);
	eraseText(xborder-2*8,250);
	GUI_Text(xborder-2*8, 250, (uint8_t *) buf, White, Black);
}
void drawScore(int score){
	char buf[20+1];
	sprintf(buf, "%d", score);
	eraseText(xoffset+7*8,280);
	GUI_Text(xoffset+7*8, 280, (uint8_t *) buf, White, Black);
}
void drawLife(int life){
	char buf[20+1];
	sprintf(buf, "%d", life);
	eraseText(xoffset+7*8,250);
	GUI_Text(xoffset+7*8, 250, (uint8_t *) buf, White, Black);
}
volatile uint8_t prev=EMPTY;
void drawBlinky(int x, int y, uint8_t status){
	int xpix, ypix;
	volatile uint16_t color;
	if(mat[x][y]!=BLINKY){//Evita riscrittura nello stato FRIGHT
		prev=mat[x][y];//Salva l'oggetto "sotto" a Blinky
	} else if(mat[x][y]==PACMAN){//Per sicurezza
		prev=EMPTY;
	}
	mat[x][y]=BLINKY;
	if(status == CHASE)
		color=Red;
	else if(status == FRIGHT)
		color=Blue2;
	coordinates(x, y, &xpix, &ypix);
	//Corpo
	LCD_DrawLine(xpix+3, ypix+1, xpix+5, ypix+1, color);
	LCD_DrawLine(xpix+2, ypix+2, xpix+6, ypix+2, color);
	LCD_DrawLine(xpix+1, ypix+3, xpix+7, ypix+3, color);
	LCD_DrawLine(xpix+1, ypix+4, xpix+7, ypix+4, color);
	LCD_DrawLine(xpix+1, ypix+5, xpix+7, ypix+5, color);
	LCD_DrawLine(xpix+1, ypix+6, xpix+7, ypix+6, color);
	LCD_DrawLine(xpix+1, ypix+7, xpix+7, ypix+7, color);
	//Occhi
	LCD_DrawLine(xpix+3, ypix+3, xpix+3, ypix+3, White);
	LCD_DrawLine(xpix+3, ypix+4, xpix+3, ypix+4, Black);
	LCD_DrawLine(xpix+5, ypix+3, xpix+5, ypix+3, White);
	LCD_DrawLine(xpix+5, ypix+4, xpix+5, ypix+4, Black);
	//"Base"
	LCD_DrawLine(xpix+2, ypix+7, xpix+2, ypix+7, Black);
	LCD_DrawLine(xpix+4, ypix+7, xpix+4, ypix+7, Black);
	LCD_DrawLine(xpix+6, ypix+7, xpix+6, ypix+7, Black);
}
//Cancella la cella evitando che Blinky sovrascriva gli oggetti sul suo percorso
void eraseCellBlinky(int x, int y){
	volatile int i=0;
	if(mat[x][y]==DOOR)//Vietato cancellare le porte
		return;
	eraseCell(x, y);
	mat[x][y]=prev;
	switch(mat[x][y]){
		case EMPTY:  break;
		case SPILL:  drawStandardPill(x, y, White);
								 break;
		case PPILL:	 drawPowerPill(x, y, White);
								 break;
		case PACMAN: break;
		default: {};
	}
}
void drawDoor(int x, int y){
	int xpix, ypix;
	mat[x][y]=DOOR;
	coordinates(x, y, &xpix, &ypix);
	LCD_DrawLine(xpix+1, ypix+4, xpix+7, ypix+4, White);
	open=0;
}
void eraseDoor(){
	eraseCell(x_door, y_door);
	open=1;
}