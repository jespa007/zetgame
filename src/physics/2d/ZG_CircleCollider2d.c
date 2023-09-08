#include "physics/@zg_physics.h"


ZG_CircleCollider2d * ZG_CircleCollider2d_New(uint16_t _radius){
	ZG_CircleCollider2d *collider=ZG_NEW(ZG_CircleCollider2d);
	collider->radius=ZG_ViewPort_ScreenToWorldWidth(_radius);
	return collider;
}

void ZG_CircleCollider2d_Delete(ZG_CircleCollider2d * _this){
	ZG_FREE(_this);
}
