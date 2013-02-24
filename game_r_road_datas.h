/*
	by rocca
	   -> 2013/02/11
*/

#define RO_X_CNT 12
#define RO_Y_CNT 8

// 画像ID
enum{
	RO_SP_BG,
	RO_SP_FB,

	RO_SP_BR_R1,
	RO_SP_BR_R2,
	RO_SP_BR_R3,

	RO_SP_BR_G1,
	RO_SP_BR_G2,
	RO_SP_BR_G3,

	RO_SP_BR_B1,
	RO_SP_BR_B2,
	RO_SP_BR_B3,

	RO_SP_LOSE1,
	RO_SP_LOSE2,
	RO_SP_LOSE3,
	RO_SP_LOSE4,

	RO_SP_OK1,
	RO_SP_OK2,
	RO_SP_OK3,
	RO_SP_OK4,
	RO_SP_OK5,

	RO_SP_NG1,
	RO_SP_NG2,

	RO_SP_T1,	// tutorial
	RO_SP_T2,	// start
	RO_SP_T3,	// wark
	RO_SP_T4,	// goal

	_RO_SP_END, // 一括読み込みSPここまで

	RO_SP_ICHI = _RO_SP_END,	// 分割読込開始,
	_RO_SP_ALL_END,
};
static int cols[] = {RO_SP_BR_R1, RO_SP_BR_G1, RO_SP_BR_B1};

// 効果音ID
// 各色８種ランダムランダム
enum{
	RO_SE_RK1,	// あか
	RO_SE_RK2,	//
	RO_SE_RK3,	//
	RO_SE_RK4,
	RO_SE_RK5,
	RO_SE_RK6,
	RO_SE_RK7,
	RO_SE_RK8,

	RO_SE_GK1,	// みどり
	RO_SE_GK2,	// 
	RO_SE_GK3,	// 
	RO_SE_GK4,
	RO_SE_GK5,
	RO_SE_GK6,
	RO_SE_GK7,
	RO_SE_GK8,

	RO_SE_BK1,	// あお
	RO_SE_BK2,	// 
	RO_SE_BK3,	// 
	RO_SE_BK4,
	RO_SE_BK5,
	RO_SE_BK6,
	RO_SE_BK7,
	RO_SE_BK8,

	RO_BGM_1,
	RO_BGM_2,
	RO_BGM_3,
	RO_BGM_T,

	RO_SE_START,
	RO_SE_CLEAR,
	_RO_SE_END
};
static int ses[] = {RO_SE_RK1, RO_SE_GK1, RO_SE_BK1};
static int bgms[] = {RO_BGM_T,RO_BGM_1,RO_BGM_1,RO_BGM_2, RO_BGM_3};

// 0なし　123：あり(向き) 0x10色(0R1G2B)
static int roads[RO_Y_CNT][RO_X_CNT] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
static int roads_se[RO_Y_CNT][RO_X_CNT] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};

#define STBG_MAX 5;
static char pix_bgs[][256] = {
	"res/bg/tutorial_rh.png",
	"res/bg/stage1_bg_rh.png",
	"res/bg/stage2_R_bg_rh.png",
	"res/bg/stage3_R_bg_rh.png",
	"res/bg/stage4_R_bg_rh.png",

	"res/bg/tutorial_rh.png",
	"res/bg/stage1_bg_rh.png",
	"res/bg/stage2_G_bg_rh.png",
	"res/bg/stage3_G_bg_rh.png",
	"res/bg/stage4_G_bg_rh.png",


	"res/bg/tutorial_rh.png",
	"res/bg/stage1_bg_rh.png",
	"res/bg/stage2_B_bg_rh.png",
	"res/bg/stage3_B_bg_rh.png",
	"res/bg/stage4_R_bg_rh.png",
};
static char pix_fgs[][256] = {

	"no",
	"res/bg/stage1_fg.png",
	"res/bg/stage2_R_fg.png",
	"res/bg/stage3_R_fg.png",
	"res/bg/stage4_R_fg.png",

	"no"
	"res/bg/stage1_fg.png",
	"res/bg/stage2_G_fg.png",
	"res/bg/stage3_G_fg.png",
	"res/bg/stage4_G_fg.png",

	"no"
	"res/bg/stage1_fg.png",
	"res/bg/stage2_B_fg.png",
	"res/bg/stage3_B_fg.png",
	"res/bg/stage4_B_fg.png",

};
static char ro_pixs[_RO_SP_ALL_END][256] = {

	"",
	"",

	"res/img/R_left.png",
	"res/img/R_up.png",
	"res/img/R_down.png",

	"res/img/G_left.png",
	"res/img/G_up.png",
	"res/img/G_down.png",

	"res/img/B_left.png",
	"res/img/B_up.png",
	"res/img/B_down.png",

	"res/img/Game Over.png",
	"res/img/Retry.png",
	"res/img/yes_on.png",
	"res/img/no_on.png",

	"res/img/R_good1.png",
	"res/img/R_good2.png",
	"res/img/R_good3.png",
	"res/img/R_good4.png",
	"res/img/R_good5.png",

	"res/img/R_bad1.png",
	"res/img/R_bad2.png",

	"res/img/tutorial.png",
	"res/img/tutorial.png",
	"res/img/tutorial.png",
	"res/img/tutorial.png",

	"res/img/ch1.png",
};


