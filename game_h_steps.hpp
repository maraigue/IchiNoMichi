#ifndef _GAME_H_STEPS_HPP_
#define _GAME_H_STEPS_HPP_

// ============================================================
// ゲーム（足場を渡り歩く）
// ============================================================

class SceneStepGame : public Scene{
protected:
	// 足場の情報
	struct StepInfo{
		int color; // 0なら足場なし。1 - 3なら足場の色
		int remained; // 0なら足場が完全に出現している。そうでなければ、足場が完全に出現するまでの時間（ミリ秒）

		// 足場としてキャラを置けるか
		int is_stepped(void){ return(color != 0 && remained == 0); }
		// 足場を即座に出現させる
		void show_step(int col){ color = col; remained = 0; }
		// 足場の出現を待機させる
		void queue_step(int col, int time){ color = col; remained = time; }
		// 足場の出現までの時間を減らす
		int proceed(int time){
			remained -= time;
			if(remained < 0) remained = 0;
			return remained;
		}
	};

	GameResult res;

	int start_time;
	int last_time;
	int elapsed;

	int cleared;
	static const int TIME_END = 3000; // ステージクリア時のメッセージを表示する時間
	int TIME_LIMIT; // 時間制限（ミリ秒）
	int STEP_INCREASE_RATE; // 足場が出る間隔（ミリ秒）

	static const int GRID_SIZE = 50;
	static const int CHARA_TOP = 50; // キャラがどれだけグリッドの上部にはみ出すか
	static const int XGRIDS = 12;
	static const int YGRIDS = 8;
	static const int FIELD_DRAW_LEFT = 100;
	static const int FIELD_DRAW_TOP = 115;
	int field_draw_left_scroll;

	int charaX, charaY;
	int cursorX, cursorY; // カーソルがキャラの位置起点でどこにあるか
	static const int MAX_RIGHTWARD_JUMP = 0; // 右に最大で何マスジャンプできるか（mapに必要）

	static const int TIME_STEP_APPEAR = 1000; // 足場が出始めてから完全に出るまでの時間
	StepInfo map[YGRIDS][XGRIDS+MAX_RIGHTWARD_JUMP];
	int mapX_offset; // 地図をスクロールするために、X座標をリングバッファ上に用いているため、そのオフセット
	int color_cursor_cand, color_cursor_cur, color_font;

	static const int jump_animation_time_max = 500;
	int jump_animation_time;
	int jump_animation_srcX, jump_animation_srcY;
	int jump_animation_dstX, jump_animation_dstY;
	int image_chara;
	int image_step[NUMBER_OF_COLORS];
	int image_foreground, image_foreground_normal[NUMBER_OF_COLORS], image_foreground_tutorial;
	int image_background, image_background_normal[NUMBER_OF_COLORS], image_background_tutorial;
	int image_tutorial;
	int sound_bgm;
	int sound_bgms[NUMBER_OF_LEVELS];
	//int sound_jump;
	
	int is_tutorial;
	
	std::list< std::pair<int, int> > rising_steps;
	
public:
	SceneStepGame(){
		image_chara = LoadGraph("res/img/step_chara.png");
		image_step[0] = LoadGraph("res/img/Rh.png");
		image_step[1] = LoadGraph("res/img/Gh.png");
		image_step[2] = LoadGraph("res/img/Bh.png");
		image_foreground_tutorial = LoadGraph("res/bg/empty.png");
		image_foreground_normal[0] = LoadGraph("res/bg/stage3_R_fg.png");
		image_foreground_normal[1] = LoadGraph("res/bg/stage3_G_fg.png");
		image_foreground_normal[2] = LoadGraph("res/bg/stage3_B_fg.png");
		image_background_tutorial = LoadGraph("res/bg/tutorial_rh.png");
		image_background_normal[0] = LoadGraph("res/bg/stage3_R_bg_rh.png");
		image_background_normal[1] = LoadGraph("res/bg/stage3_G_bg_rh.png");
		image_background_normal[2] = LoadGraph("res/bg/stage3_B_bg_rh.png");
		image_tutorial = LoadGraph("res/img/tutorial.png");
		color_cursor_cand = GetColor(255, 255, 128);
		color_cursor_cur = GetColor(255, 0, 0);
		color_font = GetColor(255, 255, 255);
		sound_bgms[0] = LoadSoundMem("res/bgm/Stage1.mp3");
		sound_bgms[1] = sound_bgms[0]
		sound_bgms[2] = LoadSoundMem("res/bgm/Stage2.mp3");
		sound_bgms[3] = LoadSoundMem("res/bgm/Stage3.mp3");
		sound_bgms[4] = sound_bgms[3]
		//sound_jump = LoadSoundMem("res/BGM2011.wav");
	}

