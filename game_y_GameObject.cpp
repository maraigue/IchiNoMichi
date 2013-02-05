#include <sstream>
#include <DxLib.h>

#include "game_y_GameObject.h"
#include "game_y_define.h"

GameObject::GameObject(void)
{
	count = 0;
	pattern = NULL;
	
}


GameObject::~GameObject(void)
{
	delete pattern;
}

SoundSource::SoundSource(void) {
	caught = FALSE;
}


void SoundSource::draw(void)
{
	if(this->visible){
		wave.draw();
		if(caught || damaged > 0){
			DrawRotaGraph2( this->x, this->y, 15, 15,1.0, 0.0, this->image, TRUE, FALSE) ;
		} else {
			//DrawRotaGraph2( this->x, this->y, 15, 15,1.0, 0.0, this->image, TRUE, FALSE) ;
		}

	}
}

void SoundSource::beep(void){
	
		if(!caught){
			wave.x = this->x;
			wave.y = this->y;
		} else {
			wave.x = this->x;
			wave.y = this->y;
		}
		wave.visible = TRUE;
		wave.count = 0;

		
}

void SoundSource::update(void){
	if(pattern != NULL){
		this->pattern->move(&x, &y);
	}

	if(!caught && count == 0){
		beep();
	}
	
	count = ++count % 60;
}



SoundWave::SoundWave(void){
	visible = FALSE;
	count = 0;
	radius = 30;
	color = GetColor(255,128,128);
}

void SoundWave::draw(void)
{
	
	//printf("abcd");

	if(this->visible == TRUE){
		count ++;

		DrawCircle(this->x, this->y, this->count, this->color,FALSE);

		if(count > radius){
			visible = FALSE;
			count = 0;
		}
	}
}


Player::Player(void)
{
	LoadDivGraph("res/img/ch1.png", CHIP_NUM,3,9,46,63, this->charaChip);
	scoreImage = LoadGraph("res/img/score bar.png");
	secencer = 0;
	x = WINDOW_WIDTH / 2;
	y = WINDOW_HEIGHT / 2;
	dir = 5;
}


Player::~Player(void)
{
}

void Player::beep(void){
	
	wave.color = GetColor(128,128,128);
	wave.x = this->x;
	wave.y = this->y;

	wave.visible = TRUE;
	wave.count = 0;
	wave.radius = 45;
}

void Player::update(void){
	
	count = ++count % 60;
	
}

void Player::draw(void){

	//std::stringstream ss;
	//ss << this->dir << count;
	wave.draw();
	DrawCircle(this->x, this->y, 3, GetColor(128,128,255),TRUE);
	//DrawString(this->x, this->y, ss.str().c_str(), GetColor(128,128,128),TRUE);
	//DrawLine(0, WINDOW_HEIGHT - 40, WINDOW_WIDTH, WINDOW_HEIGHT - 40,GetColor(0,0,255),FALSE);
	DrawGraph(0, WINDOW_HEIGHT - 65, scoreImage ,FALSE);
	DrawBox(secencer - 20, WINDOW_HEIGHT - 65, secencer + 0, WINDOW_HEIGHT - 15,GetColor(255,0,0),FALSE);

	DrawGraph(this->x - 20, this->y - 32,charaChip[dir * 3 + (count / 20)], TRUE);
	secencer = ++secencer % WINDOW_WIDTH;
	//PlaySoundMem(moveSound, DX_PLAYTYPE_BACK);
}

void Player::translate(double dx, double dy) {
	x += dx;
	y += dy;

	if(dx > 0){
		if(dy > 0){
			dir = 3; dir = 7;
		} else if(dy < 0) {
			dir = 9; dir = 8;
		} else {
			dir = 6; dir = 3;
		}
	} else if(dx < 0){
		if(dy > 0){
			dir = 1; dir = 5;
		} else if(dy < 0) {
			dir = 7; dir = 6;
		} else {
			dir = 4; dir = 4;
		}
	} else {
		if(dy > 0){
			dir = 2; dir  =1;
		} else if(dy < 0) {
			dir = 8; dir = 2;
		} else {
			dir = 5; dir = 0;
		}
	}

	if(x < 0){
		x = 0;
	} 
	if(x > WINDOW_WIDTH){
		x = WINDOW_WIDTH;
	}
	if(y < 0){
		y = 0;
	}
	if(y > WINDOW_HEIGHT){
		y = WINDOW_HEIGHT;
	}
}

Enemy::Enemy(void){
	pattern = NULL;
}

void Enemy::beep(void){
}

void Enemy::update(void){
	if(pattern != NULL){
			pattern->move(&x, &y);
	}

}

void Enemy::draw(void){
	if(visible){
		DrawBox(x - 5,y -5, x + 5, y + 5, GetColor(127,127,127),TRUE);
		DrawBox(x - 5,y -5, x + 5, y + 5, GetColor(195,195,195),FALSE);
	}
}

