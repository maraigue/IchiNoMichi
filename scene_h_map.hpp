#ifndef _SCENE_H_MAIN_HPP_
#define _SCENE_H_MAIN_HPP_

#define BIT(n) (1 << (n))
#define XPOS(stg) ((stg)[0])
#define YPOS(stg) ((stg)[1])
#define STAGE_VISIBLE(stg) (!((stg)[0] == 0 && (stg)[1] == 0))
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
	static const int SHADE_HEIGHT = 11;
	
	int current_stage_id; // �I�𒆂̃X�e�[�W�iID��0�N�_�j
	int previous_stage_id; // �Ō�ɃN���A�����X�e�[�W�i�܂������N���A���ĂȂ��ꍇ��-1�j
	int cleared_stage_ids; // �N���A�ς݂̃X�e�[�W�ꗗ�i�r�b�g�}�X�N�B�Ⴆ�΃X�e�[�W0��3���N���A�ς݂Ȃ�BIT(0)|BIT(3)�j
	int game_cleared; // �G���f�B���O�O�̃X�e�[�W���N���A�����Ȃ炻�̃X�e�[�W��ID�A�����łȂ����0
	int stage_map;
	int stage_template;
	int icon_map[11];
	int line_right[2], line_rightup[2], line_rightdown[2]; // 0�F�I������ĂȂ����C�� 1�F�I�����ꂽ���C��
	
	int color_bg, color_link, color_stage_not_cleared, color_stage_cleared;
	Cycle cycle_cursor;

	const int * stage(int stage_id){
		return StageDef::STAGE[stage_id];
	}

	void draw_stage(int stage_id, int image_id = 0){
		// ---------- �X�e�[�W��1�`�悷�� ----------
		if(!STAGE_VISIBLE(stage(stage_id))){
			return;
		}

		if(image_id == 0) image_id = icon_map[stage_id + StageDef::ICON_ID_OFFSET];
		DrawGraph(XPOS(stage(stage_id)) - STAGE_WIDTH / 2, YPOS(stage(stage_id)) - STAGE_HEIGHT / 2, image_id, TRUE);
	}

	void draw_link(int stage_id1, int stage_id2, int chosen){
		// ---------- �����N��1�`�悷�� ----------
		// chosen�͑I�𒆂̃����N�Ȃ�1�A�����łȂ����0�i-1�Ƃ��͕s�j
		
		if(!STAGE_VISIBLE(stage(stage_id1)) || !STAGE_VISIBLE(stage(stage_id2))) return;

		int src_id, dst_id;
		
		if(XPOS(stage(stage_id1)) < XPOS(stage(stage_id2))){
			src_id = stage_id1;
			dst_id = stage_id2;
		}else{
			src_id = stage_id2;
			dst_id = stage_id1;
		}

		// �����_�ł́A�ǂ̃}�b�v���g���Ă�1��ނ̐��̑g�ݍ��킹�ŏ\���A�Ƃ����O���
		// �R�[�h�������Ă���B
		// �����Ɨl�X�Ȍ`�Ԃ̐����g���̂ł���΁A�ǂ̐����g������StageDef����
		// ��`�ł���悤�ɂ��ׂ��B
		if(YPOS(stage(src_id)) < YPOS(stage(dst_id))){
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(src_id)), line_rightdown[chosen], TRUE);
		}else if (YPOS(stage(src_id)) > YPOS(stage(dst_id))){
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(dst_id)), line_rightup[chosen], TRUE);
		}else{
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(src_id)), line_right[chosen], TRUE);
		}
	}

	void create_stage_map_template(void){
		// ---------- �X�e�[�W�̃e���v���[�g����� ----------
		SetDrawScreen(stage_template);

		DrawGraph(0, 0, stage_map, FALSE);
		
		// �����N��`��
		for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
			for(int j = i + 1; j < StageDef::STAGE_NUMBER; ++j){
				// i����j�֌�������������ꍇ�A�`�悷��
				if(NEXT_STAGES(stage(i)) & BIT(j)){
					draw_link(i, j, 0);
				}
			}
		}
		
		// �X�e�[�W��`��
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			draw_stage(n);
		}

		SetDrawScreen(DX_SCREEN_FRONT);
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
		line_right[0] = LoadGraph("res/img/path_�\.png");
		line_right[1] = LoadGraph("res/img/path2_�\.png");
		line_rightup[0] = LoadGraph("res/img/path_�^.png");
		line_rightup[1] = LoadGraph("res/img/path2_�^.png");
		line_rightdown[0] = LoadGraph("res/img/path_�_.png");
		line_rightdown[1] = LoadGraph("res/img/path2_�_.png");
		stage_map = LoadGraph("res/bg/map_bg.png");
		stage_template = MakeScreen(DISPLAY_WIDTH, DISPLAY_HEIGHT);

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
		DrawGraph(0, 0, stage_template, FALSE);

		int cx0, cy0;

		// ���݃J�[�\�����������Ă���X�e�[�W�ɂ��Ă̍��W���
		// cx0/cy0�����S���W�A���̓X�e�[�W�̎l�p�`�̍��W
		cx0 = XPOS(stage(current_stage_id));
		cy0 = YPOS(stage(current_stage_id));
		int cx1 = cx0 - STAGE_WIDTH / 2;
		int cy1 = cy0 - STAGE_WIDTH / 2;
		int cx2 = cx0 + STAGE_WIDTH / 2;
		int cy2 = cy0 + STAGE_WIDTH / 2;

		// �O�ɂ����X�e�[�W����I�𒆂̃X�e�[�W�ւ̐�����������
		draw_link(previous_stage_id, current_stage_id, 1);
		draw_stage(previous_stage_id);
		draw_stage(current_stage_id);
		
		// �I�𒆂̃X�e�[�W����������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(cos(static_cast<float>(GetNowCount() % 1500) * (M_PI * 2) / 1500.0) * 255.0));
		draw_stage(current_stage_id, icon_map[0]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
	static const int ICON_ID_OFFSET = -1;
};

const int StageDefMap::STAGE[STAGE_NUMBER][5] = {
	{0, 0, 0, -1, BIT(1)},
	{109, 303, 1,-1, BIT(2)|BIT(3)|BIT(4)},
	{286, 180, 2, 0, BIT(5)|BIT(6)},
	{286, 303, 2, 1, BIT(5)|BIT(6)|BIT(7)},
	{286, 431, 2, 2, BIT(6)|BIT(7)},
	{463, 180, 3, 0, BIT(8)|BIT(9)},
	{463, 303, 3, 1, BIT(8)|BIT(9)|BIT(10)},
	{463, 431, 3, 2, BIT(9)|BIT(10)},
	{640, 180, 4, 0, 0},
	{640, 303, 4, 1, 0},
	{640, 431, 4, 2, 0},
};


class StageDefTutorial {
public:
	static const int STAGE_NUMBER = 4;
	static const int STAGE[STAGE_NUMBER][5];
	static const int ICON_ID_OFFSET = 0;
};

const int StageDefTutorial::STAGE[STAGE_NUMBER][5] = {
	{0, 0, 0, -1, BIT(1)|BIT(2)|BIT(3)},
	{200, 250, 0, -1, 0},
	{400, 250, 0, -1, 0},
	{600, 250, 0, -1, 0},
};

#endif // _SCENE_H_MAIN_HPP_
