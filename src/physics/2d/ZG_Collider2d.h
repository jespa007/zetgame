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
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
							);

bool Collider2d_TestIntersectionPointRectangle(
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
							, float _w2, float _h2
							);

bool Collider2d_TestIntersectionPointCircle(
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
							, float _r2
							);

bool Collider2d_TestIntersectionRectangleRectangle(
							  ZG_Vector3f _p1
							, float _w1, float _h1
							, ZG_Vector3f _p2
							, float _w2, float _h2);

bool Collider2d_TestIntersectionRectangleCircle(
		  	  	  	  	  	 ZG_Vector3f _p1
							, float _w1, float _h1
							, ZG_Vector3f _p2
							, float _r2);

bool Collider2d_TestIntersectionCircleCircle( ZG_Vector3f _p1
								, float _r1
								, ZG_Vector3f _p2
								, float _r2);

bool Collider2d_TestIntersectionCirclePoint( ZG_Vector3f _p1
								, float _r1
								, ZG_Vector3f _p2);


bool Collider2d_Test(ZG_Transform _t1, Collider2dType _c1, ZG_Transform _t2, Collider2dType _c2);

void Collider2d_Draw(ZG_Transform _t3d, Collider2dType _collider_type, ZG_Color4f _color);



#endif
