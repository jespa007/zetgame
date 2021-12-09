#include "physics/zg_physics.h"

typedef struct{
	Vector2f dim;
	Collider2dType type;
}Collider2dData;

bool Collider2d_TestIntersectionRectangleRectangle(
	  Vector3f _p1
		, float _w1, float _h1
		, Vector3f _p2
		, float _w2, float _h2
){

	float w1_med=_w1*0.5;
	float w2_med=_w2*0.5;
	float h1_med=_h1*0.5;
	float h2_med=_h2*0.5;

	if (_p1.x + w1_med < _p2.x - w2_med) {
		return false;
	}
	if (_p1.y + h1_med < _p2.y - h2_med) {
		return false;
	}
	if (_p1.x - w1_med > _p2.x + w2_med) {
		return false;
	}
	if (_p1.y - h1_med > _p2.y + h2_med) {
		return false;
	}
	return true;
}

bool Collider2d_TestIntersectionRectangleCircle(
	 Vector3f _p1
	, float _w1, float _h1
	, Vector3f _p2
	, float _r2){

	// check whether in its BB
	if(Collider2d_TestIntersectionRectangleRectangle(
			_p1,_w1,_h1,_p2,_r2*2, _r2*2
	)){ // collides -> check distance distance vertexs
		return true;
	}

	return false;
}

bool Collider2d_TestIntersectionCircleCircle(
	Vector3f _p1
	, float _r1
	, Vector3f _p2
	, float _r2){
	return false;
}




