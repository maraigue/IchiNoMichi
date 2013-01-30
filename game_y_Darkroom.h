#pragma once
#include "scene.hpp"
#include "game_y_GameObject.h"

#define SOUND_NUM 128
#define ENEMY_NUM 8


class Darkroom :
	public Scene
{
public:
	Darkroom(void);
	~Darkroom(void);
private:
	int frame;
	long time;
	Player player;
	int damaged;
	SoundSource sound[SOUND_NUM];
	Enemy enemy[ENEMY_NUM];
	int soundImage[3];
	int fx_r[8], fx_g[8], fx_b[8];
	int get[3];
	int forground;
	int background;
	int bgm;
	int gameEnd;
	int lastLoop;
	long limitTime;
	GameResult result;
public:
	void init(GameResult* result);
	GameResult* update(void);
	void draw(void);
private:
	void init0(void);
	void init1(void);
	void init2(void);
	void init3(void);
	void init4(void);

	void setSound(int type, SoundSource* ss);

};

