#include "zg_graphics.h"

Light * Light_New(void){
	Light * light = ZG_NEW(Light);
	// Shape3d it stores transform3d and it can render for debbuging.
	light->shape = Shape_New();

	return light;
}

void Light_Delete(Light *_this){
	if(_this==NULL) return;


	free(_this);
}
