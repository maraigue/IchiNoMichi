/*
	by rocca
	   -> 2013/02/11
*/

#pragma once
#include <DxLib.h>
#include "game_r_road.h"
#include "game_r_road_datas.h"
#include "scene.hpp"
GameResult *rd_g_res;
GameRoad::GameRoad(void)
{	
}

GameRoad::~GameRoad(void)
{
	DataDel();
}

//　データ破棄
void GameRoad::DataDel(void)
{
	for(int i=0; i<_RO_SP_END; i++){
		DeleteGraph( sp_num[i] ) ;
	}
	for(int i=0; i<8; i++){
		for(int j=0; j<5; j++){
			DeleteGraph( ichi_num[i][j] );
		}
	}
	for(int i=0; i<_RO_SE_END; i++){
		DeleteSoundMem( se_num[i] );
	}
}

//　データ読み込み
void GameRoad::DataLoad(void)
{
	int i;
	if(s_col < 0){ s_col = 0; }
	int c = s_col*5 + stage;
	spd[RO_SP_BG].num = LoadGraph(pix_bgs[c]);
	spd[RO_SP_FB].num = LoadGraph(pix_fgs[c]);

	// 画像ロード
	for(i=RO_SP_BR_R1; i<_RO_SP_END; i++){
		spd[i].num = LoadGraph(ro_pixs[i]);
		spd[i].pat = 0;
	}
			

	// 効果音ロード
	for(i=0; i<_RO_SE_END; i++){
		se_num[i] = LoadSoundMem(sounds[i]);
	}

	// いちパターン画像
	int tmp[I_PATMAX];
	int i_c = 0;
	LoadDivGraph(ro_pixs[RO_SP_ICHI], I_PATMAX, I_MOVS, I_PATS, spd[RO_SP_ICHI].w, spd[RO_SP_ICHI].h,tmp);
	for(i=0; i<I_PATS; i++){
		for(int j=0; j<I_MOVS; j++){
			ichi_num[i][j] = tmp[i_c];
			i_c++;
		}
	}
	spd[RO_SP_ICHI].num = spd[RO_SP_ICHI].pat = 0;
	spd[RO_SP_ICHI].x = 12;
	spd[RO_SP_ICHI].y = 300;
}

void GameRoad::init(GameResult * result)
{
	rd_g_res = result;
	cond = RO_INI;
	move_time = 0;

	time = 0;
	miss = 0;
	for(int i=0; i<3; i++){
		get_rgb[i] = 0;
	}
	all_road = 0;
	head_x = RO_X_CNT;
	now_level = 0;

	del_block.ok = 0;
	del_block.type = 0;
	del_block.col = 0;
	block.ok = 0;
	block.type = 0;
	block.col = rand() % 3;
	once_btn = 0;

	ch_movetime = 4;
	ch_time = 4;
	ch_mode = 0;
	ch_walk = 0;

	alpha = 255;
	SetDrawBright( alpha , alpha , alpha );
	SetFontSize(16);
	if(result == NULL){
		s_col = 0;
		stage = 0;
	}else{
		s_col = result->stage_color();
		stage = result->stage_level();
	}

	efe_ok = 0;
}


// 新規ブロック生成
void GameRoad::newBlock()
{	
	// i = 新規ブロック情報格納場所
	if(block.ok == 1){ return; }
	if(efe_ok == 1){ return; }
	// 出せるブロック判定
	// 0x00:横
	// 0x01(0x04):上
	// 0x02(0x08):下
	int set_t = 0;
	int c = 0x02, b = 0, bb = 0;
	// 最前x検索
	if(head_x > -1){
		for(int i=0; i<RO_Y_CNT; i++){
			if(roads[i][head_x]!=0){
				
				c |= 1;
				if(c & 2 && b > 1 && bb > 0){ c |= 0x08; c &= ~0x02; }
				b = bb = 0;
			}else{
				if(head_x != RO_Y_CNT -1){
					if(roads[i][head_x+1]==0){ bb++; }
				}
				b++;
				if(b>1 && c&1 && bb > 0){ c |= 0x04; }
			}
		}
	}
	set_t = c>>2;
	// 条件検索
	while(1){
		b = rand() % 3;
		if(b == 0 || b & set_t){ break; }
	}
	block.type = b;
	// セット
	int set_k = rand() % RO_Y_CNT;
	block.k = set_k;
	block.ok = 1;
	block.x = 800;
	block.col = rand() % 3;	// R G B
}

