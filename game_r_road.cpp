/*
	by rocca
	   -> 2013/01/27
*/

#pragma once
#include <DxLib.h>
#include "game_r_road.h"
#include "game_r_road_datas.h"
#include "scene.hpp"
GameResult *rd_g_res;
GameRoad::GameRoad(void)
{
	init(rd_g_res);
}


//�@�f�[�^�j��
void GameRoad::LoadDel(void)
{
	for(int i=0; i<_LO_SP_END; i++){
		DeleteGraph( sp_num[i] ) ;
	}
	for(int i=0; i<8; i++){
		for(int j=0; j<5; j++){
			DeleteGraph( ichi_num[i][j] );
		}
	}
	for(int i=0; i<_LO_SE_END; i++){
		DeleteSoundMem( se_num[i] );
	}

}

void GameRoad::init(GameResult * result)
{
	rd_g_res = result;
	LoadDel();
	cond = LO_INI;
	move_time = 0;
	next_time = 1;
	time = 0;
	miss = 0;
	for(int i=0; i<3; i++){
		get_rgb[i] = 0;
	}
	all_road = 0;
	now_level = 0;
	lose_cnt = 20;	// 20��~�X�Ŕs�k(��)

	lo_key = 0;

	del_block.ok = 0;
	del_block.type = 0;
	del_block.col = 0;
	block.ok = 0;
	block.type = 0;
	block.col = rand() % 3;
	once_btn = 0;

	// ����
	ch_movetime = 4;
	ch_time = 4;
	ch_mode = 0;
	ch_walk = 0;

	alpha = 255;
	SetDrawBright( alpha , alpha , alpha );
	SetFontSize(16);
	if(result == NULL){
		s_col;
		stage;
	}else{
		s_col = result->stage_color();
		stage = result->stage_level();
	}
}

void GameRoad::setNewBlockTime()
{

	next_time = 10;
}

// �V�K�u���b�N����
void GameRoad::newBlock()
{
	int c = 0;		
	// i = �V�K�u���b�N���i�[�ꏊ
	if(block.ok == 1){ return; }

	// �Z�b�g
	while(1){
		int set_k = rand() % RO_Y_CNT;
//		if(now_n[set_k] != -1){
			block.k = set_k;
			block.ok = 1;
			block.x = 800;
			block.type = rand() % 3;
			block.col = rand() % 3;	// R G B
			break;
//		}
	}
	setNewBlockTime();
}

void GameRoad::Failureroad(void)
{
		// �����u���b�N���s�`�F�b�N
	if(block.ok == 1){
		block.ok = -1;
		setNewBlockTime();
		del_block = block;
		del_block.ok = 255;
		miss++;
	}
}
// �L�[����
void GameRoad::roadSetCheck(void)
{

	int c = 0;
	int i;
	int n = -1;
	if(block.ok != 1){ return; }			// �������ł͖���

	int to_up, to_down, to_left = 0;
	
	SPRITE *sp = &spd[LO_SP_BR_R1];
	LO_LEVEL *lv = &levels[now_level];	// ���݃��x��
	// ���݂�x�ʒu���Z�o
	for (i=0; i<RO_X_CNT; i++) {	
		if (block.x > roadx[i] && block.x < roadx[i]+sp->w){
			break; 
		}

	}

	n = i;
	if(n == RO_X_CNT){ return; }

	if(block.k == 0){ to_up = 0; }
	else{ to_up = roads[block.k-1][n] & 0x0f; }

	if(block.k == RO_Y_CNT-1){ to_down = 0; }
	else { to_down = roads[block.k+1][n] & 0x0f; }

	// ��
	if(n == RO_X_CNT-1){ to_left = 1; }
	else{ to_left = roads[block.k][n+1] & 0x0f; } 

	int res = -1;
	// �ϗ�����
	// ��
	if((block.x <= roadx[n]+lv->speed) && to_left){
		if(block.type == 0 && !to_up && !to_down){
			res = 1;
		}else{
			res = 0;
		}
	}

	// ��
	if(to_up){
		if(block.type == 1 && !to_down && !to_left){ 
			res = 1;
		}else{
			// ���s
			res = 0;
		}
	}
	// ��
	if(to_down){
		if(block.type == 2 && !to_left && !to_up){ 
			res = 1;
		}else{
			// ���s
			res = 0;
		}
	}

	if(res == 1){
		   roads[block.k][n] |= block.type+1;
		   roads[block.k][n] |= block.col<<4;
			block.ok = 0;
			all_road++;
			get_rgb[block.col]++;
			int sn = ses[block.col]+(rand()%8);
			int se = se_num[sn];
			PlaySoundMem(se , DX_PLAYTYPE_BACK);
			roads_se[block.k][n] = se;
			setNewBlockTime();
	}else if(res == 0){
		Failureroad();
	}
}

