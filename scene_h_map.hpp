#ifndef _SCENE_H_MAIN_HPP_
#define _SCENE_H_MAIN_HPP_

#define BIT(n) (1 << (n))
#define XPOS(stg) ((stg)[0])
#define YPOS(stg) ((stg)[1])
#define NEXT_STAGES(stg) ((stg)[2])
#define IS_NEXT_STAGE(stg, next_stage_id) ((stg)[2] & BIT(next_stage_id))

// ============================================================
// ステージ選択画面
// ============================================================

template<class StageDef> class SceneSelectStage : public Scene{
private:
	GameResult result;

	static const int STAGE_WIDTH = 94;
	static const int STAGE_HEIGHT = 94;

	int current_stage_id; // 選択中のステージ（IDは0起点）
	int previous_stage_id; // 最後にクリアしたステージ（まだ何もクリアしてない場合は-1）
	int cleared_stage_ids; // クリア済みのステージ一覧（ビットマスク。例えばステージ0と3をクリア済みならBIT(0)|BIT(3)）
	int game_cleared; // エンディング前のステージをクリアしたならそのステージのID、そうでなければ0
	int stage_map;
	int icon_map[10];
	
	int color_bg, color_link, color_stage_not_cleared, color_stage_cleared;
	Cycle cycle_cursor;

	const int * stage(int stage_id){
		return StageDef::STAGE[stage_id];
	}

	void draw_stage(int stage_id){
		// ---------- ステージを1つ描画する ----------
		if(XPOS(stage(stage_id)) == 0 && YPOS(stage(stage_id)) == 0) return;
		
		DrawGraph(XPOS(stage(stage_id)) - STAGE_WIDTH / 2, YPOS(stage(stage_id)) - STAGE_HEIGHT / 2, icon_map[stage_id], TRUE);
	}

	void create_stage_map_template(void){
		/*
		// ---------- ステージのテンプレートを作る ----------
		SetDrawScreen(stage_map);

		// 白で塗りつぶす
		DrawBox(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, color_bg, TRUE);
		
		// リンクを描く
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			if(XPOS(stage(n)) == 0 && YPOS(stage(n)) == 0) continue;
			for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
				if(IS_NEXT_STAGE(stage(n), i)){
					DrawLine(XPOS(stage(n)), YPOS(stage(n)), XPOS(stage(i)), YPOS(stage(i)), color_link, 3);
				}
			}
		}

		// ステージを描く
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			draw_stage(n);
		}
		SetDrawScreen(DX_SCREEN_FRONT);
		*/
	}
	
	// 選択中のステージを切り替える
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
	}
	
	void init(GameResult * previous_result){
		if(StageDef::need_initialized(previous_result->stage_id())){
			// チュートリアルの後だった場合（初めての呼び出しのため、要初期化）
			stage_map = LoadGraph("res/bg/map_bg.png");
			current_stage_id = 0;
			previous_stage_id = 0;
			cleared_stage_ids = 0;
			game_cleared = 0;
			change_current_stage_id(1);
			create_stage_map_template();
		}else{
			// ステージがクリアできていた場合
			cleared_stage_ids |= BIT(current_stage_id);
			if(NEXT_STAGES(stage(current_stage_id)) == 0){
				// 最終ステージをクリアした場合
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
					// 前のステージにカーソルを進める
					change_current_stage_id(-1);
					break;
				case CTRL_CODE_DOWN: case CTRL_CODE_RIGHT:
					// 次のステージにカーソルを進める
					change_current_stage_id(1);
					break;
				case CTRL_CODE_CR:
					// ゲームステージへ移動
					result.stage_id(current_stage_id);
					return &result;
			}
			return NULL;
		}
	}

	void draw(){
		// 色を準備
		int offset = cycle_cursor.get();
		int color1 = GetColor(127 + 127 * offset, 0, 0);
		int color2 = GetColor(254 - 127 * offset, 0, 0);

		// テンプレートを描く
		DrawGraph(0, 0, stage_map, FALSE);

		// アイコンを描く
		int cx0, cy0;

		for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
			draw_stage(i);
		}

		// 現在カーソルが当たっているステージについての座標情報
		// cx0/cy0が中心座標、他はステージの四角形の座標
		cx0 = XPOS(stage(current_stage_id));
		cy0 = YPOS(stage(current_stage_id));
		int cx1 = cx0 - STAGE_WIDTH / 2;
		int cy1 = cy0 - STAGE_WIDTH / 2;
		int cx2 = cx0 + STAGE_WIDTH / 2;
		int cy2 = cy0 + STAGE_WIDTH / 2;

		// 前にいたステージから選択中のステージへの線を強調する
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

		// 選択中のステージを強調する
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

// ステージ配置の定義。
// 第1要素: x座標（中心点で指定する）
// 第2要素: y座標（同上）
// 第3要素: 次に進めるステージの一覧（ビットマスクで表現）
// 第3要素が0（どのステージも指定されていない）の場合、エンディングに繋がる
class StageDefMap {
public:
	static const int STAGE_NUMBER = 10;
	static const int STAGE[STAGE_NUMBER][3];
	static const int need_initialized(int game_id){
		return(IS_AFTER_TUTORIAL_GAME(game_id) || game_id == 0);
	}
};

const int StageDefMap::STAGE[STAGE_NUMBER][3] = {
	{109, 303, BIT(1)|BIT(2)|BIT(3)},
	{286, 180, BIT(4)|BIT(5)|BIT(6)},
	{286, 303, BIT(4)|BIT(5)|BIT(6)},
	{286, 431, BIT(4)|BIT(5)|BIT(6)},
	{463, 180, BIT(7)|BIT(8)|BIT(9)},
	{463, 303, BIT(7)|BIT(8)|BIT(9)},
	{463, 431, BIT(7)|BIT(8)|BIT(9)},
	{640, 180, 0},
	{640, 303, 0},
	{640, 431, 0},
};


class StageDefTutorial {
public:
	static const int STAGE_NUMBER = 4;
	static const int STAGE[STAGE_NUMBER][3];
	static const int need_initialized(int game_id){
		return(IS_AFTER_OPENING(game_id));
	}
};

const int StageDefTutorial::STAGE[STAGE_NUMBER][3] = {
	{0, 0, BIT(1)|BIT(2)|BIT(3)},
	{200, 250, 0},
	{400, 250, 0},
	{600, 250, 0},
};

#endif // _SCENE_H_MAIN_HPP_
