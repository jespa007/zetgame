#include "Graphics.h"

typedef struct ShapeData ShapeData;


Shape * Shape_New(void){
	Shape *shape=NEW(Shape);
	return shape;
}

void Shape_SetAppearance(Shape *_this,Appearance *appearance){
	_this->appearance = appearance;
}

void Shape_Draw(Shape *_this, Transform *_transform){

	Appearance_Apply(_this->appearance);
	Transform_Apply(_transform);

	Geometry_Draw(_this->geometry);

	Transform_Restore(_transform);
	Appearance_Restore(_this->appearance);
}

void Shape_Delete(Shape *_this){
	if(_this != NULL){
		FREE(_this);
	}
}
