#include "game_y_Darkroom.h"
#include <DxLib.h>
#include <sstream>
#include <math.h>
#include "game_y_define.h"


Darkroom::Darkroom(void)
{
	std::stringstream ss;
	
	for(int i = 0; i < 8; i++){
		ss.str("");
		ss <<"res/se/Red0"<< (i+1) << ".mp3";
		fx_r[i] = LoadSoundMem(ss.str().c_str());
		ss.str("");
		ss << "res/se/Green0"<<(i+1) << ".mp3";
		fx_g[i] = LoadSoundMem(ss.str().c_str());
		ss.str("");
		ss << "res/se/Blue0" <<(i+1) << ".mp3";
		fx_b[i] = LoadSoundMem(ss.str().c_str());
	}
}

Darkroom::~Darkroom(void)
{
}

void Darkroom::setSound(int type, SoundSource* ss){
	
	switch(type){
	case 0:
		{
			MoveCircular* p;
			p = new MoveCircular();
			p->angle = rand() % 360;
			p->radius = rand() % 150 + 20;
			ss->wave.color = GetColor(255,128,128);					
			ss->pattern = p;
			ss->type = 0;
			ss->image = soundImage[0];
			ss->moveSound = fx_r[rand()%8];
	   }
		break;
	case 1:
		{
			MoveReflect* p;
			p = new MoveReflect();
			ss->wave.color = GetColor(128,255,128);
			ss->pattern = p;
			ss->type = 1;
			ss->image = soundImage[1];
			ss->moveSound = fx_g[rand()%8];
		}
		break;
	case 2:
		{
			MoveEscape* p;
			p = new MoveEscape();
			p->player_x = &(player.x);
			p->player_y = &(player.y);
			ss->pattern = p;
			ss->type = 2;
			ss->wave.color = GetColor(128,128,255);
			ss->image = soundImage[2];
			ss->moveSound = fx_b[rand()%8];
		}
		break;
	case 3:
		{
			MoveSinWave* p = new MoveSinWave(ss->y, 0.001, 100);
			p->dx = 15;
			ss->pattern = p;
			ss->type = 0;
			ss->wave.color = GetColor(128,128,255);
			ss->image = soundImage[0];
			ss->moveSound = fx_b[rand()%8];
		}
		break;
	default:
			MoveSinWave* p = new MoveSinWave(ss->y - 50, 0.1, 50);
			ss->pattern = p;
			ss->type = 0;
			ss->wave.color = GetColor(255,128,128);
			ss->image = soundImage[0];
			ss->moveSound = fx_r[rand()%8];
		break;
	}
}

