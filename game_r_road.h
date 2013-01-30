
/*
	by rocca
	   -> 2013/01/25
*/

#pragma once
#include <DxLib.h>
#include "scene.hpp"
#define SC_WIDTH 800
#define SC_HEIGHT 600

// �����u���b�N�f�[�^
typedef struct {
	int k;	// �ǂ�y�ʒu
	int x;
	int type;	// 0:���A�� 1:��A���@2:���A��
	int ok;	// �����u���b�N ->1:�ړ��� -1:������ 0:�Ȃ� ���s�u���b�N->���l
	int col;	// �F RGB(0~2)
}BLOCK;

// ��Փx
typedef struct {
	int road;	// �����Ɏ���u���b�N��
	int speed;	// ���x
}LO_LEVEL;

// �摜�f�[�^
typedef struct {
	int num, pat;	// �����̏ꍇ�n���h���E�p�^�[��
	int x,y;
	int w,h;
	
}SPRITE;
// �i�s
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
	int cond;	// 0:�N���X���� 1:�ʏ� 2:end���� 3:���Send
	int time;		// �g�[�^������
	int miss;		// �~�X
	int s_col;		// �J���[
	int stage;		// �X�e�[�W
	int get_rgb[3]; 
	int alpha;
	int move_time; // �u���b�N�ړ�����
	int next_time;	// ���u���b�N���ߎ���
	int all_road;	// ���Z�b�g�u���b�N��

	int now_level;	// ��Փx
	int lose_cnt;	// �s�k�~�X��

	int lo_key;		// �������L�[
	int myroad;		// �ŏI���i��j

	int once_btn;	//�A�Ŗh�~�t���O 0:�Ȃ� 1:�����@2:���� 4:Enter
	BLOCK block;		// �\�������u���b�N
	BLOCK del_block;	// ���s�����u���b�N

	int ch_time;	// �����p�^�[���`�F���W����
	int ch_movetime;// �����ړ�����
	int ch_mode;	// 0: �Q�[�����@1:�S�[����
	int ch_walk;	// �ړ����
	POINT reg;		// �����������W

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

