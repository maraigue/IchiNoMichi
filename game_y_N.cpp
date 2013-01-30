#pragma once

#include <DxLib.h>
#include "game_y_N.h"

N::N(){
	this->x = 0;
	this->y = 0;
	this->visible = FALSE;
}

N::N(double x, double y){
	this->x = x;
	this->y = y;
}


void N::setPos(double x, double y){
	this->x = x;
	this->y = y;
}


void N::draw(){
	if(this->visible){
      DrawString(x,y,"(ÅEwÅE)Ç…Ç·Å[ÇÒ",GetColor(255,255,255),TRUE);
	}
}