	~SceneStepGame(){
		DeleteGraph(image_chara);
		DeleteGraph(image_step[0]);
		DeleteGraph(image_step[1]);
		DeleteGraph(image_step[2]);
		DeleteGraph(image_foreground_tutorial);
		DeleteGraph(image_foreground_normal[0]);
		DeleteGraph(image_foreground_normal[1]);
		DeleteGraph(image_foreground_normal[2]);
		DeleteGraph(image_background_tutorial);
		DeleteGraph(image_background_normal[0]);
		DeleteGraph(image_background_normal[1]);
		DeleteGraph(image_background_normal[2]);
		DeleteGraph(image_tutorial);
		DeleteSoundMem(sound_bgms[0]);
		DeleteSoundMem(sound_bgms[2]);
		DeleteSoundMem(sound_bgms[3]);
	}

	StepInfo & getMapValue(int x, int y){
		return map[y][(x - mapX_offset) % (XGRIDS+MAX_RIGHTWARD_JUMP)];
	}
	
	void init(GameResult * result){
		start_time = GetNowCount();
		last_time = 0;
		jump_animation_time = 0;
		rising_steps.clear();
		field_draw_left_scroll = 0;
		mapX_offset = 0;
		cleared = 0;
		is_tutorial = FALSE;

		image_background = image_background_normal[result ? result->stage_color() : 0];
		image_foreground = image_foreground_normal[result ? result->stage_color() : 0];
		sound_bgm = sound_bgms[result ? result->stage_level() : 0];
		TIME_LIMIT = result ? (5000 * (11 - result->stage_level())) : 100;
		STEP_INCREASE_RATE = result ? (50 * (16 + result->stage_level())) : 1000;
		
		res.end_status(0);
		res.r_score(0);
		res.g_score(0);
		res.b_score(0);

		for(int j = 0; j < YGRIDS; ++j){
			for(int i = 0; i < 2; ++i) getMapValue(i, j).show_step(int_rand(1, 4));
			for(int i = 2; i < XGRIDS+MAX_RIGHTWARD_JUMP; ++i) getMapValue(i, j).show_step(0);
		}
		charaX = 1;
		charaY = 3;
		cursorX = 0;
		cursorY = 0;

		ChangeVolumeSoundMem(255, sound_bgm);
		PlaySoundMem(sound_bgm, DX_PLAYTYPE_LOOP);
	}

	void leave(){
		StopSoundMem(sound_bgm);
	}
	
