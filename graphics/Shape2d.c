#include "zg_graphics.h"
/*
Shape2d * 	Shape2d_New(void){
	Shape2d * shape2d=NEW(Shape2d);
	shape2d->shape=Shape_New();
	Shape2d_SetDimensions(shape2d,100,100);

	shape2d->u1=0;
	shape2d->v1=0;
	shape2d->u2=1;
	shape2d->v2=1;

	return shape2d;
}
*/
/*
void Shape2d_SetTranslate2f(Shape2d *_this,float x, float y){
	_this->shape->transform->translate.x=x;
	_this->shape->transform->translate.y=y;
}
*//*
void  Shape2d_SetPosition2i(Shape2d *_this,int x, int y){
	_this->position.x=x;
	_this->position.y=y;
}
*/
/*

void 		Shape2d_SetScale(Shape2d *_this,float scale){
	_this->shape->transform->scale.x=scale;
	_this->shape->transform->scale.y=scale;
	_this->shape->transform->scale.z=1;
}

void 		Shape2d_SetScale2f(Shape2d *_this,float scale_x, float scale_y){
	_this->shape->transform->scale.x=scale_x;
	_this->shape->transform->scale.y=scale_y;
	_this->shape->transform->scale.z=1;
}

void Shape2d_SetRotate(Shape2d *_this,float angle_degrees){

	Transform_RotateZ(_this->shape->transform,angle_degrees); // only performs rotation over Z axis

}*/
/*
void	 	Shape2d_SetDimensions(Shape2d *_this,Sint16 w, Sint16 h){
	Shape2d_SetWidth(_this, w);
	Shape2d_SetHeight(_this, h);
}

void	 	Shape2d_SetWidth(Shape2d *_this,Sint16 w){
	_this->width=w;
	_this->width3d=ViewPort_ScreenToWorldDimX(w)*0.5f;
}

void	Shape2d_SetHeight(Shape2d *_this,Sint16 h){
	_this->height=h;
	_this->heightransform=ViewPort_ScreenToWorldDimY(h)*0.5f;
}


void 	Shape2d_Draw(Shape2d *_this, Transform *transform){
	 // this is slow (recommend to apply/restore when is needed)
	//Appearance_Apply(_this->shape->appearance);
	Transform_Apply(transform);

	//Geometry_Draw(v2d->_this->shape->geometry);

	glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
			glTexCoord2f(_this->u1, _this->v1);
			glVertex2f(-_this->width3d, _this->heightransform);

			glTexCoord2f(_this->u2, _this->v1);
			glVertex2f(_this->width3d, _this->heightransform);

			glTexCoord2f(_this->u2, _this->v2);
			glVertex2f(-_this->width3d,-_this->heightransform);

			glTexCoord2f(_this->u2, _this->v2);
			glVertex2f(_this->width3d,-_this->heightransform);
	glEnd();

	Transform_Restore(transform);
	//Appearance_Restore(_this->shape->appearance);

}

void Shape2d_Delete(Shape2d *_this){
	if(_this == NULL){
		return;
	}

	Shape_Delete(_this->shape);
	FREE(_this);

}
*/

