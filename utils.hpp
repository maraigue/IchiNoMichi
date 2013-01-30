#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <stdlib.h>
#include "DxLib.h"

int IS_AFTER_OPENING(int n);
int IS_AFTER_TUTORIAL_GAME(int n);
int IS_AFTER_MAIN_GAME(int n);

const int NUMBER_OF_COLORS = 3;
const int NUMBER_OF_LEVELS = 3;
const int DISPLAY_WIDTH = 800;
const int DISPLAY_HEIGHT = 600;

// begin以上end未満の値を返す乱数
int int_rand(int begin, int end);

// cycle_msecミリ秒のサイクルで、0からvalue_max-1の値を取って変動する
class Cycle{
private:
	int cycle;
	int last_time;
	int offset;
	int vmax;

public:
	Cycle(int cycle_msec, int value_max) : cycle(cycle_msec), vmax(value_max), last_time(0) {}
	
	int get(int multiplier = 1){
		int current_time = GetNowCount();
		if(last_time == 0){
			offset = 0;
		}else{
			offset += (current_time - last_time) * vmax * multiplier;
		}
		last_time = current_time;

		offset %= (cycle * vmax);
		return offset / cycle;
	}
};

#endif // _UTILS_HPP_
