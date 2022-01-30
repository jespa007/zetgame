#include "physics/zg_physics.h"

typedef struct{
	Vector2f dim;
	Collider2dType type;
}Collider2dData;


bool Collider2d_TestIntersectionPointPoint(
							  Vector3f _p1
							, Vector3f _p2
							){
	return _p1.x==_p2.x && _p1.y == _p2.y;
}


bool Collider2d_TestIntersectionPointRectangle(
							  Vector3f _p1
							, Vector3f _p2
							, float _w2, float _h2
							){
	float w2_med=_w2*0.5;
	float h2_med=_h2*0.5;

	if (_p1.x < _p2.x-w2_med) {
		return false;
	}
	if (_p1.y < _p2.y-h2_med) {
		return false;
	}
	if (_p1.x > _p2.x+w2_med) {
		return false;
	}
	if (_p1.y > _p2.y+ h2_med) {
		return false;
	}
	return true;

}

bool Collider2d_TestIntersectionPointCircle(
							  Vector3f _p1
							, Vector3f _p2
							, float _r2
							){
	float xdiff=fabs(_p1.x-_p2.x);
	float ydiff=fabs(_p1.y-_p2.y);

	float distance=(xdiff)*(xdiff)+
					  (ydiff)*(ydiff);

	return distance < _r2*_r2;
}

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
	float w1_med=_w1*0.5;
	float h1_med=_h1*0.5;

    float circle_distance_x = fabs(_p2.x - _p1.x);
    float circle_distance_y = fabs(_p2.y - _p1.y);

   	if (circle_distance_x > (w1_med + _r2)) { return false; }
   	if (circle_distance_y > (h1_med + _r2)) { return false; }

   	if (circle_distance_x <= (w1_med)) { return true; }
   	if (circle_distance_y <= (h1_med)) { return true; };

   	float xdiff=circle_distance_x-w1_med;
   	float ydiff=circle_distance_y-h1_med;

	float corner_distance=(xdiff)*(xdiff)+
						  (ydiff)*(ydiff);

	return corner_distance<=_r2*_r2;
}

bool Collider2d_TestIntersectionCircleCircle(
	Vector3f _p1
	, float _r1
	, Vector3f _p2
	, float _r2){

	float xdiff=fabs(_p1.x-_p2.x);
	float ydiff=fabs(_p1.y-_p2.y);

	float distance=(xdiff)*(xdiff)+
					  (ydiff)*(ydiff);

	return distance < _r1*_r1 || distance < _r2*_r2;

}

void Collider2d_Draw(Transform _t3d, Collider2dType _collider_type, Color4f _color){
	Graphics_SetColor4f(_color.r, _color.g, _color.b, _color.a);
	switch(_collider_type){
	case COLLIDER2D_TYPE_POINT:
		Graphics_DrawPoint2f(_t3d.translate.x,_t3d.translate.y,_color,1);
		break;
	case COLLIDER2D_TYPE_RECTANGLE:
		Graphics_DrawRectangle4f(
				_t3d.translate.x
				,_t3d.translate.y
				,_t3d.scale.x
				,_t3d.scale.y
				,_color,1);
		break;
	case COLLIDER2D_TYPE_CIRCLE:
		Graphics_DrawCircle3f(
				_t3d.translate.x
				,_t3d.translate.y
				,_t3d.scale.x
				,_color,1);
		break;
	}
}

bool Collider2d_Test(Transform _t1, Collider2dType _c1, Transform _t2, Collider2dType _c2){
	bool test=false;
	// check collision and set line red if collides
	switch(_c1){
	case COLLIDER2D_TYPE_POINT:
		switch(_c2){
		case COLLIDER2D_TYPE_POINT:
			test=Collider2d_TestIntersectionPointPoint(
					_t1.translate
					,_t2.translate
			);
			break;
		case COLLIDER2D_TYPE_RECTANGLE:
			test=Collider2d_TestIntersectionPointRectangle(
					_t1.translate
					,_t2.translate
					,_t2.scale.x
					,_t2.scale.y
			);
			break;
		case COLLIDER2D_TYPE_CIRCLE:

			test=Collider2d_TestIntersectionPointCircle(
					_t1.translate
					,_t2.translate
					,_t2.scale.x
			);
			break;
		default:
			break;
		}
		break;

	case COLLIDER2D_TYPE_RECTANGLE:
		switch(_c2){
		case COLLIDER2D_TYPE_POINT:
			test=Collider2d_TestIntersectionPointRectangle(
					_t2.translate
					,_t1.translate
					,_t1.scale.x
					,_t1.scale.y
			);
			break;
		case COLLIDER2D_TYPE_RECTANGLE:
			test=Collider2d_TestIntersectionRectangleRectangle(
					_t1.translate
					,_t1.scale.x
					,_t1.scale.y
					,_t2.translate
					,_t2.scale.x
					,_t2.scale.y
			);
			break;
		case COLLIDER2D_TYPE_CIRCLE:

			test=Collider2d_TestIntersectionRectangleCircle(
					_t1.translate
					,_t1.scale.x
					,_t1.scale.y
					,_t2.translate
					,_t2.scale.x
			);
			break;
		default:
			break;
		}
		break;
	case COLLIDER2D_TYPE_CIRCLE:
		switch(_c2){
		case COLLIDER2D_TYPE_POINT:
			test=Collider2d_TestIntersectionPointCircle(
					_t2.translate
					,_t1.translate
					,_t1.scale.x
			);
			break;
		case COLLIDER2D_TYPE_RECTANGLE:
			test=Collider2d_TestIntersectionRectangleCircle(
					_t2.translate
					,_t2.scale.x
					,_t2.scale.y
					,_t1.translate
					,_t1.scale.x

			);
			break;
		case COLLIDER2D_TYPE_CIRCLE:

			test=Collider2d_TestIntersectionCircleCircle(
					_t1.translate
					,_t1.scale.x
					,_t2.translate
					,_t2.scale.x
			);
			break;
		default:
			break;
		}
		break;
	default:
		break;


	}

	return test;
}



