#include "blinky.h"

blinky_t b;
volatile uint8_t b_status=CHASE;//stato di clinky (CHASE - FRIGHT)

NOTE eatblinky={d4, time_croma};

void initBlinky(){
	b.x=13;
	b.y=11;
	b.prev_x=-1;//valore fittizio per entrare sempre nel primo if di nextStep()
	b.prev_y=-1;
	b.spawned=1;
	drawBlinky(b.x, b.y, b_status);
}
void moveBlinky(){
	if(b_status==CHASE && b.spawned){
		eraseCellBlinky(b.x, b.y);
		chase();
		if(handleCollision()==0){
			drawBlinky(b.x, b.y, b_status);
			if(open && b.x==x_door && b.y==(y_door-1)){//Chiude la porta alle spalle di Blinky
				drawDoor(x_door, y_door);
			}
		}
	} else if(b_status==FRIGHT && b.spawned){
		eraseCellBlinky(b.x, b.y);
		flee();
		if(handleCollision()==0)
			drawBlinky(b.x, b.y, b_status);
	
		if(b.spawned==0)
			b_status=CHASE;
	}
}
//									DOWN,RIGHT,UP,LEFT
volatile int dx[4] = {0,	1,	0,	-1};
volatile int dy[4] = {1,	0, -1,	 0};
//Greedy Best-First Search
void chase(){
	volatile int x_next = 0, y_next = 0, nx, ny;
	volatile unsigned int min_dist = 0xffffffff, dist;
	volatile uint8_t i; 
	
	for(i = 0; i < 4; i++) {
		//Genera le mosse nelle 4 direzioni
		nx = b.x + dx[i];
		ny = b.y + dy[i];
		if(nx >= 0 && nx < R && ny >= 0 && ny < C
			&& mat[nx][ny] != WALL
			&& mat[nx][ny] != OUTMAP
			&& mat[nx][ny] != DOOR
			&& (ny != TELEP || (nx != TELEPX1 && nx != TELEPX2))
			&& (nx != b.prev_x || ny != b.prev_y)){//Per evitare i loop
			dist = abs(nx - p.x) + abs(ny - p.y);//Calcola la distanza da Pacman
			if(dist < min_dist){//Se la distanza è minima sceglie questa mossa
				min_dist = dist;
				x_next = nx;
				y_next = ny;
			}
			if(mat[nx][ny] == PACMAN)//Trovato!
				break;
		}
	}
	
	b.prev_x = b.x;
	b.prev_y = b.y;
	b.x = x_next;
	b.y = y_next;
}
//Fa il contrario di chase()
void flee(){
	volatile int x_next = 0, y_next = 0, nx, ny;
	volatile int min_dist = -1, dist;
	volatile uint8_t i;
	
	for(i = 0; i < 4; i++) {
		nx = b.x + dx[i];
		ny = b.y + dy[i];
		if(nx >= 0 && nx < R && ny >= 0 && ny < C
			&& mat[nx][ny] != WALL
			&& mat[nx][ny] != OUTMAP
			&& mat[nx][ny] != DOOR
			&& (ny != TELEP || (nx != TELEPX1 && nx != TELEPX2))
			&& (nx != b.prev_x || ny != b.prev_y || b_status==CHASE)){
			dist = abs(nx - p.x) + abs(ny - p.y);
			if(dist > min_dist){
				min_dist = dist;
				x_next = nx;
				y_next = ny;
			}
			if(mat[nx][ny] == PACMAN)
				break;
		}
	}
	
	b.prev_x = b.x;
	b.prev_y = b.y;
	b.x = x_next;
	b.y = y_next;	
}
void changeStatus(uint8_t status){
	b_status=status;
	b.prev_x = -1;//permette fuga
	b.prev_y = -1;
	drawBlinky(b.x, b.y, b_status);
}
int handleCollision(){
	if(b.x == p.x && b.y == p.y){
		if(b_status == CHASE){
			eraseCellBlinky(b.x, b.y);
			drawBlinky(b.x, b.y, b_status);
			initPacman();
			life--;
			draw=1;
		} else if(b_status == FRIGHT){
			eraseCellBlinky(b.x, b.y);
			drawPacman(p.x, p.y);
			playNote(eatblinky);
			b.x=-1;
			b.y=-1;
			drawDoor(x_door, y_door);
			b_status=CHASE;
			b.spawned=0;
			
		}
		return 1;
	}
	return 0;
}