int GameRoad::clearCheck()
{
	for(int i=0; i<RO_Y_CNT; i++){
		int n = roads[i][RO_X_CNT] & ~0xf0;
		if(roads[i][0] != 0 ){ myroad = i; return i; }
	}
	return -1;
}

int GameRoad::loseCheck()
{
	if(miss >= lose_cnt){ return 1; }

	return 0;
}
static void BGMStop()
{
	for(int i = LO_BGM_1; i<= LO_BGM_T; i++){
		StopSoundMem(se_num[i]);
	}
}
GameResult * GameRoad::update(void)
{
	int c = 0;	//�Y���u���b�N�J�E���g
	int i;

	LO_LEVEL *lv = &levels[now_level];	// ���݃��x��

	GetHitKeyStateAll( Key ) ;
	// �摜�Z�b�g�A�b�v
	switch(cond){
		case LO_GAMEOVER:
			if(alpha == 0){
				// �L�[�҂���J��
				if(Key[KEY_INPUT_Y] == 1){ init(rd_g_res); return NULL; }	// �R���e�B�j���[
				if(Key[KEY_INPUT_N] == 1){ 
					ro_g_res.end_status(0);
					return &ro_g_res; 
				}			// �I��
			} 
		break;
		case LO_END:
			ro_g_res.end_status(1);
			return &ro_g_res; // ����I��
		case LO_INI:{

			cond = LO_GAME;
			c = s_col*5 + stage;
			spd[LO_SP_BG].num = LoadGraph(pix_bgs[c]);
			spd[LO_SP_FB].num = LoadGraph(pix_fgs[c]);

			// �摜���[�h
			for(i=LO_SP_BR_R1; i<_LO_SP_END; i++){
				spd[i].num = LoadGraph(pixs_R[i]);
			}
			

			// ���ʉ����[�h
			for(i=0; i<_LO_SE_END; i++){
				se_num[i] = LoadSoundMem(sounds[i]);
			}

			// �����p�^�[���摜
			int tmp[40];
			int i_c = 0;
			LoadDivGraph(pixs_R[LO_SP_ICHI], I_PATMAX, 3, 8, spd[LO_SP_ICHI].w, spd[LO_SP_ICHI].h,tmp);
			for(i=0; i<I_PATS; i++){
				for(int j=0; j<I_MOVS; j++){
					ichi_num[i][j] = tmp[i_c];
					i_c++;
				}
			}
			spd[LO_SP_ICHI].num = spd[LO_SP_ICHI].pat = 0;
			spd[LO_SP_ICHI].x = 0;
			spd[LO_SP_ICHI].y = 300;

			// �O���
			for(i=0; i<RO_Y_CNT; i++){
				now_n[i] = RO_X_CNT - 1;
			}

			for(i=0; i<RO_Y_CNT; i++){
				for(int j=0; j<RO_X_CNT; j++){
					roads[i][j] = tmp_roads[i][j];
					roads_se[i][j] = tmp_roads_se[i][j];
				}
			}

			// bgm
			PlaySoundMem(se_num[bgms[stage]] , DX_PLAYTYPE_LOOP);

			break;
		}
		case LO_GAME:
			// �ǂ����ЂƂł��Ȃ���΃N���A�[
			if(clearCheck() > -1){ cond = LO_CLEAR; BGMStop(); break;}
			// �~�X�݌v�Ŕs�k
			if(loseCheck() == 1){ cond = LO_LOSE; BGMStop(); break; }
			time++;
			// ���x���`�F�b�N
			lv++;
			if(lv->road != -1 && lv->road <= all_road){
				now_level++;
				lv++;
			}else{
				lv--;
			}

			move_time++;
			// move_time 6���ɍX�V
			if(move_time >= 6){
				move_time = 0;
				// ����
				c = 0;
				if(block.ok == 1){
					block.x -= lv->speed;
				}
			}

			// �V�K�u���b�N����
			newBlock();

			// �����L�[
			// ���
			if(Key[KEY_INPUT_LEFT] > 0){
				move_time++;
			}
			// ���
			if(Key[KEY_INPUT_RIGHT] > 0){
				move_time--;
			}
			// �ʒu�ړ�(��)
			if(Key[KEY_INPUT_UP] == 1){
				if(!(once_btn & 1)){
					do{
						block.k--;
						if(block.k < 0){ block.k = 0; } 
					}while(now_n[block.k] == -1);
					once_btn |= 1;
				}
			}else{ once_btn &= ~1; }

			// �ʒu�ړ�(��)
			if(Key[KEY_INPUT_DOWN] == 1){
				if(!(once_btn & 2)){
					do{
						block.k++;
						if(block.k > RO_Y_CNT-1){ block.k = RO_Y_CNT-1; } 
					}while(now_n[block.k] == -1);
					once_btn |= 2;
				}
				
			}else{ once_btn &= ~2; }
			// �����ύX�L�[
			if(Key[KEY_INPUT_RETURN] == 1){
				if(!(once_btn & 4)){
					block.type++;
					if(block.type > 2){ block.type = 0; }
				}
				once_btn |= 4;
			}else{ once_btn &= ~4; }

			roadSetCheck();

			
		break;

		// game end
		case LO_CLEAR:
			// �S�[���܂ŕ����Ă�������
			if(ch_mode == 1){
				chMoveSet();
				chMove();
			}else{
//				PlaySoundMem(se_num[LO_SE_CLEAR] , DX_PLAYTYPE_BACK);
			}
			ch_mode = 1;
			break;
		
		case LO_LOSE:
			// �S�[���܂ŕ����Ă�������
			if(ch_mode == 1){
				chMoveSet();
				chMove();
			}else{
	//			PlaySoundMem(se_num[LO_SE_CLEAR] , DX_PLAYTYPE_BACK);
			}
			ch_mode = 1;
			
			break;
	}

	return 0;
}

