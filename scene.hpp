#ifndef _SCENE_HPP_
#define _SCENE_HPP_

class GameResult;

// �e�Q�[����e���j���[��ʂ́Autils.hpp�ɒ�`����Ă���Scene�N���X��
// �p�����Ă���K�v������B
// - ���Y�Q�[�����ꂩ��n�߂�ۂɌĂ΂�郁�\�b�h�iinit�j�ɂ�
//   GameResult�N���X�̃C���X�^���X���n�����B
// - �f�[�^�X�V���\�b�h�iupdate�j���A�Q�[�����ʂƂ���GameResult�N���X��
//   �C���X�^���X�ւ̃|�C���^��Ԃ��ƁA���Y�Q�[���̏I���Ƃ݂Ȃ����B
//   �Ȃ��Aupdate�ŃQ�[�����I�����Ȃ��ꍇ�ɂ́ANULL��Ԃ��΂悢�B

class Scene{
public:
	// [���Y�Q�[�����ꂩ��n�߂�ۂɌĂ΂�郁�\�b�h]
	// �R���X�g���N�^�Ƃ͕ʂɒ�`���ĉ������B
	// - �R���X�g���N�^�F�����ނ̃Q�[���ł����1��Ăׂ΂悢����i�摜�̃��[�h�Ȃǁj
	// - init���\�b�h�F1�Q�[���n�߂邲�ƂɌĂ΂��ׂ�����i�t�B�[���h�̏�Ԃ̃��Z�b�g�Ȃǁj
	// GameResult�ɁA�I�΂ꂽ�X�e�[�W�F����сA�����_�ł̃v���C���[�̊e�F�̓��_���i�[����Ă��܂��B
	virtual void init(GameResult * result) = 0;

	// [�f�[�^�i���W�b�N�j���X�V���郁�\�b�h]
	// �����_�ł́A�Ă΂��^�C�~���O�͕s��iupdate��draw���Ăяo����A
	// WaitTimer(10)�����̂��ɍēxupdate��draw���Ăԁj�ł��B
	// �����fps���ƂɌĂԋ@�\�𓱓����邩���H
	// �����ŕԂ�l��NULL���ƃQ�[�����p�����AGameResult�C���X�^���X�ւ�
	// �|�C���^��Ԃ��ƃQ�[�����I�����܂��B
	virtual GameResult * update(void) = 0;
	
	// [�`����s�����\�b�h]
	// update���Ă΂ꂽ��A����ɂ��̃��\�b�h�̎��s�Ɉڂ�܂��B
	// �iupdate�̕Ԃ�l��NULL�łȂ��ꍇ�͏����j
	virtual void draw(void) = 0;
};

// �Q�[���̌��ʂ�A���j���[��ʂɂ�����X�e�[�W�I���̌��ʂ������n���N���X
class GameResult{
	// [�X�e�[�WID]
	// �Q�[����ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// �Q�[����ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	// ���j���[��ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// ���j���[��ʂ�GameResult��Ԃ��Ƃ��F���Ƀv���C����Q�[�����i�[����
	int stage_id_;

	// [�X�e�[�W�̃��x��]
	// �Q�[����ʂ�GameResult���󂯎�����Ƃ��F���j���[��ʂőI�΂ꂽ�Q�[���̃��x��������
	// �Q�[����ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	// ���j���[��ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// ���j���[��ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	int stage_level_;

	// [�X�e�[�W�̐F]
	// �Q�[����ʂ�GameResult���󂯎�����Ƃ��F���j���[��ʂőI�΂ꂽ�Q�[���̐F������
	// �Q�[����ʂ�GameResult��Ԃ��Ƃ��F��������
	// ���j���[��ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// ���j���[��ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	int stage_color_;

	// [�Q�[�����v���C��������]
	// �Q�[����ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// �Q�[����ʂ�GameResult��Ԃ��Ƃ��F�Q�[�����v���C�������ʂ�Ԃ��i����: 1, ���s: 0�j
	// ���j���[��ʂ�GameResult���󂯎�����Ƃ��F���ɎQ�Ƃ���K�v�Ȃ�
	// ���j���[��ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	int end_status_;

	// [�Ԃ̓��_]
	// �Q�[����ʂ�GameResult���󂯎�����Ƃ��F�����_�ł̃v���C���[�̐Ԃ̓��_�i�ݐϒl�j������
	// �Q�[����ʂ�GameResult��Ԃ��Ƃ��F���̃Q�[���Ŋl�������Ԃ̓��_���i�[����
	// ���j���[��ʂ�GameResult���󂯎�����Ƃ��F�I�ׂ�X�e�[�W�����肷��̂ɗ��p����
	// ���j���[��ʂ�GameResult��Ԃ��Ƃ��F���ɕύX����K�v�Ȃ�
	int r_score_;

	// [�΂̓��_]
	// �ԂƓ��l
	int g_score_;

	// [�̓��_]
	// �ԂƓ��l
	int b_score_;
	
public:
	GameResult() :
	  stage_id_(0), stage_level_(0), stage_color_(0), end_status_(0), r_score_(0), g_score_(0), b_score_(0) {}
	GameResult(int si, int sl, int sc, int st, int r, int g, int b) :
	  stage_id_(si), stage_level_(sl), stage_color_(sc), end_status_(st), r_score_(r), g_score_(g), b_score_(b) {}
	
	int stage_id(){ return stage_id_; }
	void stage_id(int si){ stage_id_ = si; }
	
	int stage_level(){ return stage_level_; }
	void stage_level(int sl){ stage_level_ = sl; }
	int stage_color(){ return stage_color_; }
	void stage_color(int sc){ stage_color_ = sc; }
	int end_status(){ return end_status_; }
	void end_status(int st){ end_status_ = st; }
	int r_score(){ return r_score_; }
	void r_score(int r){ r_score_ = r; }
	int g_score(){ return g_score_; }
	void g_score(int g){ g_score_ = g; }
	int b_score(){ return b_score_; }
	void b_score(int b){ b_score_ = b; }
};

#endif // _SCENE_HPP_
