#ifndef _SCENE_H_MAIN_HPP_
#define _SCENE_H_MAIN_HPP_

#define BIT(n) (1 << (n))
#define XPOS(stg) ((stg)[0])
#define YPOS(stg) ((stg)[1])
#define STAGE_LEVEL(stg) ((stg)[2])
#define STAGE_COLOR(stg) ((stg)[3])
#define NEXT_STAGES(stg) ((stg)[4])
#define IS_NEXT_STAGE(stg, next_stage_id) ((stg)[4] & BIT(next_stage_id))

// ============================================================
// �X�e�[�W�I�����
// ============================================================

template<class StageDef> class SceneSelectStage : public Scene{
private:
	GameResult result;

	static const int STAGE_WIDTH = 94;
	static const int STAGE_HEIGHT = 94;

	int current_stage_id; // �I�𒆂̃X�e�[�W�iID��0�N�_�j
	int previous_stage_id; // �Ō�ɃN���A�����X�e�[�W�i�܂������N���A���ĂȂ��ꍇ��-1�j
	int cleared_stage_ids; // �N���A�ς݂̃X�e�[�W�ꗗ�i�r�b�g�}�X�N�B�Ⴆ�΃X�e�[�W0��3���N���A�ς݂Ȃ�BIT(0)|BIT(3)�j
	int game_cleared; // �G���f�B���O�O�̃X�e�[�W���N���A�����Ȃ炻�̃X�e�[�W��ID�A�����łȂ����0
	int stage_map;
	int icon_map[10];
	
	int color_bg, color_link, color_stage_not_cleared, color_stage_cleared;
	Cycle cycle_cursor;

	const int * stage(int stage_id){
		return StageDef::STAGE[stage_id];
	}

	void draw_stage(int stage_id){
		// ---------- �X�e�[�W��1�`�悷�� ----------
		if(XPOS(stage(stage_id)) == 0 && YPOS(stage(stage_id)) == 0) return;
		
		DrawGraph(XPOS(stage(stage_id)) - STAGE_WIDTH / 2, YPOS(stage(stage_id)) - STAGE_HEIGHT / 2, icon_map[stage_id], TRUE);
	}

	void create_stage_map_template(void){
		/*
		// ---------- �X�e�[�W�̃e���v���[�g����� ----------
		SetDrawScreen(stage_map);

		// ���œh��Ԃ�
		DrawBox(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, color_bg, TRUE);
		
		// �����N��`��
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			if(XPOS(stage(n)) == 0 && YPOS(stage(n)) == 0) continue;
			for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
				if(IS_NEXT_STAGE(stage(n), i)){
					DrawLine(XPOS(stage(n)), YPOS(stage(n)), XPOS(stage(i)), YPOS(stage(i)), color_link, 3);
				}
			}
		}

		// �X�e�[�W��`��
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			draw_stage(n);
		}
		SetDrawScreen(DX_SCREEN_FRONT);
		*/
	}
	
	// �I�𒆂̃X�e�[�W��؂�ւ���
	void change_current_stage_id(int change){
		if(previous_stage_id == -1 || NEXT_STAGES(stage(previous_stage_id)) == 0) return;
		
		int i = current_stage_id;
		do{
			i += change;
			i %= StageDef::STAGE_NUMBER;
			if(i < 0) i += StageDef::STAGE_NUMBER;
			if(IS_NEXT_STAGE(stage(previous_stage_id), i)) break;
		}while(i != current_stage_id);
		
		current_stage_id = i;
	}

public:
	SceneSelectStage() : cycle_cursor(1000, 2){
		stage_map = MakeScreen(DISPLAY_WIDTH, DISPLAY_HEIGHT);
		color_bg = GetColor(255, 255, 255);
		color_link = GetColor(0, 0, 0);
		color_stage_not_cleared = GetColor(255, 255, 128);
		color_stage_cleared = GetColor(128, 255, 128);

		icon_map[0] = LoadGraph("res/img/1.png");
		icon_map[1] = LoadGraph("res/img/2-R.png");
		icon_map[2] = LoadGraph("res/img/2-G.png");
		icon_map[3] = LoadGraph("res/img/2-B.png");
		icon_map[4] = LoadGraph("res/img/3-R.png");
		icon_map[5] = LoadGraph("res/img/3-G.png");
		icon_map[6] = LoadGraph("res/img/3-B.png");
		icon_map[7] = LoadGraph("res/img/4-R.png");
		icon_map[8] = LoadGraph("res/img/4-G.png");
		icon_map[9] = LoadGraph("res/img/4-B.png");
		
		stage_map = LoadGraph("res/bg/map_bg.png");
		current_stage_id = 0;
		previous_stage_id = 0;
		cleared_stage_ids = 0;
		game_cleared = 0;
		change_current_stage_id(1);
		create_stage_map_template();
	}
	
	void init(GameResult * previous_result){
		if(previous_result->end_status()){
			// �X�e�[�W���N���A�ł��Ă����ꍇ
			cleared_stage_ids |= BIT(current_stage_id);
			if(NEXT_STAGES(stage(current_stage_id)) == 0){
				// �ŏI�X�e�[�W���N���A�����ꍇ
				game_cleared = current_stage_id;
			}
			
			previous_stage_id = current_stage_id;
			change_current_stage_id(1);
			
			create_stage_map_template();
		}
	}
	
	GameResult * update(){
		if(game_cleared){
			WaitTimer(3000);
			result.stage_id(0);
			return &result;
		}else{
			switch(GetInputChar(TRUE)){
				case CTRL_CODE_UP: case CTRL_CODE_LEFT:
					// �O�̃X�e�[�W�ɃJ�[�\����i�߂�
					change_current_stage_id(-1);
					break;
				case CTRL_CODE_DOWN: case CTRL_CODE_RIGHT:
					// ���̃X�e�[�W�ɃJ�[�\����i�߂�
					change_current_stage_id(1);
					break;
				case CTRL_CODE_CR:
					// �Q�[���X�e�[�W�ֈړ�
					result.stage_id(current_stage_id);
					result.stage_level(STAGE_LEVEL(stage(current_stage_id)));
					result.stage_color(STAGE_COLOR(stage(current_stage_id)));
					return &result;
			}
			return NULL;
		}
	}

	void draw(){
		// �F������
		int offset = cycle_cursor.get();
		int color1 = GetColor(127 + 127 * offset, 0, 0);
		int color2 = GetColor(254 - 127 * offset, 0, 0);

		// �e���v���[�g��`��
		DrawGraph(0, 0, stage_map, FALSE);

		// �A�C�R����`��
		int cx0, cy0;

		for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
			draw_stage(i);
		}

		// ���݃J�[�\�����������Ă���X�e�[�W�ɂ��Ă̍��W���
		// cx0/cy0�����S���W�A���̓X�e�[�W�̎l�p�`�̍��W
		cx0 = XPOS(stage(current_stage_id));
		cy0 = YPOS(stage(current_stage_id));
		int cx1 = cx0 - STAGE_WIDTH / 2;
		int cy1 = cy0 - STAGE_WIDTH / 2;
		int cx2 = cx0 + STAGE_WIDTH / 2;
		int cy2 = cy0 + STAGE_WIDTH / 2;

		// �O�ɂ����X�e�[�W����I�𒆂̃X�e�[�W�ւ̐�����������
		int px0, py0;

		if(previous_stage_id >= 0){
			px0 = XPOS(stage(previous_stage_id));
			py0 = YPOS(stage(previous_stage_id));
			
			if(!(px0 == 0 && py0 == 0)){
				DrawLine(px0, py0 - 3, cx0, cy0 - 3, color2);
				DrawLine(px0, py0 - 2, cx0, cy0 - 2, color2);
				DrawLine(px0, py0 - 1, cx0, cy0 - 1, color1);
				DrawLine(px0, py0, cx0, cy0, color1);
				DrawLine(px0, py0 + 1, cx0, cy0 + 1, color1);
				DrawLine(px0, py0 + 2, cx0, cy0 + 2, color2);
				DrawLine(px0, py0 + 3, cx0, cy0 + 3, color2);
			}
			
			
			draw_stage(previous_stage_id);
			draw_stage(current_stage_id);
		}

		// �I�𒆂̃X�e�[�W����������
		/*
		DrawBox(cx1, cy1, cx2, cy2, color1, FALSE);
		DrawBox(cx1 + 1, cy1 + 1, cx2 - 1, cy2 - 1, color1, FALSE);
		DrawBox(cx1 + 2, cy1 + 2, cx2 - 2, cy2 - 2, color1, FALSE);
		DrawBox(cx1 + 3, cy1 + 3, cx2 - 3, cy2 - 3, color2, FALSE);
		DrawBox(cx1 + 4, cy1 + 4, cx2 - 4, cy2 - 4, color2, FALSE);
		DrawBox(cx1 + 5, cy1 + 5, cx2 - 5, cy2 - 5, color2, FALSE);
		*/
		DrawCircle(cx0, cy0, 50, color1, FALSE);
		DrawCircle(cx0, cy0, 51, color1, FALSE);
		DrawCircle(cx0, cy0, 52, color1, FALSE);
		DrawCircle(cx0, cy0, 53, color2, FALSE);
		DrawCircle(cx0, cy0, 54, color2, FALSE);
		DrawCircle(cx0, cy0, 55, color2, FALSE);
	}
};