void GameRoad::draw(void)
{
	SPRITE *sp;
	int c = 0;
	if(cond == LO_INI){ return; }
	if(del_block.ok > 0){
		SetDrawBlendMode( DX_BLENDMODE_INVSRC, 128);
	}
	if(cond == LO_GAMEOVER/* || cond == LO_END*/){
		
		SetDrawBright( alpha , alpha , alpha );
		if(alpha == 0){
			SetDrawBright( 255, 255, 255 );
			// �Q�[���I�[�o�[�w�i�`��
			for(int i=LO_SP_LOSE1; i<=LO_SP_LOSE4; i++){
				sp = &spd[i];
				DrawGraph(sp->x, sp->y, sp->num, TRUE);
			}
			return;
		}
		alpha -= 16;
		if(alpha < 0){ alpha = 0; }
	}
	sp = &spd[LO_SP_BG];
	DrawGraph(sp->x, sp->y, sp->num, TRUE);
	
	sp = &spd[cols[block.col]+block.type];
	
	// ������u���b�N�`��
	if(block.ok == 1){
		DrawGraph(block.x, roady[block.k], sp->num, TRUE);
	}
	
	// ���ݖ��܂��Ă���u���b�N��`��
	
	for(int i=0;i<RO_Y_CNT; i++){
		for(int j=0; j<RO_X_CNT; j++){
			int n = roads[i][j] & ~0xf0;
			int co = roads[i][j]>>4;
			if(n!=0){
				sp = &spd[cols[co]+(n-1)];
				DrawGraph(roadx[j], roady[i], sp->num, TRUE); 
			}
		}
	}

	// ����
	sp = &spd[LO_SP_ICHI];
	DrawGraph(sp->x, sp->y, ichi_num[sp->pat][sp->num], TRUE);

	sp = &spd[LO_SP_FB];
	DrawGraph(sp->x, sp->y, sp->num, TRUE);

	LO_LEVEL *lv = &levels[now_level];	// ���݃��x��

	// ���s�u���b�N�`��
	if(del_block.ok > 0){
		del_block.ok -= 16;
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0);
	}
	if(del_block.ok <= 0){ del_block.ok = 0;}
	else{
		int n = del_block.type;
		sp = &spd[cols[del_block.col] + n];
		SetDrawBlendMode( DX_BLENDMODE_ALPHA,  del_block.ok );		//�u�����h���[�h
		DrawRotaGraph( del_block.x, roady[del_block.k], 1.0,  0.0, sp->num, TRUE ); //�摜�̕`��
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );		//�u�����h���[�h���I�t
	}