	GameResult * update(void){
		int current_time = GetNowCount();
		int diff_time;
		
		if(last_time == 0){
			diff_time = 0;
		}else{
			diff_time = current_time - last_time;
		}
		last_time = current_time;
		elapsed = current_time - start_time;

		// ゲームクリアの場合
		if(cleared){
			if(cleared <= diff_time){
				leave();
				res.end_status(1);
				return &res;
			}else{
				// 音量フェードアウト
				cleared -= diff_time;
				ChangeVolumeSoundMem(cleared * 255 / TIME_END, sound_bgm);
				return NULL;
			}
		}

		// 時間制限終了の場合
		if(elapsed >= TIME_LIMIT){
			if(elapsed >= TIME_LIMIT + TIME_END){
				leave();
				res.end_status(0);
				return &res;
			}
			// 音量フェードアウト
			ChangeVolumeSoundMem((TIME_LIMIT + TIME_END - elapsed) * 255 / TIME_END, sound_bgm);
			return NULL;
		}

		// 足場を出す
		for(std::list< std::pair<int, int> >::iterator it = rising_steps.begin(); it != rising_steps.end(); ){
			if(getMapValue(it->first, it->second).proceed(diff_time) == 0){
				// 足場が完全に出切った場合
				it = rising_steps.erase(it);
			}else{
				// 足場がまだ出切ってない場合
				++it;
			}
		}

		// 足場を増やす（STEP_INCREASE_RATEミリ秒に1個の割合）
		if(int_rand(0, STEP_INCREASE_RATE) < diff_time){
			int steppos_x = XGRIDS;
			int steppos_y = YGRIDS;
			int steppos_count = 6;
			while(!(steppos_x < XGRIDS && steppos_y >= 0 && steppos_y < YGRIDS && getMapValue(steppos_x, steppos_y).color == 0) && steppos_count > 0){
				steppos_x = charaX + int_rand(0, 3);
				steppos_y = int_rand(0, YGRIDS);
				--steppos_count;
			}
			
			if(steppos_count > 0){
				getMapValue(steppos_x, steppos_y).queue_step(int_rand(1, 4), TIME_STEP_APPEAR);
				rising_steps.push_back(std::make_pair(steppos_x, steppos_y));
			}
		}
		
		if(jump_animation_time == 0){
			// キーを受け取る
			int new_cursorX = cursorX, new_cursorY = cursorY;
			int x, y;
			
			switch(GetInputChar(TRUE)){
				case CTRL_CODE_UP:
					--new_cursorY;
					break;
				case CTRL_CODE_LEFT:
					--new_cursorX;
					break;
				case CTRL_CODE_DOWN:
					++new_cursorY;
					break;
				case CTRL_CODE_RIGHT:
					++new_cursorX;
					break;
				case CTRL_CODE_CR: case 'z':
					x = charaX + cursorX;
					y = charaY + cursorY;
					if(!(cursorX == 0 && cursorY == 0) && is_in_map(x, y) && getMapValue(x, y).is_stepped()){
						// ジャンプする
						jump_animation_srcX = charaX;
						jump_animation_srcY = charaY;
						jump_animation_dstX = x;
						jump_animation_dstY = y;
						charaX = x;
						charaY = y;
						jump_animation_time = jump_animation_time_max;
						switch(getMapValue(x, y).color){
							case 1:
								res.r_score(res.r_score() + 1);
								break;
							case 2:
								res.g_score(res.g_score() + 1);
								break;
							case 3:
								res.b_score(res.b_score() + 1);
								break;
						}
					}
					break;
			}
			// カーソルが移動可能な位置を指している場合は、そこにカーソルを動かす
			if(is_in_map(charaX + new_cursorX, charaY + new_cursorY) && abs(new_cursorX) + abs(new_cursorY) <= 2){
				cursorX = new_cursorX;
				cursorY = new_cursorY;
			}
		}else if(jump_animation_time > 0){
			// ジャンプ中（キー操作は受け付けない）
			if(jump_animation_time <= diff_time){
				// ジャンプ終了
				jump_animation_time = 0;
				
				// マップをずらす
				//mapX_offset += jump_animation_dstX - jump_animation_srcX;
				
				// カーソルをずらす
				cursorX = 0;
				cursorY = 0;

				// クリア判定
				if(charaX == XGRIDS - 1) cleared = TIME_END;
			}else{
				jump_animation_time -= diff_time;
			}
		}
		return NULL;
	}

	int is_in_map(int i, int j){
		return(i >= 0 && i < XGRIDS+MAX_RIGHTWARD_JUMP && j >= 0 && j < YGRIDS);
	}
	
	int xpos(int i){
		return FIELD_DRAW_LEFT + field_draw_left_scroll + GRID_SIZE * i;
	}

	int ypos(int j){
		return FIELD_DRAW_TOP + GRID_SIZE * j;
	}

