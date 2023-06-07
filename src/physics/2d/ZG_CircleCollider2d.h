#ifndef __ZG_CIRCLE_COLLIDER2D_H__
#define __ZG_CIRCLE_COLLIDER2D_H__

typedef struct ZG_CircleCollider2d ZG_CircleCollider2d;

struct ZG_CircleCollider2d{
	float radius;
};


ZG_CircleCollider2d * ZG_CircleCollider2d_New(uint16_t _radius);
void ZG_CircleCollider2d_Delete(ZG_CircleCollider2d * _this);

#endif
