#ifndef __RECTANGLE_COLLIDER2D_H__
#define __RECTANGLE_COLLIDER2D_H__

typedef struct RectangleCollider2d RectangleCollider2d;

struct RectangleCollider2d{
	float width,height;
};

RectangleCollider2d * 	RectangleCollider2d_New(uint16_t width, uint16_t height);
void 					RectangleCollider2d_Delete(RectangleCollider2d * _this);

#endif
