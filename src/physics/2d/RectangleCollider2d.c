#include "physics/zg_physics.h"


RectangleCollider2d * RectangleCollider2d_New(uint16_t _width, uint16_t _height){
	RectangleCollider2d *collider=ZG_NEW(RectangleCollider2d);
	collider->width=ViewPort_ScreenToWorldWidth(_width)*0.5;
	collider->height=ViewPort_ScreenToWorldHeight(_height)*0.5;
	return collider;
}


void RectangleCollider2d_Delete(RectangleCollider2d * _this){
	FREE(_this);
}

