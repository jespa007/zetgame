#include "_zg_graphics_.h"

ZG_Light * ZG_Light_New(void){
	ZG_Light * light = ZG_NEW(ZG_Light);
	// Shape3d it stores transform3d and it can render for debbuging.
	light->shape = ZG_Shape_New();

	return light;
}

void ZG_Light_Delete(ZG_Light *_this){
	if(_this==NULL) return;


	free(_this);
}
