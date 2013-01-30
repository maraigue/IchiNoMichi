#ifndef _GAME_H_PANELS_HPP_
#define _GAME_H_PANELS_HPP_

// ============================================================
// �Q�[���i�����؂�ւ��ăL�����𐶂��c�点��j
// ============================================================

class ScenePanelGame : public Scene{
	GameResult result;

	static const int FIELD_LOGICAL_SIZE = 100000;
	static const int FIELD_DRAW_LEFT = 200;
	static const int FIELD_DRAW_TOP = 100;
	static const int FIELD_DRAW_WIDTH = 400;
	static const int FIELD_DRAW_HEIGHT = 400;
	static const int DIRECTION_CHANGE_RATIO = 1200;
	int last_time;
	int next_direction_change_time;
	int remained_time; // ���~���b������������΃X�e�[�W�N���A�ɂȂ邩
	double direction;
	int speed; // �b��
	int turned_on_panels[4]; // �I���ɂȂ��Ă�p�l���B
	// 0 1
	// 2 3

	int charaX, charaY;
	int panel_color[2];
	int is_running; // �����Ă��邩�iFALSE�Ȃ玸�s�����j
	int falling_time; // �L�����������Ă����A�j���[�V�����̏�������
	static const int FALLING = 3000;

public:
	ScenePanelGame(void){
		panel_color[0] = GetColor(0,0,0);
		panel_color[1] = GetColor(128,128,0);
	}

	void init(GameResult * result){
		last_time = 0;
		next_direction_change_time = 0;
		direction = 0;
		is_running = 1;
		falling_time = FALLING;
		speed = FIELD_LOGICAL_SIZE / 2;
		charaX = FIELD_LOGICAL_SIZE / 2;
		charaY = FIELD_LOGICAL_SIZE / 10;
		remained_time = 20000;
		turned_on_panels[0] = 1;
		turned_on_panels[1] = 1;
		turned_on_panels[2] = 0;
		turned_on_panels[3] = 0;
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

		if(is_running){
			// ---------- �ړ��� ----------

			// �L�[���󂯎��
			switch(GetInputChar(TRUE)){
				case CTRL_CODE_UP:
					turned_on_panels[0] = 1;
					turned_on_panels[1] = 1;
					turned_on_panels[2] = 0;
					turned_on_panels[3] = 0;
					break;
				case CTRL_CODE_LEFT:
					turned_on_panels[0] = 1;
					turned_on_panels[1] = 0;
					turned_on_panels[2] = 1;
					turned_on_panels[3] = 0;
					break;
				case CTRL_CODE_DOWN:
					turned_on_panels[0] = 0;
					turned_on_panels[1] = 0;
					turned_on_panels[2] = 1;
					turned_on_panels[3] = 1;
					break;
				case CTRL_CODE_RIGHT:
					turned_on_panels[0] = 0;
					turned_on_panels[1] = 1;
					turned_on_panels[2] = 0;
					turned_on_panels[3] = 1;
					break;
			}
			
			// �L�����̈ړ�����v�Z
			if(current_time > next_direction_change_time){
				direction += M_PI * (rand() - RAND_MAX / 2) / RAND_MAX;
				next_direction_change_time = current_time + 1000;
			}
			direction += M_PI * rand() / RAND_MAX * diff_time / DIRECTION_CHANGE_RATIO;
			
			int new_charaX = charaX + (int)((double)speed * diff_time * cos(direction) / 1000);
			int new_charaY = charaY + (int)((double)speed * diff_time * sin(direction) / 1000);

			if(new_charaX < 0 || new_charaX > FIELD_LOGICAL_SIZE ||
				new_charaY < 0 || new_charaY > FIELD_LOGICAL_SIZE ||
				(new_charaX > FIELD_LOGICAL_SIZE / 3 && new_charaX < FIELD_LOGICAL_SIZE * 2 / 3 &&
				new_charaY > FIELD_LOGICAL_SIZE / 3 && new_charaY < FIELD_LOGICAL_SIZE * 2 / 3)){
					next_direction_change_time = 0;
			}else{
				charaX = new_charaX;
				charaY = new_charaY;
			}

			// ��������
			if(
				(turned_on_panels[0] == 0 && turned_on_panels[1] == 0 && charaY < FIELD_LOGICAL_SIZE / 2) ||
				(turned_on_panels[2] == 0 && turned_on_panels[3] == 0 && charaY > FIELD_LOGICAL_SIZE / 2) ||
				(turned_on_panels[0] == 0 && turned_on_panels[2] == 0 && charaX < FIELD_LOGICAL_SIZE / 2) ||
				(turned_on_panels[1] == 0 && turned_on_panels[3] == 0 && charaX > FIELD_LOGICAL_SIZE / 2)){
					is_running = 0;
			}

			// �N���A������
			if(remained_time < diff_time){
				remained_time = 0;
				is_running = 0;
			}else{
				remained_time -= diff_time;
			}
			return NULL;
		}else{
			if(remained_time == 0){
				// �N���A
				if(GetInputChar(TRUE) == CTRL_CODE_CR){
					return &result;
				}
			}else{
				// ������
				if(falling_time < diff_time){
					init(NULL);
				}else{
					falling_time -= diff_time;
				}
			}
			return NULL;
		}
	}

	void draw(void){
		// ---------- �w�i ----------
		DrawBox(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GetColor(0, 0, 0), TRUE);
		// ---------- �p�l�� ----------
		DrawBox(
			FIELD_DRAW_LEFT, FIELD_DRAW_TOP,
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH / 2 - 1, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT / 2 - 1,
			panel_color[turned_on_panels[0]], TRUE);
		DrawBox(
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH / 2 + 1, FIELD_DRAW_TOP,
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT / 2 - 1,
			panel_color[turned_on_panels[1]], TRUE);
		DrawBox(
			FIELD_DRAW_LEFT, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT / 2 + 1,
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH / 2 - 1, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT,
			panel_color[turned_on_panels[2]], TRUE);
		DrawBox(
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH / 2 + 1, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT / 2 + 1,
			FIELD_DRAW_LEFT + FIELD_DRAW_WIDTH, FIELD_DRAW_TOP + FIELD_DRAW_HEIGHT,
			panel_color[turned_on_panels[3]], TRUE);
		
		// ---------- �L���� ----------
		DrawCircle(
			charaX * FIELD_DRAW_WIDTH / FIELD_LOGICAL_SIZE + FIELD_DRAW_LEFT,
			charaY * FIELD_DRAW_HEIGHT / FIELD_LOGICAL_SIZE + FIELD_DRAW_TOP,
			FIELD_DRAW_WIDTH * falling_time / FALLING / 20, GetColor(255, 255, 255), TRUE);

		// ---------- �N���A���Ă����ꍇ ----------
		if(remained_time == 0){
			DrawString(0, 0, "Cleared!", GetColor(255, 255, 255));
		}else{
			DrawFormatString(0, 0, GetColor(255, 255, 255), "%d (msec) to be cleared", remained_time);
		}
	}
};

#endif // _GAME_H_PANELS_HPP_
