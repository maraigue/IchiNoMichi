#pragma once

class MovePattern
{
public:
	MovePattern(void);
	~MovePattern(void);
	virtual void move(double* x, double* y) = 0;
};

class MoveDirect :public MovePattern
{
public:
	double dest_x;
	double dest_y; 
	double speed;

	void move(double* x, double* y);
};

class MoveReflect : public MovePattern
{
public:
	double vx;
	double vy;
public:
	MoveReflect(void);
	void move(double* x, double* y);
};

class MoveCircular :public MovePattern{
public:

	double radius;
	int angle;
	void move(double* x, double* y);
};

class MoveEscape : public MovePattern {

public:
	double* player_x;
	double* player_y;
public:
	void move(double* x, double* y);

};

class MoveSinWave: public MovePattern {
public:
	double width;
	double base;
	double angle;
	int dx;
	double av;
public:
	MoveSinWave(double base, double av, double width = 50);
	void move(double* x, double* y);

};

