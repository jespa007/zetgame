#ifndef __COLLIDER2D_H__
#define __COLLIDER2D_H__

typedef struct Collider2d Collider2d;
typedef struct Transform Transform;

typedef enum{
	COLLIDER2D_TYPE_POINT=0
	,COLLIDER2D_TYPE_RECTANGLE
	,COLLIDER2D_TYPE_CIRCLE

}Collider2dType;

struct Collider2d{
	void *data;
};

bool Collider2d_TestIntersectionRectanglePoint(
							  Vector3f _p1
							, float _w1, float _h1
							, Vector3f _p2
							);

bool Collider2d_TestIntersectionRectangleRectangle(
							  Vector3f _p1
							, float _w1, float _h1
							, Vector3f _p2
							, float _w2, float _h2);

bool Collider2d_TestIntersectionRectangleCircle(
		  	  	  	  	  	 Vector3f _p1
							, float _w1, float _h1
							, Vector3f _p2
							, float _r2);

bool Collider2d_TestIntersectionCircleCircle( Vector3f _p1
								, float _r1
								, Vector3f _p2
								, float _r2);

bool Collider2d_TestIntersectionCirclePoint( Vector3f _p1
								, float _r1
								, Vector3f _p2);







#endif
