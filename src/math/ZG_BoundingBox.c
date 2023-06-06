
#include "zg_math.h"

BoundingBox  BoundingBox_New(void){
	return (BoundingBox){
		 .minx=0
		,.miny=0
		,.maxx=0
		,.maxy=0
	};
}

BoundingBox  BoundingBox_New4f(
		float _minx
		,float _miny
		,float _maxx
		,float _maxy
){
	return (BoundingBox){
		 .minx=_minx
		,.miny=_miny
		,.maxx=_maxx
		,.maxy=_maxy
	};
}
