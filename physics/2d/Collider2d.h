#ifndef __COLLIDER2D_H__
#define __COLLIDER2D_H__

typedef struct Collider2d Collider2d;

typedef enum{
	 COLLIDER2D_TYPE_RECTANGLE=0
	,COLLIDER2D_TYPE_CIRCLE

}Collider2dType;

struct Collider2d{
	void *data;
};

bool Collider2d_CheckCollision(Transform *_absolute_pos_coll1
							, Collider2d *coll1
							, Transform *_absolute_pos_coll2
							, Collider2d *coll2);


Collider2d * Collider2d_NewRectangle(uint16_t width, uint16_t height);
Collider2d * Collider2d_NewCircle(uint16_t diameter);
void Physics2dCollider_Delete(Collider2d * _this);




#endif
