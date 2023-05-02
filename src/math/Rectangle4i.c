
#include "zg_math.h"

Rectangle4i  Rectangle4i_New(void){
	return (Rectangle4i){
		 .x1=0
		,.y1=0
		,.x2=0
		,.y2=0
	};
}

Rectangle4i  Rectangle4i_New4i(
		int _x1
		,int _y1
		,int _x2
		,int _y2
){
	return (Rectangle4i){
		 .x1=_x1
		,.y1=_y1
		,.x2=_x2
		,.y2=_y2
	};
}
