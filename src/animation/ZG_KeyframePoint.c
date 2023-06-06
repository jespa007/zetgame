#include "zg_animation.h"

ZG_KeyFramePoint * ZG_KeyFramePoint_New(void){
	ZG_KeyFramePoint * bp = ZG_NEW(ZG_KeyFramePoint);
	memset(bp, 0, sizeof(ZG_KeyFramePoint));
	return bp;
}

void ZG_KeyFramePoint_Delete(ZG_KeyFramePoint * _this){
	if(_this){
		ZG_FREE(_this);
	}
}
