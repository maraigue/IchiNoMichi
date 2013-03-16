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
// ステージ選択画面
// ============================================================

template<class StageDef> class SceneSelectStage : public Scene{
private:
	GameResult result;

	static const int STAGE_WIDTH = 94;
	static const int STAGE_HEIGHT = 94;
	static const int SHADE_HEIGHT = 11;
	
	int current_stage_id; // 選択中のステージ（IDは0起点）
	int previous_stage_id; // 最後にクリアしたステージ（まだ何もクリアしてない場合は-1）
	int cleared_stage_ids; // クリア済みのステージ一覧（ビットマスク。例えばステージ0と3をクリア済みならBIT(0)|BIT(3)）
	int game_cleared; // エンディング前のステージをクリアしたならそのステージのID、そうでなければ0
	int stage_map;
	int stage_template;
	int icon_map[11];
	int line_right[2], line_rightup[2], line_rightdown[2]; // 0：選択されてないライン 1：選択されたライン
	
	int color_bg, color_link, color_stage_not_cleared, color_stage_cleared;
	Cycle cycle_cursor;

	const int * stage(int stage_id){
		return StageDef::STAGE[stage_id];
	}

	void draw_stage(int stage_id, int image_id = 0){
		// ---------- ステージを1つ描画する ----------
		if(!STAGE_VISIBLE(stage(stage_id))){
			return;
		}

		if(image_id == 0) image_id = icon_map[stage_id + StageDef::ICON_ID_OFFSET];
		DrawGraph(XPOS(stage(stage_id)) - STAGE_WIDTH / 2, YPOS(stage(stage_id)) - STAGE_HEIGHT / 2, image_id, TRUE);
	}

	void draw_link(int stage_id1, int stage_id2, int chosen){
		// ---------- リンクを1つ描画する ----------
		// chosenは選択中のリンクなら1、そうでなければ0（-1とかは不可）
		
		if(!STAGE_VISIBLE(stage(stage_id1)) || !STAGE_VISIBLE(stage(stage_id2))) return;

		int src_id, dst_id;
		
		if(XPOS(stage(stage_id1)) < XPOS(stage(stage_id2))){
			src_id = stage_id1;
			dst_id = stage_id2;
		}else{
			src_id = stage_id2;
			dst_id = stage_id1;
		}

		// 現時点では、どのマップを使っても1種類の線の組み合わせで十分、という前提で
		// コードを書いている。
		// もっと様々な形態の線を使うのであれば、どの線を使うかをStageDef側で
		// 定義できるようにすべき。
		if(YPOS(stage(src_id)) < YPOS(stage(dst_id))){
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(src_id)), line_rightdown[chosen], TRUE);
		}else if (YPOS(stage(src_id)) > YPOS(stage(dst_id))){
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(dst_id)), line_rightup[chosen], TRUE);
		}else{
			DrawGraph(XPOS(stage(src_id)), YPOS(stage(src_id)), line_right[chosen], TRUE);
		}
	}

	void create_stage_map_template(void){
		// ---------- ステージのテンプレートを作る ----------
		SetDrawScreen(stage_template);

		DrawGraph(0, 0, stage_map, FALSE);
		
		// リンクを描く
		for(int i = 0; i < StageDef::STAGE_NUMBER; ++i){
			for(int j = i + 1; j < StageDef::STAGE_NUMBER; ++j){
				// iからjへ向かう線がある場合、描画する
				if(NEXT_STAGES(stage(i)) & BIT(j)){
					draw_link(i, j, 0);
				}
			}
		}
		
		// ステージを描く
		for(int n = 0; n < StageDef::STAGE_NUMBER; ++n){
			draw_stage(n);
		}

		SetDrawScreen(DX_SCREEN_FRONT);
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
		line_right[0] = LoadGraph("res/img/path_―.png");
		line_right[1] = LoadGraph("res/img/path2_―.png");
		line_rightup[0] = LoadGraph("res/img/path_／.png");
		line_rightup[1] = LoadGraph("res/img/path2_／.png");
		line_rightdown[0] = LoadGraph("res/img/path_＼.png");
		line_rightdown[1] = LoadGraph("res/img/path2_＼.png");
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
					result.stage_level(STAGE_LEVEL(stage(current_stage_id)));
					result.stage_color(STAGE_COLOR(stage(current_stage_id)));
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
		DrawGraph(0, 0, stage_template, FALSE);

		int cx0, cy0;

		// 現在カーソルが当たっているステージについての座標情報
		// cx0/cy0が中心座標、他はステージの四角形の座標
		cx0 = XPOS(stage(current_stage_id));
		cy0 = YPOS(stage(current_stage_id));
		int cx1 = cx0 - STAGE_WIDTH / 2;
		int cy1 = cy0 - STAGE_WIDTH / 2;
		int cx2 = cx0 + STAGE_WIDTH / 2;
		int cy2 = cy0 + STAGE_WIDTH / 2;

		// 前にいたステージから選択中のステージへの線を強調する
		draw_link(previous_stage_id, current_stage_id, 1);
		draw_stage(previous_stage_id);
		draw_stage(current_stage_id);
		
		// 選択中のステージを強調する
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(cos(static_cast<float>(GetNowCount() % 1500) * (M_PI * 2) / 1500.0) * 255.0));
		draw_stage(current_stage_id, icon_map[0]);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
};

// ステージ配置の定義。
// 第1要素: x座標（中心点で指定する）
// 第2要素: y座標（同上）
// 第3要素: ステージのレベル（0:チュートリアル 1以上:ステージ番号）
// 第4要素: ステージ色（0:赤 1:緑 2:青 -1:色なし）
// 第5要素: 次に進めるステージの一覧（ビットマスクで表現）
// ※第1要素・第2要素ともに0の場合、マップ上に表示されない
//   （マップを開いたときに最初に選べるステージを選ぶ）
// ※第5要素が0（どのステージも指定されていない）の場合、エンディングに繋がる
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
