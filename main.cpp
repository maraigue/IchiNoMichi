#include "DxLib.h"
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <list>
#include <utility>

#include "utils.hpp"
#include "scene.hpp"
#include "game_h_panels.hpp"
#include "game_h_steps.hpp"
#include "scene_h_map.hpp"

#include "game_y_Darkroom.h"

#include "game_r_road.h"

// ============================================================
// ゲーム（ダミー）
// ============================================================

class SceneDummyGame : public Scene{
	const char * stage_name;
	int image;
	int start_time;
	int elapsed;
	int key_pressed;
	static const int TIME_END = 2000;
	GameResult res;

public:
	SceneDummyGame(const char * text, const char * fname = NULL){
		stage_name = text;
		if(fname){
			image = LoadGraph(fname);
		}else{
			image = -1;
		}
	}
	
	void init(GameResult * result){
		
		if(result) res = *result;
		res.end_status(0);
		key_pressed = 0;
	}
	
	GameResult * update(void){
		if(!key_pressed && GetInputChar(TRUE) == CTRL_CODE_CR){
			key_pressed = 1;
			start_time = GetNowCount();
		}
		
		if(key_pressed){
			elapsed = GetNowCount() - start_time;
			if(elapsed >= TIME_END){
				res.end_status(1);
				return &res;
			}
		}else{
			elapsed = 0;
		}

		return NULL;
	}

	void draw(void){
		int color_value = 255 * elapsed / TIME_END;
		DrawBox(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GetColor(color_value, color_value, color_value), TRUE);

		if(image != -1){
			DrawGraph(0, 0, image, TRUE);
		}else{
			SetFontSize(32);
			ChangeFont("Century");

			int y = 100;
			const char * ptr = stage_name;
			while(*ptr){
				DrawString(100, y, ptr, GetColor(255, 255, 255));
				ptr += strlen(ptr) + 1;
				y += 40;
			}
		}
	}
};

// ============================================================
// ゲーム全体
// ============================================================

const int SCENES = 34;
const int FIRST_TUTGAME_ID = 3;
const int FIRST_MAINGAME_ID = 6;
const int FIRST_PROLOG_ID = 15;
const int FIRST_EPILOG_ID = 24;
const int FIRST_ENDING_ID = 33;
inline int IS_AFTER_OPENING(int n){ return(n == 0); }
inline int IS_AFTER_TUTORIAL_GAME(int n){ return(n >= FIRST_TUTGAME_ID && n < FIRST_MAINGAME_ID); }
inline int IS_AFTER_MAIN_GAME(int n){ return(n >= FIRST_MAINGAME_ID && n < FIRST_PROLOG_ID); }

GameResult PlayerStatus;
Scene * SelectedGame = NULL;

void define_scenes(Scene ** sc){
	sc[0] = new SceneDummyGame("", "res/bg/__opening.png");
	sc[1] = new SceneSelectStage<StageDefTutorial>();
	sc[2] = new SceneSelectStage<StageDefMap>();
	
	sc[3] = new SceneStepGameTutorial();
	sc[4] = new SceneDummyGame("Game \"Dark Room\" Tutorial\0");
	sc[5] = new SceneDummyGame("Game \"Road\" Tutorial\0"); // new GameRoad();
	
	const char * story1 = "A shade seen in a forest\0Her name is Ichi\0She is shaking her head\0as she lost her way\0Now she is walking around\0to get out here\0";
	const char * story2 = "Somebody over there\0Ichi shouted with full of her voice\0for asking her way\0But the voice not reached the person\0and she went away and away\0Ichi begin running for him\0";
	const char * story3 = "Ichi found something on the ground\0a cute red ribbonShe wonder why it is here\0as nobody is here\0She thinks it is probably owned\0by the person over there\0Ichi continues running for the person\0";

	sc[15] = new SceneDummyGame(story1);
	sc[16] = new SceneDummyGame(story1);
	sc[17] = new SceneDummyGame(story1);
	sc[18] = new SceneDummyGame(story2);
	sc[19] = new SceneDummyGame(story2);
	sc[20] = new SceneDummyGame(story2);
	sc[21] = new SceneDummyGame(story3);
	sc[22] = new SceneDummyGame(story3);
	sc[23] = new SceneDummyGame(story3);
	
	sc[24] = new SceneDummyGame("Stage 1 Cleared!\0");
	sc[25] = new SceneDummyGame("Stage 1 Cleared!\0");
	sc[26] = new SceneDummyGame("Stage 1 Cleared!\0");
	sc[27] = new SceneDummyGame("Stage 2 Cleared!\0");
	sc[28] = new SceneDummyGame("Stage 2 Cleared!\0");
	sc[29] = new SceneDummyGame("Stage 2 Cleared!\0");
	sc[30] = new SceneDummyGame("Stage 3 Cleared!\0");
	sc[31] = new SceneDummyGame("Stage 3 Cleared!\0");
	sc[32] = new SceneDummyGame("Stage 3 Cleared!\0");

	sc[33] = new SceneDummyGame("", "res/bg/G_end+credit.png");
}