void  GameRoad::efeSet(int mode, int pos)
{
	int n, t, _x, _y;

	_x = block.x - spd[RO_SP_OK1].w / 2; 
	_y = roady[block.k];

	if(pos != 1){ _x -= spd[RO_SP_OK1].w / 4; }
	if(pos == 2){ _y -= spd[RO_SP_OK1].h / 2; }
	if(pos == 3){ _y += spd[RO_SP_OK1].h / 2; }

	if(mode == 0){ n = 2; t =  RO_SP_NG1; }
	else { n = 5; t = RO_SP_OK1; }

	for(int i=0; i<n; i++){
			spd[t+i].x = _x;
			spd[t+i].y = _y;
		}

}
void GameRoad::Failureroad(void)
{
	// 落ちブロック失敗チェック
	if(block.ok == 1){
		block.ok = -1;
		del_block = block;
		del_block.ok = 255;
		miss++;
	}
}
// キー判定
void GameRoad::roadSetCheck(void)
{

	int c = 0;
	int i;
	int n = -1;
	if(block.ok != 1){ return; }			// 落下中では無い

	int to_up, to_down, to_left = 0;
	SPRITE *sp = &spd[RO_SP_BR_R1];
	RO_LEVEL *lv = &levels[now_level];	// 現在レベル
	// 現在のx位置を算出
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

	// 左
	if(n == RO_X_CNT-1){ to_left = 1; }
	else{ to_left = roads[block.k][n+1] & 0x0f; } 

	int res = 0;
	// 積立成功
	// 左
	if((block.x <= roadx[n]+lv->speed) && to_left){
		if(block.type == 0 && !to_up && !to_down){
			res = 1;
		}else{
			res = -1;
		}
	}

	// 上
	if(to_up){
		if(block.type == 1 && (!to_down && !to_left || n == RO_X_CNT-1)){ 
			res = 2;
		}else{
			res = -2;
		}
	}
	// 下
	if(to_down){
		if(block.type == 2 && (!to_left && !to_up || n == RO_X_CNT-1)){ 
			res = 3;
		}else{
			res = -3;
		}
	}

	if(res > 0){
		if(head_x > n){ head_x = n; }
		roads[block.k][n] |= block.type+1;
		roads[block.k][n] |= block.col<<4;
		block.ok = 0;
		all_road++;
		get_rgb[block.col]++;
		int sn = ses[block.col]+(rand()%8);
		int se = se_num[sn];
		PlaySoundMem(se , DX_PLAYTYPE_BACK);
		roads_se[block.k][n] = se;
		efeSet(1, res);
		efe_ok = 1;
	}else if(res < 0){
		Failureroad();
		efeSet(0, -res);
	}
}

int GameRoad::clearCheck()
{
	for(int i=0; i<RO_Y_CNT; i++){
		int n = roads[i][RO_X_CNT] & ~0xf0;
		if(roads[i][0] != 0 ){ return i; }
	}
	return -1;
}

static void BGMStop()
{
	for(int i = RO_BGM_1; i<= RO_BGM_T; i++){
		StopSoundMem(se_num[i]);
	}
}
GameResult * GameRoad::update(void)
{
	int c = 0;	//該当ブロックカウント
	int i;

	RO_LEVEL *lv = &levels[now_level];
	GetHitKeyStateAll( Key ) ;

	switch(cond){
		case RO_GAMEOVER:
			// キー待ち後遷移
			if(Key[KEY_INPUT_Y] == 1){ init(rd_g_res); }
			if(Key[KEY_INPUT_N] == 1){ // 終了
				ro_g_res.end_status(0);
				return &ro_g_res; 
			}
		break;
		case RO_END:
			ro_g_res.end_status(1);
			return &ro_g_res; // 平常終了

		// 初期化
		case RO_INI:{

			DataLoad();
			cond = RO_GAME_S;

			for(i=0; i<RO_Y_CNT; i++){
				for(int j=0; j<RO_X_CNT; j++){
					roads[i][j] = 0;
					roads_se[i][j] = 0;
				}
			}
			PlaySoundMem(se_num[RO_SE_START] , DX_PLAYTYPE_BACK);
			break;
		}
		// ゲーム開始
		case RO_GAME_S:

			if(CheckSoundMem(se_num[RO_SE_START]) != 1){
				cond = RO_GAME;
				PlaySoundMem(se_num[bgms[stage]] , DX_PLAYTYPE_LOOP);
			}
			break;
		// ゲーム中
		case RO_GAME:
			if(clearCheck() > -1){ cond = RO_CLEAR; BGMStop(); break;}

			time++;
			// レベルチェック
			lv++;
			if(lv->road != -1 && lv->road <= all_road){
				now_level++;
				lv++;
			}else{
				lv--;
			}

			move_time++;

			if(move_time >= 6){
				move_time = 0;
				// 落下
				c = 0;
				if(block.ok == 1){
					block.x -= lv->speed;
				}
			}

			// 新規ブロック生成
			newBlock();

			// 方向キー
			// 先へ
			if(Key[KEY_INPUT_LEFT] > 0){
				move_time++;
			}
			// 停滞
			if(Key[KEY_INPUT_RIGHT] > 0){
				move_time--;
			}
			// 位置移動(上)
			if(Key[KEY_INPUT_UP] == 1){
				if(!(once_btn & 1)){
					block.k--;
					if(block.k < 0){ block.k = 0; } 
					once_btn |= 1;
				}
			}else{ once_btn &= ~1; }

			// 位置移動(下)
			if(Key[KEY_INPUT_DOWN] == 1){
				if(!(once_btn & 2)){
					block.k++;
					if(block.k > RO_Y_CNT-1){ block.k = RO_Y_CNT-1; } 
					once_btn |= 2;
				}
				
			}else{ once_btn &= ~2; }
			// 向き変更キー
			if(Key[KEY_INPUT_RETURN] == 1 || Key[KEY_INPUT_Z] == 1){
				if(!(once_btn & 4)){
					block.col++;
					if(block.col > 2){ block.col = 0; }
				}
				once_btn |= 4;
			}else{ once_btn &= ~4; }

			roadSetCheck();

		break;

		// game clear
		case RO_CLEAR:
			if(ch_mode == 1){
				chMoveSet();
				chMove();
			}else{
				PlaySoundMem(se_num[RO_SE_CLEAR] , DX_PLAYTYPE_BACK);
			}
			ch_mode = 1;
		break;
		
	}

	return 0;
}

