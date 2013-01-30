
/*
	by rocca
	   -> 2013/01/25
*/

#pragma once
#include <DxLib.h>
#include "scene.hpp"
#define SC_WIDTH 800
#define SC_HEIGHT 600

// 落下ブロックデータ
typedef struct {
	int k;	// どのy位置
	int x;
	int type;	// 0:左連結 1:上連結　2:下連結
	int ok;	// 落下ブロック ->1:移動中 -1:落下中 0:なし 失敗ブロック->α値
	int col;	// 色 RGB(0~2)
}BLOCK;

// 難易度
typedef struct {
	int road;	// そこに至るブロック数
	int speed;	// 速度
}LO_LEVEL;

// 画像データ
typedef struct {
	int num, pat;	// いちの場合ハンドル・パターン
	int x,y;
	int w,h;
	
}SPRITE;
// 進行
enum {
	LO_INI,
	LO_GAME,
	LO_CLEAR,
	LO_LOSE,

	LO_END,

	LO_GAMEOVER,
};

class GameRoad : public Scene {
public:
	int cond;	// 0:クラス初回 1:通常 2:end動作 3:完全end
	int time;		// トータル時間
	int miss;		// ミス
	int s_col;		// カラー
	int stage;		// ステージ
	int get_rgb[3]; 
	int alpha;
	int move_time; // ブロック移動時間
	int next_time;	// 次ブロック透過時間
	int all_road;	// 総セットブロック数

	int now_level;	// 難易度
	int lose_cnt;	// 敗北ミス数

	int lo_key;		// 押下中キー
	int myroad;		// 最終道（列）

	int once_btn;	//連打防止フラグ 0:なし 1:うえ　2:した 4:Enter
	BLOCK block;		// 表示落下ブロック
	BLOCK del_block;	// 失敗落下ブロック

	int ch_time;	// いちパターンチェンジ時間
	int ch_movetime;// いち移動時間
	int ch_mode;	// 0: ゲーム中　1:ゴールへ
	int ch_walk;	// 移動情報
	POINT reg;		// いち足元座標

	char Key[ 256 ] ;
	GameResult ro_g_res;
public:
	GameRoad(void);
	void init(GameResult * result);
	void draw(void);
	GameResult * update(void);

	void newBlock(void);
	void setNewBlockTime(void);
	void roadSetCheck(void);
	int clearCheck(void);
	int loseCheck(void);
	void Failureroad(void);

	void chMoveSet(void);
	void chMove(void);

	void chReg(SPRITE *sp);
	void LoadDel(void);
	void RoadEnd(void);
	void RoadGameOverEnd(void);
};