Scene * getSceneById(Scene ** sc, int n){
	if(n >= FIRST_MAINGAME_ID && n < FIRST_PROLOG_ID){
		return SelectedGame;
	}else{
		return sc[n];
	}
}

int select_next_scene(int source_scene, GameResult * result){
	if(source_scene == 0){
		// オープニングの場合：終わったらメインメニューへ
		if(result) *result = GameResult();
		return 1;
	}else if(source_scene == 1){
		// チュートリアルの場合：当該番号のゲームへ移動
		result->stage_level(0);
		result->stage_color(1);
		result->r_score(0);
		result->g_score(0);
		result->b_score(0);
		switch(result->stage_id()){
			case 1:
				SelectedGame = new SceneStepGame();
				return 3;
			case 2:
				SelectedGame = new Darkroom();
				return 4;
			case 3:
				SelectedGame = new GameRoad();
				((GameRoad *)SelectedGame)->s_col = 0;	// 初期色設定
				((GameRoad *)SelectedGame)->stage = 0;	// チュートリアル
				((GameRoad *)SelectedGame)->init(NULL);
				return 5;
		}
	}else if(source_scene == 2){
		// メインメニューの場合
		if(result->stage_id() >= 1){
			// 正の数の場合、当該番号のゲームのプロローグへ移動
			result->stage_level((result->stage_id() + 2) / 3);
			result->stage_color((result->stage_id() + 2) % 3);
			result->r_score(PlayerStatus.r_score());
			result->g_score(PlayerStatus.g_score());
			result->b_score(PlayerStatus.b_score());
			return result->stage_id() + FIRST_PROLOG_ID - 1;
		}else if(result->stage_id() <= -1){
			// 負の数の場合、当該番号のエンディングへ移動
			return -(result->stage_id()) + FIRST_ENDING_ID - 1;
		}else{
			// ゼロの場合、エンディングへ移動
			return FIRST_ENDING_ID;
		}
	}else if(source_scene >= FIRST_TUTGAME_ID && source_scene < FIRST_MAINGAME_ID){
		// チュートリアルのゲームの場合
		// ゲームが終了した場合、メインメニューへ
		return 2;
	}else if(source_scene >= FIRST_MAINGAME_ID && source_scene < FIRST_PROLOG_ID){
		// 本番のゲームの場合
		// ゲームが終了した場合、成功したら当該ゲームのエピローグへ、失敗したらエンディングへ
		if(result->end_status()){
			PlayerStatus.r_score(PlayerStatus.r_score() + result->r_score());
			PlayerStatus.g_score(PlayerStatus.g_score() + result->g_score());
			PlayerStatus.b_score(PlayerStatus.b_score() + result->b_score());
			return source_scene - FIRST_MAINGAME_ID + FIRST_EPILOG_ID;
		}else{
			return FIRST_ENDING_ID;
		}
	}else if(source_scene >= FIRST_PROLOG_ID && source_scene < FIRST_EPILOG_ID){
		// ゲームのプロローグの場合
		// 当該ゲームのゲーム本番へ
		return source_scene - FIRST_PROLOG_ID + FIRST_MAINGAME_ID;
	}else if(source_scene >= FIRST_EPILOG_ID && source_scene < FIRST_ENDING_ID){
		// ゲームのエピローグの場合
		// メインメニューへ
		return 2;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));

	ChangeWindowMode(TRUE);
	SetGraphMode(DISPLAY_WIDTH, DISPLAY_HEIGHT, 32);
	SetMainWindowText("Ichi no Michi");

	if(DxLib_Init() == -1){
		MessageBox(0, "Failed to launch the program: DxLib is not correctly initialized", "Error", 0);
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	Scene * sc[SCENES];
	define_scenes(sc);
	int current_scene = 0, prev_scene;
	GameResult * update_result;
	GameResult chosen_stage_info;
	
	sc[0]->init(NULL);
	while(ProcessMessage() == 0){
		ClearDrawScreen();
		
		update_result = getSceneById(sc, current_scene)->update();
		if(update_result != NULL){
			while(GetInputChar(TRUE)){ }
			
			prev_scene = current_scene;
			current_scene = select_next_scene(current_scene, update_result);
			
			update_result->stage_id(prev_scene);
			getSceneById(sc, current_scene)->init(update_result);
		}
		getSceneById(sc, current_scene)->draw();
		ScreenFlip();
	}
	
	for(int i = 0; i < SCENES; ++i){
		if(i < FIRST_MAINGAME_ID || i >= FIRST_PROLOG_ID){
			delete sc[i];
		}
	}
	
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}