void shine(SPRITE *sp)
{
	if(sp->efe > 0){ sp->pat += 2; }
	else { sp->pat -= 2; }

	if(sp->pat > 255){ sp->pat = 255; sp->efe = 0; }
	if(sp->pat < 0){ sp->pat = 0; sp->efe = 1; }
}

void GameRoad::draw(void)
{
	SPRITE *sp;
	int c = 0;
	if(cond == RO_INI){ return; }
	if(del_block.ok > 0){
		SetDrawBlendMode( DX_BLENDMODE_INVSRC, 128);
	}

	// 背景
	sp = &spd[RO_SP_BG];
	DrawGraph(sp->x, sp->y, sp->num, TRUE);
	
	// 落ちるブロック描画
	sp = &spd[cols[block.col]+block.type];
	
	if(block.ok == 1){
		DrawGraph(block.x, roady[block.k], sp->num, TRUE);
	}
	
	// 現在埋まっているブロックを描画
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


	if(cond == RO_GAMEOVER){

		// ゲームオーバー
		SetDrawBright( 255 , 255 , 255 );
		for(int i=RO_SP_LOSE1; i<=RO_SP_LOSE4; i++){
			sp = &spd[i];
			DrawGraph(sp->x, sp->y, sp->num, TRUE);
		}
		SetDrawBright( alpha , alpha , alpha );

		alpha -= 8;
		if(alpha < 128){ alpha = 128; }

		return;
	}

	// いち
	sp = &spd[RO_SP_ICHI];
	DrawGraph(sp->x, sp->y, ichi_num[sp->pat][sp->num], TRUE);

	sp = &spd[RO_SP_FB];
	DrawGraph(sp->x, sp->y, sp->num, TRUE);

	RO_LEVEL *lv = &levels[now_level];	// 現在レベル

	// 失敗ブロック描画
	if(del_block.ok > 0){
		del_block.ok -= 8;
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0);
	}

	if(del_block.ok <= 0){ del_block.ok = 0;}
	else{
		int n = del_block.type;
		sp = &spd[cols[del_block.col] + n];
		SetDrawBlendMode( DX_BLENDMODE_ALPHA,  del_block.ok );	//ブレンドモード
		DrawRotaGraph( del_block.x, roady[del_block.k], 1.0,  0.0, sp->num, TRUE );

		// エフェクト描画
		if(del_block.ok > 184){	sp = &spd[RO_SP_NG1]; }
		else{ sp = &spd[RO_SP_NG2]; }
		DrawGraph(sp->x, sp->y, sp->num, TRUE);

		SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );		//ブレンドモードをオフ
	}

	if(efe_ok > 0){
		int e = efe_ok / 8;
		if( e > 4 ){ efe_ok = 0; return; }
		
		sp = &spd[RO_SP_OK1+e];
		DrawGraph(sp->x, sp->y, sp->num, TRUE);
		efe_ok++;
	}

	if(cond == RO_GAME_S){
		if(stage == 0){ sp = &spd[RO_SP_T1]; }
		else { sp = &spd[RO_SP_T2]; }
		
		shine(sp);

		SetDrawBlendMode( DX_BLENDMODE_ALPHA,  sp->pat);
		DrawGraph(sp->x, sp->y, sp->num, TRUE);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND,  0 );
		
	}
