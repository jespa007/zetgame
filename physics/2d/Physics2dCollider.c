#include "core/physics/zg_physics.h"

Physics2dCollider * Physics2dCollider_New(void){
	Physics2dCollider *p2d_collider=NEW(Physics2dCollider);
	return p2d_collider;
}

void Physics2dCollider_Delete(Physics2dCollider * _this){
	FREE(_this);
}

