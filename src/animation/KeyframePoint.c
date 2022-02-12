#include "zg_animation.h"

KeyframePoint * KeyframePoint_New(void){
	KeyframePoint * bp = ZG_NEW(KeyframePoint);
	memset(bp, 0, sizeof(KeyframePoint));
	return bp;
}

void KeyframePoint_Delete(KeyframePoint * _this){
	if(_this){
		FREE(_this);
	}
}
