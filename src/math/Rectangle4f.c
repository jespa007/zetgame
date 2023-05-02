
#include "zg_math.h"

Rectangle4f  Rectangle4f_New4f(float x1,float y1,float x2,float y2){
	return (Rectangle4f){
		 .x1=x1
		,.y1=y1
		,.x2=x2
		,.y2=y2
	};
}
