
#include "zg_math.h"

BoundingBox  BoundingBox_New(void){
	return (Rectangle4i){
		 .minx=0
		,.miny=0
		,.maxx=0
		,.maxy=0
	};
}

BoundingBox  BoundingBox_New4i(
		float _minx
		,float _miny
		,float _maxx
		,float _maxy
){
	return (BoundingBox){
		 .x1=_minx
		,.y1=_miny
		,.x2=_maxx
		,.y2=_maxy
	};
}
