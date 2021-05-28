#include "Graphics.h"

typedef struct ShapeData ShapeData;


Shape * Shape_New(void){
	Shape *shape=NEW(Shape);
	return shape;
}
/*
void Shape_SetTranslate3f(Shape *_this,float x, float y, float z){
	_this->transform->translate.x=x;
	_this->transform->translate.y=y;
	_this->transform->translate.z=z;
}
void Shape_SetRotate3f(Shape *_this,float x, float y, float z){
	_this->transform->rotate.x=x;
	_this->transform->rotate.y=y;
	_this->transform->rotate.z=z;

	_this->transform->quaternion=Quaternion_FromEulerV3f(_this->transform->rotate);

}*/
void Shape_SetAppearance(Shape *_this,Appearance *appearance){
	_this->appearance = appearance;
}

void Shape_Draw(Shape *_this, Transform *transform){

	Appearance_Apply(_this->appearance);
	Transform_Apply(transform);

	// TODO:Draw

	Transform_Restore(transform);
	Appearance_Restore(_this->appearance);
}

void	Shape_DrawRectangle(float width, float height, bool text_coords){
	width=width*0.5;
	height=height*0.5;

	glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
			if(text_coords) glTexCoord2f(0, 0);
			glVertex2f(-width, height);

			if(text_coords) glTexCoord2f(1, 0);
			glVertex2f(width, height);

			if(text_coords) glTexCoord2f(1,1);
			glVertex2f(-width,-height);

			if(text_coords) glTexCoord2f(0, 1);
			glVertex2f(width,-height);
	glEnd();
}

void Shape_Delete(Shape *_this){
	if(_this != NULL){
		FREE(_this);
	}
}
