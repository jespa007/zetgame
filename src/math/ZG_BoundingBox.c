
#include "zg_math.h"

ZG_BoundingBox  BoundingBox_New(void){
	return (ZG_BoundingBox){
		 .minx=0
		,.miny=0
		,.maxx=0
		,.maxy=0
	};
}

ZG_BoundingBox  ZG_BoundingBox_New4f(
		float _minx
		,float _miny
		,float _maxx
		,float _maxy
){
	return (ZG_BoundingBox){
		 .minx=_minx
		,.miny=_miny
		,.maxx=_maxx
		,.maxy=_maxy
	};
}
