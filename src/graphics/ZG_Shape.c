#include "@zg_graphics.h"

typedef struct ZG_ShapeData ZG_ShapeData;


ZG_Shape * ZG_Shape_New(void){
	ZG_Shape *shape=ZG_NEW(ZG_Shape);
	return shape;
}

void ZG_Shape_SetAppearance(ZG_Shape *_this,ZG_Appearance *appearance){
	_this->appearance = appearance;
}

void ZG_Shape_Draw(ZG_Shape *_this, ZG_Transform *_transform){

	ZG_Appearance_Apply(_this->appearance);
	ZG_Transform_Apply(_transform);

	ZG_Geometry_Draw(_this->geometry);

	ZG_Transform_Restore(_transform);
	ZG_Appearance_Restore(_this->appearance);
}

void ZG_Shape_Delete(ZG_Shape *_this){
	if(_this != NULL){
		ZG_FREE(_this);
	}
}
