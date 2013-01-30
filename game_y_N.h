#pragma once

class N {
public:
	double x;
	double y;
	int visible;
	N();
	N(double x, double y);
	
	void setPos(double x, double y);
	void draw();

};