#ifdef _DEBUG
	// クリア描画
	if(cond == RO_CLEAR){
		if(stage == 0){
			SetFontSize(24);
			DrawFormatString( 420 , 8 , GetColor( 0 , 0 , 0 ), "Ichi can walk! (←/↓/↑/→)");
			SetFontSize(20);
			DrawFormatString( 100, 48, GetColor(0, 0, 0), 
				"Let's cross to the opposite shore. Or let's fall from a bridge. " );
			DrawFormatString( 100, 76, GetColor(0, 0, 0), 
				"対岸へ渡りましょう。無理なら橋から落ちましょう。" );
		}
	}else if(stage == 0){
		DrawString(100, 48+8, "↑/↓：Panel up down", GetColor(0, 0, 0));
		DrawString(100, 76+8, "←/→：Panel speed up or down", GetColor(0, 0, 0));
		DrawString(100, 104+8,"Enter-key:panel is rotated.", GetColor(0, 0, 0));
		DrawString(100, 132+8, "Let's connect a portion without a frame and construct a bridge. ", GetColor(0, 0, 0));
		DrawString(100, 160+8, "流れる音のパネルを、左岸か他のパネルにつなげて道を作りましょう。", GetColor(0, 0, 0));
		DrawString(100, 188+8, "枠の無い面でのみ、つなげられます。", GetColor(0, 0, 0));
	}

#else
	DrawFormatString( 0 , 0 , GetColor( 255, 255, 255 ) , "level:%d",now_level);
#endif
	DrawFormatString( 12 ,  32, GetColor(255 , 255 , 255 ) , "time:%d miss:%d",time, miss);
	DrawFormatString( 12 ,  56, GetColor(255 , 0, 0), "red:%d",  get_rgb[0]);
	DrawFormatString( 12 ,  80, GetColor(0 , 255, 0),"green:%d", get_rgb[1]);
	DrawFormatString( 12 , 104, GetColor(0 , 0, 255),"blue:%d",  get_rgb[2]);
	return;
}

static int ch_pats[] = {-1, 4, 3, -1, 2, 6, 8, -1, 1, 5, 7, -1, -1, -1, -1, -1, -1};
void GameRoad::chMoveSet(void)
{
	SPRITE *sp = &spd[RO_SP_ICHI];
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

// キャラ移動
void GameRoad::chMove(void)
{
	if(ch_walk == 0){ return; }
	// 移動率
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
	SPRITE *sp = &spd[RO_SP_ICHI];
	add = adds[sp->pat];
	int move = 0;

	ch_movetime--;
	if(ch_movetime <= 0){

	if(add.x < 0 && sp->x >= 4 || add.x > 0 && sp->x <= (SC_WIDTH - sp->w)){
		sp->x += add.x;
		move = 1;
	}

	if(add.y < 0 && sp->y >= 4 || add.y > 0 && sp->y <= (SC_HEIGHT - sp->h)){
		sp->y += add.y;
		move = 1;
	}
		ch_movetime = 4;
	}

	if(move == 1){ 
		chReg(sp); 
		ch_time--; 
	}
	if(ch_time < 1){
		if(sp->num < I_MOVS-1){ sp->num++; }
		else			 { sp->num = 0; }
		ch_time = 2;

	}
}

// 足座標・踏んだものチェック
void GameRoad::chReg(SPRITE *sp)
{
	// 足元
	reg.x = sp->x;
	reg.y = sp->y + sp->h - 4;
	if(reg.y < 0){ reg.y = 0; }

	int c = 0;
	int _x, _y;
	static POINT now;
	int i;
	SPRITE *sp2 = &spd[RO_SP_BR_R1];
	if(reg.x < roadx[RO_X_CNT-1]){ 
		now.x = -1; now.y = -1; return; 
	}
	// Goal
	if(reg.x >= roadx[0]+sp2->w){ 
		cond = RO_END; now.x = -1; now.y = -1; return; 
	}

	// 現在のx位置を算出
	for (i=0; i<RO_X_CNT; i++) {
		if (reg.x >= roadx[i] && reg.x < roadx[i]+sp2->w) { break; }
	}
	_x = i;

	c = 0;
	// 現在のy位置を算出
	for (i=0; i<RO_Y_CNT; i++) {
		if (reg.y >= roady[i] && reg.y < roady[i]+sp2->h+4) { break; }
		c = i;
	}
	_y = i;

	
	if(!(roads[_y][_x] & 0x0f)){
		cond = RO_GAMEOVER;
	}else{
		// あれば再生
		if(now.x != _x || now.y != _y){
			PlaySoundMem(roads_se[_y][_x] , DX_PLAYTYPE_BACK);
		}
	}
	now.x = _x; now.y = _y;
}
