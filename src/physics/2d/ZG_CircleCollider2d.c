#include "physics/zg_physics.h"


CircleCollider2d * CircleCollider2d_New(uint16_t _radius){
	CircleCollider2d *collider=ZG_NEW(CircleCollider2d);
	collider->radius=ZG_ViewPort_ScreenToWorldWidth(_radius);
	return collider;
}

void CircleCollider2d_Delete(CircleCollider2d * _this){
	ZG_FREE(_this);
}
