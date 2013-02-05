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
	// ���j���[��ʂőI�΂ꂽ�Q�[����ID�i�}�b�v���1�_�ɂ�
	// 1���蓖�Ă���ID�j���i�[�����B�Q�[��������Q�Ƃ���
	// �K�v�͂Ȃ��B
	int stage_id_;
	
	// [�}�b�vID]
	// �ǂ̃}�b�v���g���Ă��邩�������B
	// �i�`���[�g���A���I��p�}�b�v�A���C���}�b�v�Ȃǁj
	int map_id_;
	
	// [�V�`���G�[�V����ID]
	// ���܂ǂ̃V�[���Ȃ̂����i�[�����B
	// 0: ���j���[�őI��
	// 1: �Q�[���J�n�O
	// 2: �Q�[����
	// 3: �Q�[����
	int stage_situation_;
	static const int STAGE_SITUATIONS = 4;
	
	// [�X�e�[�W�̃��x��]
	// ���j���[��ʂőI�΂ꂽ�Q�[���̃��x�����i�[�����B
	int stage_level_;

	// [�X�e�[�W�̐F]
	// ���j���[��ʂőI�΂ꂽ�Q�[���̐F���i�[�����B
	int stage_color_;
	
	// [�Q�[�����v���C��������]
	// �Q�[���I�����A���̒l�ɃQ�[�������Ȃ��1�A���s�Ȃ��0��
	// �w�肵�ĕԂ��B
	int end_status_;
	
	// [�Ԃ̓��_]
	// ���̒l�ɁA�����_�ł̃v���C���[�̐Ԃ̓��_�i�ݐϒl�j��
	// �i�[���ꂽ��ԂŃQ�[���ɓn�����B
	// �Q�[���I�����A���̒l�ɂ��̃Q�[�����Ńv���C���[���l������
	// �Ԃ̓��_�i��L�̒l�ɑ��������ʂł͂Ȃ��j���w�肵�ĕԂ��B
	int r_score_;

	// [�΂̓��_]
	// �ԂƓ��l
	int g_score_;

	// [�̓��_]
	// �ԂƓ��l
	int b_score_;
	
public:
	GameResult() :
	  stage_id_(0), map_id_(0), stage_situation_(0), stage_level_(0), stage_color_(0), end_status_(0), r_score_(0), g_score_(0), b_score_(0) {}
	
	int stage_id(){ return stage_id_; }
	void stage_id(int si){ stage_id_ = si; }
	int map_id(){ return map_id_; }
	void map_id(int mi){ map_id_ = mi; }
	int stage_situation(){ return stage_situation_; }
	void stage_situation(int ss){ stage_situation_ = ss; }
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
