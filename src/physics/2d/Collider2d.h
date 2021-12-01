#ifndef __COLLIDER2D_H__
#define __COLLIDER2D_H__

typedef struct Collider2d Collider2d;
typedef struct Transform Transform;

typedef enum{
	 COLLIDER2D_TYPE_RECTANGLE=0
	,COLLIDER2D_TYPE_CIRCLE

}Collider2dType;

struct Collider2d{
	void *data;
};

bool Collider2d_CheckCollisionRectangleRectangle(
							  Vector3f _p1
							, float _w1, float _h1
							, Vector3f _p2
							, float _w2, float _h2);

bool Collider2d_CheckCollisionRectangleCircle(
		  	  	  	  	  	 Vector3f _p1
							, float _w1, float _h1
							, Vector3f _p2
							, float _r2);

bool Collider2d_CheckCollisionCircleCircle( Vector3f _p1
								, float _r
								, Vector3f _p2
								, float _r);







#endif
