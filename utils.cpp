#include "utils.hpp"

// beginˆÈãend–¢–‚Ì’l‚ğ•Ô‚·—”
int int_rand(int begin, int end){
	if(begin >= end) return begin;

	int result;
	do{
		result = begin + (rand() / (RAND_MAX / (end - begin)));
	}while(result >= end);
	return result;
}
