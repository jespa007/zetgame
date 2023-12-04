
#include "@zg_math.h"

ZG_BoundingBox  ZG_BoundingBox_New(void){
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

ZG_BoundingBox  ZG_BoundingBox_MulFactor(ZG_BoundingBox _bb, float _factor){

	return (ZG_BoundingBox){
		.minx=_bb.minx *_factor
		,.miny=_bb.miny * _factor
		,.maxx=_bb.maxx * _factor
		,.maxy=_bb.maxy * _factor
	};
}
