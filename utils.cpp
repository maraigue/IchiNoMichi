#include "utils.hpp"

// begin�ȏ�end�����̒l��Ԃ�����
int int_rand(int begin, int end){
	if(begin >= end) return begin;

	int result;
	do{
		result = begin + (rand() / (RAND_MAX / (end - begin)));
	}while(result >= end);
	return result;
}
