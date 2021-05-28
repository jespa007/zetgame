#ifndef __PHYSICS2D_COLLIDER_H__
#define __PHYSICS2D_COLLIDER_H__

typedef struct Physics2dCollider Physics2dCollider;

struct Physics2dCollider{
	void *data;
};


Physics2dCollider * Physics2dCollider_New(void);
void Physics2dCollider_Delete(Physics2dCollider * _this);




#endif
