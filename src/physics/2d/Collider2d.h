#ifndef __COLLIDER2D_H__
#define __COLLIDER2D_H__

typedef struct Collider2d Collider2d;

typedef enum{
	COLLIDER2D_TYPE_POINT=0
	,COLLIDER2D_TYPE_RECTANGLE
	,COLLIDER2D_TYPE_CIRCLE

}Collider2dType;

struct Collider2d{
	void *data;
};

bool Collider2d_TestIntersectionPointPoint(
							  Vector3f _p1
							, Vector3f _p2
							);

bool Collider2d_TestIntersectionPointRectangle(
							  Vector3f _p1
							, Vector3f _p2
							, float _w2, float _h2
							);

bool Collider2d_TestIntersectionPointCircle(
							  Vector3f _p1
							, Vector3f _p2
							, float _r2
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


bool Collider2d_Test(Transform _t1, Collider2dType _c1, Transform _t2, Collider2dType _c2);

void Collider2d_Draw(Transform _t3d, Collider2dType _collider_type, Color4f _color);



#endif
