#include "pacman.h"

pacman_t p;
volatile int score=0, eatp=0, life=1, tonewlife=0;

void initPacman(){
	p.x=13;
	p.y=21;
	drawPacman(p.x, p.y);
	p.dir=KEEP;
}
void movePacman(){
	if(p.dir==KEEP)
		return;
	
	if(p.dir == DOWN){
		if(mat[p.x][p.y-1]!=WALL){//DOWN
			setScore(p.x, p.y-1);
			eraseCell(p.x, p.y);			
			p.y-=1;
		}
	}
	
	if(p.dir == LEFT){
		if(p.x==0 && p.y==TELEP){
			eraseCell(p.x, p.y);
			p.x=C-1;
		} else if(mat[p.x-1][p.y]!=WALL){//LEFT
			setScore(p.x-1, p.y);
			eraseCell(p.x, p.y);
			p.x-=1;
		}
	}
	
	if(p.dir == RIGHT){
		if(p.x==C-1 && p.y==TELEP){
			eraseCell(p.x, p.y);
			p.x=0;
		} else if(mat[p.x+1][p.y]!=WALL){//RIGHT
			setScore(p.x+1, p.y);
			eraseCell(p.x, p.y);
			p.x+=1;
		}
	}
	
	if(p.dir == UP){
		if(mat[p.x][p.y+1]!=WALL){//UP
			setScore(p.x, p.y+1);
			eraseCell(p.x, p.y);
			p.y+=1;
		}
	}
	
	if(handleCollision()==0)
		drawPacman(p.x, p.y);
}
volatile int dir=KEEP;//direzione precedente alla pausa
void stopPacman(){
	dir=p.dir;
	p.dir=KEEP;
}
void restorePacman(){
	p.dir=dir;
	dir=KEEP;//safe
}
void setScore(int x, int y){
	int points=0;
	if(mat[x][y]==SPILL){
		points=SCORESP;
	} else if(mat[x][y]==PPILL){
		changeStatus(FRIGHT);
		points=SCOREPP;
	} else if(mat[x][y]==BLINKY && b_status==FRIGHT){
		points=SCOREBLINKY;
	}
	if(points){
		//gestione punteggio
		score+=points;
		//drawScore();
		draw=1;
		//gestione vita
		tonewlife+=points;
		if(tonewlife>=NEWLIFE){
			tonewlife-=NEWLIFE;
			life++;
			draw=1;
			//drawLife();
		}
		//gestione vittoria
		eatp++;
		if(eatp == NSP)
		victory=1;
	}	
}