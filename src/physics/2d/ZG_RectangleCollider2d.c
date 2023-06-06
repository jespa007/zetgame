#include "physics/zg_physics.h"


RectangleCollider2d * RectangleCollider2d_New(uint16_t _width, uint16_t _height){
	RectangleCollider2d *collider=ZG_NEW(RectangleCollider2d);
	collider->width=ZG_ViewPort_ScreenToWorldWidth(_width)*0.5;
	collider->height=ZG_ViewPort_ScreenToWorldHeight(_height)*0.5;
	return collider;
}


void RectangleCollider2d_Delete(RectangleCollider2d * _this){
	ZG_FREE(_this);
}

