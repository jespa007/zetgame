#ifndef __ZG_RECTANGLE_COLLIDER2D_H__
#define __ZG_RECTANGLE_COLLIDER2D_H__

typedef struct ZG_RectangleCollider2d ZG_RectangleCollider2d;

struct ZG_RectangleCollider2d{
	float width,height;
};

ZG_RectangleCollider2d * 	ZG_RectangleCollider2d_New(uint16_t width, uint16_t height);
void 					ZG_RectangleCollider2d_Delete(ZG_RectangleCollider2d * _this);

#endif
