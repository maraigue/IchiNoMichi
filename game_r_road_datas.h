#define RO_X_CNT 12
#define RO_Y_CNT 8
int now_br;

// �摜ID
enum{
	LO_SP_BG,
	LO_SP_FB,

	LO_SP_BR_R1,
	LO_SP_BR_R2,
	LO_SP_BR_R3,

	LO_SP_BR_G1,
	LO_SP_BR_G2,
	LO_SP_BR_G3,

	LO_SP_BR_B1,
	LO_SP_BR_B2,
	LO_SP_BR_B3,

	LO_SP_LOSE1,
	LO_SP_LOSE2,
	LO_SP_LOSE3,
	LO_SP_LOSE4,

	_LO_SP_END, // �ꊇ�ǂݍ���SP�����܂�

	LO_SP_ICHI = _LO_SP_END,	// �����Ǎ��J�n,
	_LO_SP_ALL_END,
};
static int cols[] = {LO_SP_BR_R1, LO_SP_BR_G1, LO_SP_BR_B1};

// ���ʉ�ID
// �e�F�W�탉���_�������_��
enum{
	LO_SE_RK1,	// ����
	LO_SE_RK2,	//
	LO_SE_RK3,	//
	LO_SE_RK4,
	LO_SE_RK5,
	LO_SE_RK6,
	LO_SE_RK7,
	LO_SE_RK8,

	LO_SE_GK1,	// �݂ǂ�
	LO_SE_GK2,	// 
	LO_SE_GK3,	// 
	LO_SE_GK4,
	LO_SE_GK5,
	LO_SE_GK6,
	LO_SE_GK7,
	LO_SE_GK8,

	LO_SE_BK1,	// ����
	LO_SE_BK2,	// 
	LO_SE_BK3,	// 
	LO_SE_BK4,
	LO_SE_BK5,
	LO_SE_BK6,
	LO_SE_BK7,
	LO_SE_BK8,

	LO_BGM_1,
	LO_BGM_2,
	LO_BGM_3,
	LO_BGM_T,

//	LO_SE_CLEAR,
//	LO_SE_LOSE,
	_LO_SE_END
};
static int ses[] = {LO_SE_RK1, LO_SE_GK1, LO_SE_BK1};
static int bgms[] = {LO_BGM_T,LO_BGM_1,LO_BGM_1,LO_BGM_2, LO_BGM_3};
// 0�Ȃ��@123�F����(����) 0x10�F(0R1G2B)
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

// ��init�p
static int tmp_roads[RO_Y_CNT][RO_X_CNT] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
static int tmp_roads_se[RO_Y_CNT][RO_X_CNT] = {
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},	// 
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
// ���݂̗ݐϏ�
static int now_n[RO_Y_CNT] = { 11, 11, 11, 11, 11, 11, 11, 11};

#define STBG_MAX 5;
static char pix_bgs[][256] = {
	"res/bg/tutorial_rh.png",
	"res/bg/stage1_R_bg_rh.png",
	"res/bg/stage2_R_bg_rh.png",
	"res/bg/stage3_R_bg_rh.png",
	"res/bg/stage4_R_bg_rh.png",

	"res/bg/tutorial_rh.png",
	"res/bg/stage1_G_bg_rh.png",
	"res/bg/stage2_G_bg_rh.png",
	"res/bg/stage3_G_bg_rh.png",
	"res/bg/stage4_G_bg_rh.png",


	"res/bg/tutorial_rh.png",
	"res/bg/stage1_B_bg_rh.png",
	"res/bg/stage2_B_bg_rh.png",
	"res/bg/stage3_B_bg_rh.png",
	"res/bg/stage4_R_bg_rh.png",
};
static char pix_fgs[][256] = {

	"",
	"res/bg/stage1_R_fg_rh.png",
	"res/bg/stage2_R_fg_rh.png",
	"res/bg/stage3_R_fg_rh.png",
	"res/bg/stage4_R_fg_rh.png",

	""
	"res/bg/stage1_G_fg_rh.png",
	"res/bg/stage2_G_fg_rh.png",
	"res/bg/stage3_G_fg_rh.png",
	"res/bg/stage4_G_fg_rh.png",

	""
	"res/bg/stage1_B_fg_rh.png",
	"res/bg/stage2_B_fg_rh.png",
	"res/bg/stage3_B_fg_rh.png",
	"res/bg/stage4_B_fg_rh.png",

};
static char pixs_R[_LO_SP_ALL_END][256] = {

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

	"res/img/ch1.png",
};


static char sounds[_LO_SE_END][256] = {
	"res/se/Red01.mp3",	// ����
	"res/se/Red02.mp3",
	"res/se/Red03.mp3",
	"res/se/Red04.mp3",
	"res/se/Red05.mp3",
	"res/se/Red06.mp3",
	"res/se/Red07.mp3",
	"res/se/Red08.mp3",

	"res/se/Green01.mp3",	// �݂ǂ�
	"res/se/Green02.mp3",
	"res/se/Green03.mp3",
	"res/se/Green04.mp3",
	"res/se/Green05.mp3",
	"res/se/Green06.mp3",
	"res/se/Green07.mp3",
	"res/se/Green08.mp3",

	"res/se/Blue01.mp3",	// ����
	"res/se/Blue02.mp3",
	"res/se/Blue03.mp3",
	"res/se/Blue04.mp3",
	"res/se/Blue05.mp3",
	"res/se/Blue06.mp3",
	"res/se/Blue07.mp3",
	"res/se/Blue08.mp3",

	"res/bgm/Stage1.mp3",
	"res/bgm/Stage2.mp3",
	"res/bgm/Stage3.mp3",
	"res/bgm/Tutorial.mp3",
//	"res/se/se_003.wav",
//	"res/se/se_003.wav",
};

SPRITE spd[_LO_SP_ALL_END] = {
	{ 0,0,   0,   0, 800, 600}, // bg_b
	{ 0,0,   0,   0, 800, 600}, // bg_f

	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0,   0,   0, 50, 50},	// �u���b�N�i��{�T�C�Y�f�[�^�j

	{ 0,0, 128, 100,545, 80},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0, 440, 240,158, 55},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0, 105, 420,190, 65},	// �u���b�N�i��{�T�C�Y�f�[�^�j
	{ 0,0, 505, 420,190, 65},	// �u���b�N�i��{�T�C�Y�f�[�^�j

	{ 0,0, 160, 100, 46, 63},	// �L����(num=���� x=�����p�^�[��) 
};


int ichi_num[8][5];		// �����@�p�^�[��

int se_num[_LO_SE_END]; // ���ԍ�
int sp_num[_LO_SP_END];	// �摜�ԍ�

// �e�ʒu���W
static int roadx[RO_X_CNT] = { 650, 600, 550, 500, 450, 400, 350, 300, 250, 200, 150, 100};
static int roady[RO_Y_CNT]  = { 115, 165, 215, 265, 315, 365, 415, 465};

// ���x��
static LO_LEVEL levels[] = { 
	{0, 8},
	{2, 8},
	{6, 12},
	{10,12},
	{14,16},
	{18,16},
	{22,20},
	{26,20},
	{32,24},
	{-1,-1},
};

#define I_PATMAX 27 //40
#define I_PATS   8
#define I_MOVS   3   //5
//