// �X�e�[�W�z�u�̒�`�B
// ��1�v�f: x���W�i���S�_�Ŏw�肷��j
// ��2�v�f: y���W�i����j
// ��3�v�f: �X�e�[�W�̃��x���i0:�`���[�g���A�� 1�ȏ�:�X�e�[�W�ԍ��j
// ��4�v�f: �X�e�[�W�F�i0:�� 1:�� 2:�� -1:�F�Ȃ��j
// ��5�v�f: ���ɐi�߂�X�e�[�W�̈ꗗ�i�r�b�g�}�X�N�ŕ\���j
// ����1�v�f�E��2�v�f�Ƃ���0�̏ꍇ�A�}�b�v��ɕ\������Ȃ�
//   �i�}�b�v���J�����Ƃ��ɍŏ��ɑI�ׂ�X�e�[�W��I�ԁj
// ����5�v�f��0�i�ǂ̃X�e�[�W���w�肳��Ă��Ȃ��j�̏ꍇ�A�G���f�B���O�Ɍq����
class StageDefMap {
public:
	static const int STAGE_NUMBER = 11;
	static const int STAGE[STAGE_NUMBER][5];
};

const int StageDefMap::STAGE[STAGE_NUMBER][5] = {
	{0, 0, 0, -1, BIT(1)},
	{109, 303, 1,-1, BIT(2)|BIT(3)|BIT(4)},
	{286, 180, 2, 0, BIT(5)|BIT(6)|BIT(7)},
	{286, 303, 2, 1, BIT(5)|BIT(6)|BIT(7)},
	{286, 431, 2, 2, BIT(5)|BIT(6)|BIT(7)},
	{463, 180, 3, 0, BIT(8)|BIT(9)|BIT(10)},
	{463, 303, 3, 1, BIT(8)|BIT(9)|BIT(10)},
	{463, 431, 3, 2, BIT(8)|BIT(9)|BIT(10)},
	{640, 180, 4, 0, 0},
	{640, 303, 4, 1, 0},
	{640, 431, 4, 2, 0},
};


class StageDefTutorial {
public:
	static const int STAGE_NUMBER = 4;
	static const int STAGE[STAGE_NUMBER][5];
};

const int StageDefTutorial::STAGE[STAGE_NUMBER][5] = {
	{0, 0, 0, -1, BIT(1)|BIT(2)|BIT(3)},
	{200, 250, 0, -1, 0},
	{400, 250, 0, -1, 0},
	{600, 250, 0, -1, 0},
};

#endif // _SCENE_H_MAIN_HPP_