void Darkroom::init0(void){
	limitTime = 999 * 60;
	DeleteGraph(background);
	DeleteGraph(forground);
	background = LoadGraph("res/bg/tutorial_ym.png",TRUE);
	bgm = LoadSoundMem("res/bgm/Tutorial.mp3");


	for(int i = 0; i < SOUND_NUM; i++) {
		sound[i].visible = FALSE;
	}

	MoveCircular* p;
	
	{
	setSound(0, &sound[0]);
	sound[0].visible = TRUE;
	sound[0].caught = FALSE;
	sound[0].x = WINDOW_WIDTH / 2 + 150 * cos(20 * PI / 180);
	sound[0].y = WINDOW_HEIGHT / 2 + 150 * sin(20 * PI / 180);
	
	p = new MoveCircular();
	p->angle = 20;
	p->radius = 150;
	sound[0].pattern = p;
	}
	{
	setSound(1, &sound[1]);
	sound[1].visible = TRUE;
	sound[1].caught = FALSE;
	sound[1].x = WINDOW_WIDTH / 2 + 150 * cos(140 * PI / 180);
	sound[1].y = WINDOW_HEIGHT / 2 + 150 * sin(140 * PI / 180);

	p = new MoveCircular();
	p->angle = 140;
	p->radius = 150;
	sound[1].pattern = p;
	}
	{
	setSound(2, &sound[2]);
	sound[2].visible = TRUE;
	sound[2].caught = FALSE;
	sound[2].x = WINDOW_WIDTH / 2 + 150 * cos(260 * PI / 180);
	sound[2].y = WINDOW_HEIGHT / 2 + 150 * sin(260 * PI / 180);

	p = new MoveCircular();
	p->angle = 260;
	p->radius = 150;
	sound[2].pattern = p;
	}


	for(int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].visible = FALSE;
	}
	
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Darkroom::init(GameResult * in_result){
	
	result.stage_color(in_result->stage_color());
	result.stage_level(in_result->stage_level());
	result.end_status(0);
	DeleteGraph(soundImage[0]);
	soundImage[0] = LoadGraph("res/img/R.png");
	DeleteGraph(soundImage[1]);
	soundImage[1] = LoadGraph("res/img/G.png");
	DeleteGraph(soundImage[2]);
	soundImage[2] = LoadGraph("res/img/B.png");

	lastLoop = -1;
	gameEnd = FALSE;
	damaged = 0;
	frame = 0;
	time = 0;
	limitTime = 1200;
	switch(result.stage_level()){
	case 0:
		init0();
		break;
	case 1:
		init1();
		break;
	case 2:
		init2();
		break;
	case 3:
		init3();
		break;
	case 4:
		init4();
		break;
	default:
		init4();
		break;
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}


GameResult * Darkroom::update(void){

	char key[256];
	GetHitKeyStateAll(key);
	int joypad = GetJoypadInputState(DX_INPUT_PAD1);
	double dx = 0;
	double dy = 0;

	if(key[KEY_INPUT_UP] == 1 || (joypad & PAD_INPUT_UP)){
		dy = -3;
	} else if(key[KEY_INPUT_DOWN] == 1 || (joypad & PAD_INPUT_DOWN)) {
		dy = 3;
	} 

	if(key[KEY_INPUT_LEFT] == 1 || (joypad & PAD_INPUT_LEFT)){
		dx = -3;
	} else if(key[KEY_INPUT_RIGHT] == 1 || (joypad & PAD_INPUT_RIGHT)) {
		dx = 3;
	} 


	if(key[KEY_INPUT_Z] == 1 || (joypad & PAD_INPUT_1)) {
		if(!player.wave.visible){
			player.beep();
		}
	}

	if(key[KEY_INPUT_N] == 1 ) {
		lastLoop = 999;
		result.end_status(1);
	} 

	if(damaged <= 0){
		player.translate(dx,dy);
	}
	player.update();
	for(int i = 0; i < ENEMY_NUM; i++){
		this->enemy[i].update();
	}

	for(int i = 0; i < SOUND_NUM; i++){
		if(sound[i].visible){
			//sound[i].damaged = damaged;
			sound[i].update();
			if(sound[i].caught){
				if(sound[i].x == player.secencer){
					sound[i].beep();
					PlaySoundMem(sound[i].moveSound, DX_PLAYTYPE_BACK);
				}
			}
			else {
				double dx = sound[i].x - player.wave.x;
				double dy = sound[i].y - player.wave.y;

				double r = player.wave.count;

				if(dx * dx + dy * dy < r * r){
					MoveDirect* p = new MoveDirect();
					//p->dest_x = sound[i].x;
					p->dest_x = (int)(player.secencer / 20) * 20;
					p->dest_y = WINDOW_HEIGHT - 60 +  sound[i].type * 20;
					delete sound[i].pattern;
					sound[i].pattern = p;
					sound[i].caught = TRUE;
					sound[i].beep();
					PlaySoundMem(sound[i].moveSound, DX_PLAYTYPE_BACK);
				}
			}
		}
	}

	for(int i = 0; i < ENEMY_NUM; i++){
		if(enemy[i].visible){
			double dx = this->enemy[i].x - player.x;
			double dy = this->enemy[i].y - player.y;

			if(damaged <= 0 && dx * dx + dy * dy < 64){
				damaged = 60;
			}
		}
	}

	frame = ++frame % 60;
	time++;

	if(lastLoop >= WINDOW_WIDTH){
		SetVolumeMusic(255) ;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		StopSoundMem(bgm);
		//result.stage_level(result.stage_level()+1);
		//init(&result);
	}


	return lastLoop < WINDOW_WIDTH ? NULL : &result;
}

void Darkroom::draw(void){
	SetBackgroundColor(255,255,255);
	if(lastLoop > 0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - lastLoop * 0.2);
		SetVolumeMusic(255 - lastLoop * 0.2) ;
	} else if(CheckHitKey(KEY_INPUT_R) || damaged > 0){
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		damaged--;
	} else {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	DrawGraph(0,0, background,FALSE);	
	DrawGraph(0,0, forground,TRUE);

	this->player.draw();

	int total = 0;
	get[0] = get[1] = get[2] = 0;
	for(int i = 0; i < SOUND_NUM; i++){
		this->sound[i].draw();
		if(this->sound[i].caught){
			get[sound[i].type]++;
		}
		if(sound[i].visible){
			total++;
		}
	}
	result.r_score(get[0]);
	result.g_score(get[1]);
	result.b_score(get[2]);

	for(int i = 0; i < ENEMY_NUM; i++){
		this->enemy[i].draw();
	}

	if(total * 0.8 < get[0] + get[1] + get[2]){
		gameEnd = TRUE;
		lastLoop++;
		result.end_status(1);
	}
	if( time > limitTime){
		lastLoop++;
	}

	std::stringstream ss;
	ss.str("");
	int score = (int)(((double) get[0] + get[1] + get[2]) / total * 100);
	ss << "STAGE:" << result.stage_level() << " SOUNDS: " << get[0] + get[1] + get[2] << "/" << total <<  "(" << score << "%)";
	if(score > 79){
		ss << "CLEAR";
	}
	ss << std::endl;
	DrawString(0, 0, ss.str().c_str(), GetColor(0,0,0),  GetColor(255,255,255));

	ss.str("");
	ss <<"R:" << get[0] << " G:" << get[1] << " B:" << get[2] << std::endl;
	DrawString(0, 20, ss.str().c_str(), GetColor(0,0,0),  GetColor(255,255,255));

	ss.str("");
	ss <<"TIME:"<< (time / 60) << "sec" <<  " limit:" << (limitTime / 60)<< " last:"<< lastLoop <<std::endl;
	DrawString(0, 40, ss.str().c_str(), GetColor(0,0,0),  GetColor(255,255,255));
}

