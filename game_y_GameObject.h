#pragma once

#include <DxLib.h>
#include "game_y_MovePattern.h"


class GameObject
{
public:
	double x;
	double y;
	int count;
	int visible;
	int moveSound;
	MovePattern* pattern;
public:
	GameObject(void);
	~GameObject(void);

	virtual void draw(void) = 0;
};


class SoundWave
	:public GameObject
{
public:
	MovePattern* pattern;
	int color;

public:
	SoundWave(void);
	double radius;
	void draw(void);

};


#define CHIP_NUM 40
class Player :
	public GameObject
{
public:
	int scoreImage;
	int secencer;
	int dir;
	int charaChip[CHIP_NUM];
	SoundWave wave;
public:
	Player(void);
	~Player(void);

	void beep(void);
	void update(void);
	void draw(void);

	void translate(double x, double y);
};


class SoundSource
	: public GameObject
{
public:
	SoundWave wave;
	int caught;
	double v_x;
	double v_y;
	int type;
	int image;
public:
	SoundSource(void);
	void beep(void);
	void draw(void);
	void update(void);
};

class Enemy : public GameObject {

public:
	Enemy(void);
	void beep(void);
	void draw(void);
	void update(void);
};

