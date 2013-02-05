#include "game_y_Darkroom.h"
#include "game_y_define.h"

void Darkroom::init1(void){
	limitTime = 60 * 60;
	DeleteGraph(background);
	background = LoadGraph("res/bg/stage1_bg_y.png");	
	DeleteGraph(forground);
	forground = LoadGraph("res/bg/stage1_fg.png");
	DeleteSoundMem(bgm);
	bgm = LoadSoundMem("res/bgm/Stage1.mp3");

	
	for(int i = 0; i < SOUND_NUM; i++) {
		sound[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		sound[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		sound[i].v_x = 1 + (double)(rand() % 100)/100;
		sound[i].v_y = 1 + (double)(rand() % 100)/100;
		sound[i].caught = FALSE;
		if( i < 24){
			sound[i].visible = TRUE;
		} else {
			sound[i].visible = FALSE;
			continue;
		}
		sound[i].count = rand() % 4 * 15;
		if(i < 24 / 3){
			setSound(0, &sound[i]);
		} else if(i < 24 / 3 * 2){
			setSound(1, &sound[i]);
		} else {
			setSound(2, &sound[i]);
		}

	}

	for(int i = 0; i < 8; i++) {

		enemy[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		enemy[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		if(i < 2){
			enemy[i].visible = TRUE;
		} else {
			enemy[i].visible = FALSE;
			continue;		
		}

		enemy[i].count = rand() % 4 * 15;
		delete(enemy[i].pattern);
		if(i % 4 == 0){
			//MoveSinWave* p;
			MoveSinWave* p = new MoveSinWave(enemy[i].y, rand() % 5 * 0.01 , rand() % 3 * 20 + 10);
			//p->angle = rand() % 360;
			//p->radius = rand() % 150 + 20;
			enemy[i].pattern = p;
		} else if(i % 4 == 1){
			MoveReflect* p;
			p = new MoveReflect();
			enemy[i].pattern = p;
		} else if(i % 4 == 2){
			MoveEscape* p;
			p = new MoveEscape();
			p->player_x = &(player.x);
			p->player_y = &(player.y);
			enemy[i].pattern = p;
		} else {
			MoveSinWave* p = new MoveSinWave(enemy[i].y, rand() % 5 * 0.01 , rand() % 3 * 20 + 10);
			p->dx = rand() % 5;
			enemy[i].pattern = p;
		}
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Darkroom::init2(void){
	limitTime = 60 * 90;
	DeleteGraph(background);
	switch(result.stage_color()){
	case 0:
		background = LoadGraph("res/bg/stage2_R_bg_y.png");
		break;
	case 1:
		background = LoadGraph("res/bg/stage2_G_bg_y.png");
		break;
	case 2:
		background = LoadGraph("res/bg/stage2_B_bg_y.png");	
		break;
	}
	DeleteGraph(forground);
	switch(result.stage_color()){
	case 0:
		forground = LoadGraph("res/bg/stage2_R_fg.png");
		break;
	case 1:
		forground = LoadGraph("res/bg/stage2_G_fg.png");
		break;
	case 2:
		forground = LoadGraph("res/bg/stage2_B_fg.png");	
		break;
	}
	DeleteSoundMem(bgm);
	bgm = LoadSoundMem("res/bgm/Stage2.mp3");


	for(int i = 0; i < SOUND_NUM; i++) {
		sound[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		sound[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		sound[i].v_x = 1 + (double)(rand() % 100)/100;
		sound[i].v_y = 1 + (double)(rand() % 100)/100;
		sound[i].caught = FALSE;
		if( i < 48){
			sound[i].visible = TRUE;
		} else {
			sound[i].visible = FALSE;
			continue;
		}

		sound[i].count = rand() % 4 * 15;
		setSound(i % 3, &sound[i]);
	}
	
		for(int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		enemy[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		//enemy[i].v_x = 1 + (double)(rand() % 100)/100;
		//enemy[i].v_y = 1 + (double)(rand() % 100)/100;
		if(i < 4){
			enemy[i].visible = TRUE;
		} else {
			enemy[i].visible = FALSE;
			continue;		
		}

		enemy[i].count = rand() % 4 * 15;
		delete(enemy[i].pattern);
		if(i % 3 == 0){
			MoveCircular* p;
			p = new MoveCircular();
			p->angle = rand() % 360;
			p->radius = rand() % 150 + 20;
			//sound[i].wave.color = GetColor(255,128,128);					
			enemy[i].pattern = p;
		} else if(i % 3 == 1){
			MoveReflect* p;
			p = new MoveReflect();
			//enemy[i].wave.color = GetColor(128,255,128);
			enemy[i].pattern = p;
		} else {
			MoveSinWave* p = new MoveSinWave(enemy[i].y, rand() % 5 * 0.01 , rand() % 3 * 20 + 10);
			enemy[i].pattern = p;
		}
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Darkroom::init3(void){
	limitTime = 60 * 120;
	DeleteGraph(background);
	switch(result.stage_color()){
	case 0:
		background = LoadGraph("res/bg/stage3_R_bg_y.png");
		break;
	case 1:
		background = LoadGraph("res/bg/stage3_G_bg_y.png");
		break;
	case 2:
		background = LoadGraph("res/bg/stage3_B_bg_y.png");	
		break;
	}
	DeleteGraph(forground);
	switch(result.stage_color()){
	case 0:
		forground = LoadGraph("res/bg/stage3_R_fg.png");
		break;
	case 1:
		forground = LoadGraph("res/bg/stage3_G_fg.png");
		break;
	case 2:
		forground = LoadGraph("res/bg/stage3_B_fg.png");	
		break;
	}
	DeleteSoundMem(bgm);
	bgm = LoadSoundMem("res/bgm/Stage3.mp3");


	for(int i = 0; i < SOUND_NUM; i++) {
		sound[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		sound[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		sound[i].v_x = 1 + (double)(rand() % 100)/100;
		sound[i].v_y = 1 + (double)(rand() % 100)/100;
		sound[i].caught = FALSE;
		if( i < 64){
			sound[i].visible = TRUE;
		} else {
			sound[i].visible = FALSE;
			continue;
		}

		sound[i].count = rand() % 4 * 15;
		setSound(i % 3, &sound[i]);
	}

	for(int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		enemy[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		//enemy[i].v_x = 1 + (double)(rand() % 100)/100;
		//enemy[i].v_y = 1 + (double)(rand() % 100)/100;
		if(i < 6){
			enemy[i].visible = TRUE;
		} else {
			enemy[i].visible = FALSE;
			continue;		
		}

		enemy[i].count = rand() % 4 * 15;
		delete(enemy[i].pattern);
		if(i % 3 == 0){
			MoveCircular* p;
			p = new MoveCircular();
			p->angle = rand() % 360;
			p->radius = rand() % 150 + 20;
			//sound[i].wave.color = GetColor(255,128,128);					
			enemy[i].pattern = p;
		} else if(i % 3 == 1){
			MoveReflect* p;
			p = new MoveReflect();
			//enemy[i].wave.color = GetColor(128,255,128);
			enemy[i].pattern = p;
		} else {
			MoveEscape* p;
			p = new MoveEscape();
			p->player_x = &(player.x);
			p->player_y = &(player.y);
			//sound[i].wave.color = GetColor(128,128,255);
			enemy[i].pattern = p;
		}
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Darkroom::init4(void){
	limitTime = 60 * 120;
	DeleteGraph(background);
	switch(result.stage_color()){
	case 0:
		background = LoadGraph("res/bg/stage4_R_bg_y.png");
		break;
	case 1:
		background = LoadGraph("res/bg/stage4_G_bg_y.png");
		break;
	case 2:
		background = LoadGraph("res/bg/stage4_B_bg_y.png");	
		break;
	}
	DeleteGraph(forground);
	switch(result.stage_color()){
	case 0:
		forground = LoadGraph("res/bg/stage4_R_fg.png");
		break;
	case 1:
		forground = LoadGraph("res/bg/stage4_G_fg.png");
		break;
	case 2:
		forground = LoadGraph("res/bg/stage4_B_fg.png");	
		break;
	}
	DeleteSoundMem(bgm);
	bgm = LoadSoundMem("res/bgm/Stage3.mp3");


	for(int i = 0; i < SOUND_NUM; i++) {
		sound[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		sound[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		sound[i].v_x = 1 + (double)(rand() % 100)/100;
		sound[i].v_y = 1 + (double)(rand() % 100)/100;
		sound[i].caught = FALSE;
		if( i < 64){
			sound[i].visible = TRUE;
		} else {
			sound[i].visible = FALSE;
			continue;
		}

		sound[i].count = rand() % 4 * 15;
		setSound(i % 3, &sound[i]);
	}

	for(int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].x = rand() % WINDOW_WIDTH - 80 + 40;
		enemy[i].y = rand() % WINDOW_HEIGHT - 80 + 20;
		if(i < 6){
			enemy[i].visible = TRUE;
		} else {
			enemy[i].visible = FALSE;
			continue;		
		}

		enemy[i].count = rand() % 4 * 15;
		delete(enemy[i].pattern);
		if(i % 3 == 0){
			MoveCircular* p;
			p = new MoveCircular();
			p->angle = rand() % 360;
			p->radius = rand() % 150 + 20;				
			enemy[i].pattern = p;
		} else if(i % 3 == 1){
			MoveReflect* p;
			p = new MoveReflect();
			enemy[i].pattern = p;
		} else {
			MoveEscape* p;
			p = new MoveEscape();
			p->player_x = &(player.x);
			p->player_y = &(player.y);
			enemy[i].pattern = p;
		}
	}
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}