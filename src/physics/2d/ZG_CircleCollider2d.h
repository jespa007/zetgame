#ifndef __CIRCLE_COLLIDER2D_H__
#define __CIRCLE_COLLIDER2D_H__

typedef struct CircleCollider2d CircleCollider2d;

struct CircleCollider2d{
	float radius;
};


CircleCollider2d * CircleCollider2d_New(uint16_t _radius);
void CircleCollider2d_Delete(CircleCollider2d * _this);

#endif