//#ifdef _DEBUG
	// �N���A�`��
	if(cond == LO_CLEAR || cond == LO_LOSE){
		if(stage == 0){
			SetFontSize(24);
			DrawFormatString( 420 , 8 , GetColor( 0 , 0 , 0 ), "Ichi can walk! (��/��/��/��)");
			SetFontSize(20);
			DrawFormatString( 100, 48, GetColor(0, 0, 0), 
				"Let's cross to the opposite shore. Or let's fall from a bridge. " );
			DrawFormatString( 100, 76, GetColor(0, 0, 0), 
				"�Ί݂֓n��܂��傤�B�����Ȃ狴���痎���܂��傤�B" );
		}
	}else if(stage == 0){
		DrawString(100, 48+8, "��/���FPanel up down", GetColor(0, 0, 0));
		DrawString(100, 76+8, "��/���FPanel speed up or down", GetColor(0, 0, 0));
		DrawString(100, 104+8,"Enter-key:panel is rotated.", GetColor(0, 0, 0));
		DrawString(100, 132+8, "Let's connect a portion without a frame and construct a bridge. ", GetColor(0, 0, 0));
		DrawString(100, 160+8, "����鉹�̃p�l�����A���݂����̃p�l���ɂȂ��ē������܂��傤�B", GetColor(0, 0, 0));
		DrawString(100, 188+8, "�g�̖����ʂł̂݁A�Ȃ����܂��B", GetColor(0, 0, 0));
	}
	// �f�o�b�O
//	DrawFormatString( 12 , 8 , GetColor( 255, 255, 255 ) , "level:%d speed:%d set_br:%d",now_level, lv->speed, lv->road );	
//#else
	DrawFormatString( 0 , 0 , GetColor( 255, 255, 255 ) , "level:%d",now_level);
//#endif
	DrawFormatString( 12 ,  32, GetColor(255 , 255 , 255 ) , "time:%d miss:%d/%d",time, miss, lose_cnt);
	DrawFormatString( 12 ,  56, GetColor(255 , 0, 0), "red:%d",  get_rgb[0]);
	DrawFormatString( 12 ,  80, GetColor(0 , 255, 0),"green:%d", get_rgb[1]);
	DrawFormatString( 12 , 104, GetColor(0 , 0, 255),"blue:%d",  get_rgb[2]);
	return;
}

