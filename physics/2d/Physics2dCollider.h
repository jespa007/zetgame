#ifndef __PHYSICS2D_COLLIDER_H__
#define __PHYSICS2D_COLLIDER_H__

typedef struct Physics2dCollider Physics2dCollider;

typedef enum{
	 PHYSICS2D_COLLIDER_QUAD=0
	,PHYSICS2D_COLLIDER_CIRCLE

}Physics2dCollider;

struct Physics2dCollider{
	void *data;
};


Physics2dCollider * Physics2dCollider_NewQuad(uint16_t width, uint16_t height);
Physics2dCollider * Physics2dCollider_NewCircle(uint16_t diameter);
void Physics2dCollider_Delete(Physics2dCollider * _this);




#endif
