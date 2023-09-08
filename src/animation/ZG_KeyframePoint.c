#include "@zg_animation.h"

ZG_KeyframePoint * ZG_KeyframePoint_New(void){
	ZG_KeyframePoint * bp = ZG_NEW(ZG_KeyframePoint);
	memset(bp, 0, sizeof(ZG_KeyframePoint));
	return bp;
}

void ZG_KeyframePoint_Delete(ZG_KeyframePoint * _this){
	if(_this){
		ZG_FREE(_this);
	}
}
