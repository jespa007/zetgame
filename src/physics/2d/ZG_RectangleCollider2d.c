#include "physics/@zg_physics.h"


ZG_RectangleCollider2d * ZG_RectangleCollider2d_New(uint16_t _width, uint16_t _height){
	ZG_RectangleCollider2d *collider=ZG_NEW(ZG_RectangleCollider2d);
	collider->width=ZG_ViewPort_ScreenToWorldWidth(_width)*0.5;
	collider->height=ZG_ViewPort_ScreenToWorldHeight(_height)*0.5;
	return collider;
}


void ZG_RectangleCollider2d_Delete(ZG_RectangleCollider2d * _this){
	ZG_FREE(_this);
}

