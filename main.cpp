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
// �Q�[���i�_�~�[�j
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
// �Q�[���S��
// ============================================================

// �V�[���̈ꗗ��ێ�����
class SceneSet{
public:
	static const int STAGES = 4;

private:
	Scene * menus[3];
	Scene * game;
	Scene * tutorial;
	Scene * prologs[STAGES];
	Scene * epilogs[STAGES];
	Scene * ending;

public:
	SceneSet(){
		menus[0] = new SceneDummyGame("", "res/bg/__opening.png");
		menus[1] = new SceneSelectStage<StageDefTutorial>();
		menus[2] = new SceneSelectStage<StageDefMap>();
		
		prologs[0] = new SceneDummyGame("A shade seen in a forest\0Her name is Ichi\0She is shaking her head\0as she lost her way\0Now she is walking around\0to get out here\0");
		prologs[1] = new SceneDummyGame("Somebody over there\0Ichi shouted with full of her voice\0for asking her way\0But the voice not reached the person\0and she went away and away\0Ichi begin running for him\0");
		prologs[2] = new SceneDummyGame("Ichi found something on the ground\0a cute red ribbonShe wonder why it is here\0as nobody is here\0She thinks it is probably owned\0by the person over there\0Ichi continues running for the person\0");
		prologs[3] = new SceneDummyGame("Stage4\0");
		
		epilogs[0] = new SceneDummyGame("Stage 1 Cleared!\0");
		epilogs[1] = new SceneDummyGame("Stage 2 Cleared!\0");
		epilogs[2] = new SceneDummyGame("Stage 3 Cleared!\0");
		epilogs[3] = new SceneDummyGame("Stage 4 Cleared!\0");

		ending = new SceneDummyGame("", "res/bg/G_end+credit.png");
	}

	~SceneSet(){
		delete menus[0]; delete menus[1]; delete menus[2];
		delete prologs[0]; delete prologs[1]; delete prologs[2]; delete prologs[3];
		delete epilogs[0]; delete epilogs[1]; delete epilogs[2]; delete epilogs[3];
		delete ending;
	}

	// �Q�[�����ʂ�X�e�[�W�Z���N�g���ʂ𔽉f����B
	// �Ԃ�l�Ŏ���Scene��Ԃ��A������current_stage���ŐV�̂��̂ɍX�V����B
	Scene * select_next_scene(GameResult * current_stage, GameResult * update_result){
		switch(current_stage->map_id()){
			case 0: // ������������Ă��Ȃ��ꍇ: �I�[�v�j���O��ʂ�
				current_stage->map_id(1);
				return menus[0];
			case 1: // ���܃I�[�v�j���O�ł���ꍇ: �`���[�g���A���I����ʂ�
				current_stage->map_id(2);
				current_stage->stage_situation(0);
				update_result->end_status(0);
				return menus[1];
			case 2: // ���܃`���[�g���A���I����ʂł���ꍇ: �`���[�g���A����
				switch(current_stage->stage_situation()){
					case 0: // �`���[�g���A���I����ʂɓ��������ゾ�����ꍇ
						current_stage->stage_situation(2); // �Q�[�����i�`���[�g���A�����j�ł��邱�Ƃ��w��
						current_stage->stage_level(0); // �`���[�g���A���ł��邱�Ƃ��w��
						
						// �����ł̑I�����ʂɂ���ăQ�[���̎�ނ�����
						switch(update_result->stage_id()){
							case 1:
								game = new SceneStepGame();
								tutorial = new SceneStepGameTutorial();
								break;
							case 2:
								game = new Darkroom();
								tutorial = new SceneDummyGame("Game \"Dark Room\" Tutorial\0");
								break;
							case 3:
								game = new GameRoad();
								tutorial = new SceneDummyGame("Game \"Road\" Tutorial\0"); // new GameRoad();
								break;
							default:
								game = NULL;
								tutorial = NULL;
								break;
						}
						return tutorial;
					case 2: // �`���[�g���A���I����̏ꍇ
						current_stage->map_id(3);
						current_stage->stage_situation(0);
						update_result->end_status(0);
						return menus[2];
				}
				break;
			case 3: // ���܃��C���Q�[���I����ʂł���ꍇ:
				switch(current_stage->stage_situation()){
					case 0: // �Q�[�����I�΂ꂽ���ゾ�����ꍇ: �I�[�v�j���O��
						current_stage->stage_situation(1); // �I�[�v�j���O�ł��邱�Ƃ��w��
						current_stage->stage_level(update_result->stage_level());
						return prologs[update_result->stage_level() - 1];
					case 1: // �I�[�v�j���O���I������ꍇ: ���C���Q�[����
						current_stage->stage_situation(2); // �Q�[�����ł��邱�Ƃ��w��
						current_stage->stage_level(update_result->stage_level());
						return game;
					case 2: // ���C���Q�[���I����̏ꍇ: �G���f�B���O��
						current_stage->stage_situation(3);
						return epilogs[current_stage->stage_level() - 1];
					case 3: // �G���f�B���O�I����̏ꍇ: �Q�[���I�[�o�[ or ���̃��x����
						if(update_result->end_status()){
							current_stage->stage_situation(0);
							current_stage->stage_level(update_result->stage_level() + 1);
							return menus[2];
						}else{
							return ending;
						}
					case 4: // �S�X�e�[�W�N���A�̏ꍇ: �S�̂̃G���f�B���O��
						current_stage->map_id(4);
						return ending;
				}
				break;
			case 4: // �G���f�B���O
				return ending;
		}
		return NULL;
	}
};

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
	
	GameResult player_status;
	GameResult * update_result;
	SceneSet * scene_set = new SceneSet();
	Scene * current_scene_tmp;
	Scene * current_scene = scene_set->select_next_scene(&player_status, NULL);
	current_scene->init(NULL);
	
	while(ProcessMessage() == 0){
		ClearDrawScreen();
		
		update_result = current_scene->update();
		if(update_result != NULL){
			while(GetInputChar(TRUE)){ }

			current_scene_tmp = scene_set->select_next_scene(&player_status, update_result);
			if(current_scene_tmp != NULL){
				current_scene = current_scene_tmp;
				current_scene->init(update_result);
			}
		}
		current_scene->draw();
		ScreenFlip();
	}
	
	delete scene_set;
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}