#if 0
static int ch_pats[] = {-1, 2, 6, -1, 4, 3, 5, -1, 0, 1, 7, -1, -1, -1, -1, -1, -1};
#endif
// �z��ԍ�(key)
// 0:�����@1:���@2:�E 3:�E�� 4:�� 5:���� 6:�E�� 7:�R�� 8:�� 9:���� 10:�E�� 11�F�R�c
static int ch_pats[] = {-1, 4, 3, -1, 2, 6, 8, -1, 1, 5, 7, -1, -1, -1, -1, -1, -1};
// ��������
void GameRoad::chMoveSet(void)
{
	SPRITE *sp = &spd[LO_SP_ICHI];
	int i = 0;
	if(Key[KEY_INPUT_LEFT]  > 0) i |= 1;
	if(Key[KEY_INPUT_RIGHT] > 0) i |= 2;
	if(Key[KEY_INPUT_UP]    > 0) i |= 4;
	if(Key[KEY_INPUT_DOWN]  > 0) i |= 8;

	if(ch_pats[i] == -1){ ch_walk = 0; sp->num = 1; return; }
	sp->pat = ch_pats[i];
	ch_walk = 1;
}

typedef struct{
	int x,y;
}SXY;

// �L�����ړ�
void GameRoad::chMove(void)
{
	if(ch_walk == 0){ return; }
	// �ړ���
	static SXY adds[] = {
		{ 0, 8},
		{ 0, 8},
		{ 0,-8},
		{ 8, 0},
		{-8, 0},
		{-6, 6},
		{-6,-6},
		{ 6, 6},
		{ 6,-6},
	};

	SXY add;
	SPRITE *sp = &spd[LO_SP_ICHI];
	add = adds[sp->pat];			// cpu
	int move = 0;

	
	// �ړ�
	// x���W

	ch_movetime--;
	if(ch_movetime <= 0){

	if(add.x < 0 && sp->x > 4 || add.x > 0 && sp->x < (SC_WIDTH - sp->w)){
		sp->x += add.x;
		move = 1;
	}
	// y���W
	if(add.y < 0 && sp->y > 4 || add.y > 0 && sp->y < (SC_HEIGHT - sp->h)){
		sp->y += add.y;
		move = 1;
	}
		ch_movetime = 4;
	}

	// �p�^�[���`�F���W
	if(move == 1){ 
		chReg(sp); 
		ch_time--; 
	}
	if(ch_time < 1){
#if 0
		if(sp->num < 4){ sp->num++; }
		else			 { sp->num = 1; }
#endif
		if(sp->num < I_MOVS-1){ sp->num++; }
		else			 { sp->num = 0; }
		ch_time = 2;

	}

}

// �����W�E���񂾂��̃`�F�b�N

void GameRoad::chReg(SPRITE *sp)
{
	reg.x = sp->x;
	reg.y = sp->y + sp->h;
	if(reg.y < 0){ reg.y = 0; }

	int c = 0;
	int _x, _y;
	static POINT now;
	int i;
	SPRITE *sp2 = &spd[LO_SP_BR_R1];
	if(reg.x < roadx[RO_X_CNT-1]){ 
		now.x = -1; now.y = -1; return; 
	}
	if(reg.x > roadx[0]+sp2->w){ 
		cond = LO_END; now.x = -1; now.y = -1; return; 
	}
	// ���݂�x�ʒu���Z�o
	for (i=0; i<RO_X_CNT; i++) {
		if (reg.x >= roadx[i] && reg.x < roadx[i]+sp2->w) { break; }
	}
	_x = i;

	c = 0;
	// ���݂�y�ʒu���Z�o
	for (i=0; i<RO_Y_CNT; i++) {
		if (reg.y >= roady[i] && reg.y < roady[i]+sp2->h) { break; }
		c = i;
	}
	_y = i;

	
	// ����������΃Q�[���I�[�o�[
	if(!(roads[_y][_x] & 0x0f)){
		cond = LO_GAMEOVER;
	}else{
		// ����΍Đ�
		if(now.x != _x || now.y != _y){
			PlaySoundMem(roads_se[_y][_x] , DX_PLAYTYPE_BACK);
		}
	}
	now.x = _x; now.y = _y;
}