	void showStep(int i, int j, const StepInfo & mode){
		if(mode.color == 0){
			// Do nothing
		}else{
			if(mode.remained == 0){
				DrawGraph(xpos(i), ypos(j), image_step[mode.color - 1], TRUE);
			}else{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (TIME_STEP_APPEAR - mode.remained) * 255 / TIME_STEP_APPEAR * 4 / 5);
				DrawGraph(xpos(i), ypos(j), image_step[mode.color - 1], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}

	void showCursor(int i, int j, int color){
		DrawBox(xpos(i), ypos(j), xpos(i) + GRID_SIZE, ypos(j) + GRID_SIZE, color, TRUE);
	}

	void draw(void){
		//if(jump_animation_time == 0){
		//	field_draw_left_scroll = 0;
		//}else{
		//	field_draw_left_scroll = -(jump_animation_dstX - jump_animation_srcX) * (jump_animation_time_max - jump_animation_time) * GRID_SIZE / jump_animation_time_max;
		//}
		
		// ---------- 背景 ----------
		DrawGraph(0, 0, image_background, TRUE);
		
		// ---------- マップ ----------
		for(int j = 0; j < YGRIDS; ++j){
			for(int i = 0; i < XGRIDS; ++i){
				showStep(i, j, getMapValue(i, j));
			}
		}
		
		// ---------- カーソルを動かせる範囲 ----------
		if(jump_animation_time == 0){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
			
			for(int j = charaY - 2; j <= charaY + 2; ++j){
				for(int i = charaX - (2 - abs(j - charaY)); i <= charaX + 2 - abs(j - charaY); ++i){
					if(is_in_map(i, j)){
						showCursor(i, j, color_cursor_cand);
					}
				}
			}
		}
		
		// ---------- 現在のカーソル ----------
		if(jump_animation_time == 0){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			showCursor(charaX+cursorX, charaY+cursorY, color_cursor_cur);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ---------- キャラ ----------
		if(jump_animation_time == 0){
			DrawGraph(xpos(charaX), ypos(charaY) - CHARA_TOP, image_chara, TRUE);
		}else{
			int ratio2 = jump_animation_time_max - jump_animation_time;
			int jump_height = 4 * jump_animation_time * (jump_animation_time_max - jump_animation_time) * GRID_SIZE / jump_animation_time_max;
			DrawGraph(
				(xpos(jump_animation_srcX) * jump_animation_time + xpos(jump_animation_dstX) * ratio2) / jump_animation_time_max,
				(ypos(jump_animation_srcY) * jump_animation_time + ypos(jump_animation_dstY) * ratio2 - jump_height) / jump_animation_time_max - CHARA_TOP,
				image_chara, TRUE);
		}

		// ---------- 前景 ----------
		DrawGraph(0, 0, image_foreground, TRUE);

		// ---------- ステージクリア、残り時間ゲーム失敗 ----------
		SetFontSize(48);
		ChangeFontType(DX_FONTTYPE_EDGE);
		if(!is_tutorial){
			if(cleared){
				DrawString(0, 0, "Cleared!", color_font);
			}else{
				if(elapsed >= TIME_LIMIT){
					DrawString(0, 0, "Failed...", color_font);
				}else{
					DrawFormatString(0, 0, color_font, "Time: %ds R%d G%d B%d", (TIME_LIMIT - elapsed)/1000, res.r_score(), res.g_score(), res.b_score());
				}
			}
		}
	}
};

// ============================================================
// チュートリアル
// ============================================================

class SceneStepGameTutorial : public SceneStepGame{
	int blink;
	int step1_added, step1_jumped1, step1_jumped2, step2_added, step2_jumped;
	
public:
	void init(GameResult * result){
		SceneStepGame::init(NULL);
		image_background = image_background_tutorial;
		image_foreground = image_foreground_tutorial;
		step1_added = FALSE;
		step2_added = FALSE;
		step1_jumped1 = FALSE;
		step1_jumped2 = FALSE;
		step2_jumped = FALSE;
		is_tutorial = TRUE;
	}

	void cursor_sample(int time){
		if(time < 500){
			cursorX = 0; cursorY = -1;
		}else if(time < 1000){
			cursorX = 0; cursorY = -2;
		}else if(time < 1500){
			cursorX = 0; cursorY = -1;
		}else if(time < 2000){
			cursorX = 0; cursorY = 0;
		}else if(time < 2500){
			cursorX = 1; cursorY = 0;
		}else if(time < 3000){
			cursorX = 2; cursorY = 0;
		}else if(time < 3500){
			cursorX = 1; cursorY = 0;
		}else{
			cursorX = 0; cursorY = 0;
		}
	}

	GameResult * update(void){
		elapsed = GetNowCount() - start_time;
		
		// キーが押された場合、時計を進め、チュートリアル終了を早める
		switch(GetInputChar(TRUE)){
			case CTRL_CODE_CR: case 'z':
				start_time -= (29000 - elapsed);
				elapsed = 29000;
				break;
		}
		
		blink = (elapsed > 6000) ? 0 : abs((elapsed / 5) % 510 - 255);
		
		if(elapsed < 3000){
			charaX = 1; charaY = 3;
			cursorX = 0; cursorY = 0;
		}else if(elapsed < 7000){
			cursor_sample(elapsed - 3000);
		}else if(elapsed < 11000){
			cursor_sample(elapsed - 7000);
		}else if(elapsed < 12000){
			cursor_sample(elapsed - 11000);
		}else if(elapsed < 14000){
			// Do nothing
		}else if(elapsed < 14000 + TIME_STEP_APPEAR){
			getMapValue(3, 2).queue_step(1, 14000 + TIME_STEP_APPEAR - elapsed);
		}else if(elapsed < 15500){
			getMapValue(3, 2).show_step(1);
			// Do nothing
		}else if(elapsed < 16200){
			cursorX = 0; cursorY = -1;
		}else if(elapsed < 16400){
			cursorX = -1; cursorY = -1;
		}else if(elapsed < 16600){
			cursorX = -1; cursorY = 0;
		}else if(elapsed < 16800){
			cursorX = 0; cursorY = 0;
		}else if(elapsed < 17000){
			cursorX = 1; cursorY = 0;
		}else if(elapsed < 17200){
			cursorX = 1; cursorY = -1;
		}else if(elapsed < 17500){
			cursorX = 0; cursorY = -1;
		}else if(elapsed < 17500 + jump_animation_time_max){
			if(!step1_jumped1){
				step1_jumped1 = TRUE;
				jump_animation_srcX = charaX;
				jump_animation_srcY = charaY;
				jump_animation_dstX = charaX + cursorX;
				jump_animation_dstY = charaY + cursorY;
				charaX = jump_animation_dstX;
				charaY = jump_animation_dstY;
				cursorX = 0;
				cursorY = 0;
			}
			jump_animation_time = 17500 + jump_animation_time_max - elapsed;
		}else if(elapsed < 18500){
			jump_animation_time = 0;
			// Do nothing
		}else if(elapsed < 19250){
			cursorX = 0; cursorY = -1;
		}else if(elapsed < 19500){
			cursorX = 1; cursorY = -1;
		}else if(elapsed < 19750){
			cursorX = 1; cursorY = 0;
		}else if(elapsed < 20000){
			cursorX = 2; cursorY = 0;
		}else if(elapsed < 20000 + jump_animation_time_max){
			if(!step1_jumped2){
				step1_jumped2 = TRUE;
				
				jump_animation_srcX = charaX;
				jump_animation_srcY = charaY;
				jump_animation_dstX = charaX + cursorX;
				jump_animation_dstY = charaY + cursorY;
				charaX = jump_animation_dstX;
				charaY = jump_animation_dstY;
				cursorX = 0;
				cursorY = 0;
			}
			jump_animation_time = 20000 + jump_animation_time_max - elapsed;
		}else if(elapsed < 21000){
			jump_animation_time = 0;
			// Do nothing
		}else if(elapsed < 21000 + TIME_STEP_APPEAR){
			getMapValue(5, 2).queue_step(2, 21000 + TIME_STEP_APPEAR - elapsed);
		}else if(elapsed < 22500){
			getMapValue(5, 2).show_step(2);
			// Do nothing
		}else if(elapsed < 22900){
			cursorX = 0; cursorY = 1;
		}else if(elapsed < 23100){
			cursorX = 1; cursorY = 1;
		}else if(elapsed < 23300){
			cursorX = 1; cursorY = 0;
		}else if(elapsed < 23500){
			cursorX = 2; cursorY = 0;
		}else if(elapsed < 23500 + jump_animation_time_max){
			if(!step2_jumped){
				step2_jumped = TRUE;
				
				jump_animation_srcX = charaX;
				jump_animation_srcY = charaY;
				jump_animation_dstX = charaX + cursorX;
				jump_animation_dstY = charaY + cursorY;
				charaX = jump_animation_dstX;
				charaY = jump_animation_dstY;
				cursorX = 0;
				cursorY = 0;
			}
			jump_animation_time = 23500 + jump_animation_time_max - elapsed;
		}else if(elapsed < 30000){
			jump_animation_time = 0;
			ChangeVolumeSoundMem((30000 - elapsed) * 255 / 5500, sound_bgm);
		}else{
			SceneStepGame::leave();
			return &res;
		}
		return NULL;
	}

	void draw(void){
		SceneStepGame::draw();
		if(blink > 0){
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, blink);
			DrawGraph(100, 200, image_tutorial, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
};

#endif // _GAME_H_STEPS_HPP_