static char sounds[_RO_SE_END][256] = {
	"res/se/Red01.mp3",		// RO_SE_RK1~
	"res/se/Red02.mp3",
	"res/se/Red03.mp3",
	"res/se/Red04.mp3",
	"res/se/Red05.mp3",
	"res/se/Red06.mp3",
	"res/se/Red07.mp3",
	"res/se/Red08.mp3",

	"res/se/Green01.mp3",	// RO_SE_GK1~
	"res/se/Green02.mp3",
	"res/se/Green03.mp3",
	"res/se/Green04.mp3",
	"res/se/Green05.mp3",
	"res/se/Green06.mp3",
	"res/se/Green07.mp3",
	"res/se/Green08.mp3",

	"res/se/Blue01.mp3",	// RO_SE_BK1~
	"res/se/Blue02.mp3",
	"res/se/Blue03.mp3",
	"res/se/Blue04.mp3",
	"res/se/Blue05.mp3",
	"res/se/Blue06.mp3",
	"res/se/Blue07.mp3",
	"res/se/Blue08.mp3",

	"res/bgm/Stage1.mp3",	// RO_BGM_1~
	"res/bgm/Stage2.mp3",
	"res/bgm/Stage3.mp3",
	"res/bgm/Tutorial.mp3", // RO_BGM_T

	"res/bgm/Jingle/Start.mp3",	// RO_SE_START
	"res/bgm/Jingle/Clear.mp3",	// RO_SE_CLEAR

};

SPRITE spd[_RO_SP_ALL_END] = {
	{ 0,0, 0,  0,   0, 800, 600}, // RO_SP_BG
	{ 0,0, 0,  0,   0, 800, 600}, // RO_SP_FB

	{ 0,0, 0,  0,   0, 50, 50},	// RO_SP_BR_R1~
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//
	{ 0,0, 0,  0,   0, 50, 50},	//

	{ 0,0, 0,128, 152,545, 80},	//
	{ 0,0, 0,321, 300,158, 55},	//
	{ 0,0, 0,130, 380,190, 65},	//
	{ 0,0, 0,480, 380,190, 65},	//

	{ 0,0, 0,  0,   0, 80, 50},	// RO_SP_OK1~
	{ 0,0, 0,  0,   0, 80, 50},	//
	{ 0,0, 0,  0,   0, 80, 50},	//
	{ 0,0, 0,  0,   0, 80, 50},	//
	{ 0,0, 0,  0,   0, 80, 50},	//

	{ 0,0, 0,  0,   0, 80, 50},	// RO_SP_NG1~
	{ 0,0, 0,  0,   0, 80, 50},	//

	{ 0,0, 0,100, 160,600,100},	// RO_SP_R1~
	{ 0,0, 0,100, 160,600,100},	//
	{ 0,0, 0,100, 160,600,100},	//
	{ 0,0, 0,100, 160,600,100},	//

	{ 0,0, 0,160, 100, 46, 63},	// RO_SP_ICHI(num=向き x=歩きパターン) 
};

#define I_PATMAX 27
#define I_PATS   9
#define I_MOVS   3

int ichi_num[I_PATS][I_MOVS];
int se_num[_RO_SE_END];
int sp_num[_RO_SP_END];

// 各マス位置座標
static int roadx[RO_X_CNT] = { 650, 600, 550, 500, 450, 400, 350, 300, 250, 200, 150, 100};
static int roady[RO_Y_CNT]  = { 115, 165, 215, 265, 315, 365, 415, 465};

// レベル
static RO_LEVEL levels[] = { 
	{0, 8},
	{4, 8},
	{12,12},
	{24,16},
	{32,20},
	{44,24},
	{-1,-1},
};
