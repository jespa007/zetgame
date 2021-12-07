#include "physics/zg_physics.h"

typedef struct{
	Vector2f dim;
	Collider2dType type;
}Collider2dData;

bool Collider2d_CheckCollisionRectangleRectangle(
	  Vector3f _p1
		, float _w1, float _h1
		, Vector3f _p2
		, float _w2, float _h2
){

	if (_p1.x + _w1 < _p2.x - _w2) {
		return false;
	}
	if (_p1.y + _h1 < _p2.y - _h2) {
		return false;
	}
	if (_p1.x - _w1 > _p2.x + _w2) {
		return false;
	}
	if (_p1.y - _h1 > _p2.y + _h2) {
		return false;
	}
	return true;
}

bool Collider2d_CheckCollisionRectangleCircle(
	 Vector3f _p1
	, float _w1, float _h1
	, Vector3f _p2
	, float _r2){



	return false;
}

bool Collider2d_CheckCollisionCircleCircle(
	Vector3f _p1
	, float _r1
	, Vector3f _p2
	, float _r2){
	return false;
}




