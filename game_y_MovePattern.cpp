
#include <math.h>
#include "game_y_MovePattern.h"
#include "game_y_define.h"
#include <stdio.h>


MovePattern::MovePattern(void)
{
}


MovePattern::~MovePattern(void)
{
}

void MoveDirect::move(double* x, double* y){

	double dx = this->dest_x - *x;
	double dy = this->dest_y - *y;

	if(dx * dx + dy * dy > 0.1){
		*x += dx / 30;
		*y += dy / 30;
	} else {
		*x = this->dest_x;
		*y = this->dest_y;
	}


}

MoveReflect::MoveReflect(void){
	vx = 1.3;
	vy = 1.3;
}

void MoveReflect::move(double* x, double* y){

	*x += vx;
	*y += vy;

	if(*x < 20){
		*x = 20;
		vx *= -1;

	}else if(*x > WINDOW_WIDTH - 20){
		*x = WINDOW_WIDTH - 20;
		vx *= -1;
	}

	if(*y < 80){
		*y = 80;
		vy *= -1;
	} else if(*y > WINDOW_HEIGHT - 60){
		*y = WINDOW_HEIGHT - 60;
		vy *= -1;		
	}
}

void MoveCircular::move(double* x, double* y) {

	double rad = angle * PI / 180;

	double cx = *x - radius * cos(rad);
	double cy = *y - radius * sin(rad);

	angle = ++angle % 360;
	rad = angle * PI / 180;
	*x = cx + radius * cos(rad);
	*y = cy + radius * sin(rad);
}


void MoveEscape::move(double* x, double* y){

	double dx = *x - *player_x;
	double dy = *y - *player_y;

	double distance_sq = dx * dx + dy * dy;

	if(distance_sq > 10000){
		*x += dx > 0 ? -0.1: 0.1;
		*y += dy > 0 ? -0.1: 0.1;
	} else {
		*x += dx < 0 ? -0.5: 0.5;
		*y += dy < 0 ? -0.5: 0.5;
	}
	

	if(*x < 20){
		*x = 20;
	

	}else if(*x > WINDOW_WIDTH - 20){
		*x = WINDOW_WIDTH - 20;

	}

	if(*y < 40){
		*y = 40;

	} else if(*y > WINDOW_HEIGHT - 60){
		*y =  WINDOW_HEIGHT - 60;
			
	}

}

MoveSinWave::MoveSinWave(double base, double av, double width){
	this->base = base;
	this->av = av;
	this->width = width;
	angle = 0;
	dx = 1;

}

void MoveSinWave::move(double* x, double* y){

	//double angle = asin((*y - base) / width);
	angle += av;
	*y = width * sin(angle) + base;
	*x = ((int)(*x + dx)) % WINDOW_WIDTH;

}