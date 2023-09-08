#include "physics/@zg_physics.h"

typedef struct{
	ZG_Vector2f dim;
	ZG_Collider2dType type;
}ZG_Collider2dData;


bool ZG_Collider2d_TestIntersectionPointPoint(
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
							){
	return _p1.x==_p2.x && _p1.y == _p2.y;
}


bool ZG_Collider2d_TestIntersectionPointRectangle(
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
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

bool ZG_Collider2d_TestIntersectionPointCircle(
							  ZG_Vector3f _p1
							, ZG_Vector3f _p2
							, float _r2
							){
	float xdiff=fabs(_p1.x-_p2.x);
	float ydiff=fabs(_p1.y-_p2.y);

	float distance=(xdiff)*(xdiff)+
					  (ydiff)*(ydiff);

	return distance < _r2*_r2;
}

bool ZG_Collider2d_TestIntersectionRectangleRectangle(
	  ZG_Vector3f _p1
		, float _w1, float _h1
		, ZG_Vector3f _p2
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

bool ZG_Collider2d_TestIntersectionRectangleCircle(
	 ZG_Vector3f _p1
	, float _w1
	, float _h1
	, ZG_Vector3f _p2
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

bool ZG_Collider2d_TestIntersectionCircleCircle(
	ZG_Vector3f _p1
	, float _r1
	, ZG_Vector3f _p2
	, float _r2){

	float xdiff=fabs(_p1.x-_p2.x);
	float ydiff=fabs(_p1.y-_p2.y);
	float rad_sum_sq = (_r1 + _r2) * (_r1 + _r2);

	float distance=(xdiff)*(xdiff)+
					  (ydiff)*(ydiff);

	return distance <= rad_sum_sq;

}

void ZG_Collider2d_Draw(ZG_Transform _t3d, ZG_Collider2dType _collider_type, ZG_Color4f _color){
	ZG_Graphics_SetColor4f(_color.r, _color.g, _color.b, _color.a);
	switch(_collider_type){
	case ZG_COLLIDER2D_TYPE_POINT:
		ZG_Graphics_DrawPoint2f(_t3d.translate.x,_t3d.translate.y,_color,1);
		break;
	case ZG_COLLIDER2D_TYPE_RECTANGLE:
		ZG_Graphics_DrawRectangle4f(
				_t3d.translate.x
				,_t3d.translate.y
				,_t3d.scale.x
				,_t3d.scale.y
				,_color,1);
		break;
	case ZG_COLLIDER2D_TYPE_CIRCLE:
		ZG_Graphics_DrawCircle3f(
				_t3d.translate.x
				,_t3d.translate.y
				,_t3d.scale.x // diameter
				,_color,1);
		break;
	}
}

bool ZG_Collider2d_Test(ZG_Transform _t1, ZG_Collider2dType _c1, ZG_Transform _t2, ZG_Collider2dType _c2){
	bool test=false;
	// check collision and set line red if collides
	switch(_c1){
	case ZG_COLLIDER2D_TYPE_POINT:
		switch(_c2){
		case ZG_COLLIDER2D_TYPE_POINT:
			test=ZG_Collider2d_TestIntersectionPointPoint(
					_t1.translate
					,_t2.translate
			);
			break;
		case ZG_COLLIDER2D_TYPE_RECTANGLE:
			test=ZG_Collider2d_TestIntersectionPointRectangle(
					_t1.translate
					,_t2.translate
					,_t2.scale.x
					,_t2.scale.y
			);
			break;
		case ZG_COLLIDER2D_TYPE_CIRCLE:

			test=ZG_Collider2d_TestIntersectionPointCircle(
					_t1.translate
					,_t2.translate
					,_t2.scale.x
			);
			break;
		default:
			break;
		}
		break;

	case ZG_COLLIDER2D_TYPE_RECTANGLE:
		switch(_c2){
		case ZG_COLLIDER2D_TYPE_POINT:
			test=ZG_Collider2d_TestIntersectionPointRectangle(
					_t2.translate
					,_t1.translate
					,_t1.scale.x
					,_t1.scale.y
			);
			break;
		case ZG_COLLIDER2D_TYPE_RECTANGLE:
			test=ZG_Collider2d_TestIntersectionRectangleRectangle(
					_t1.translate
					,_t1.scale.x
					,_t1.scale.y
					,_t2.translate
					,_t2.scale.x
					,_t2.scale.y
			);
			break;
		case ZG_COLLIDER2D_TYPE_CIRCLE:

			test=ZG_Collider2d_TestIntersectionRectangleCircle(
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
	case ZG_COLLIDER2D_TYPE_CIRCLE:
		switch(_c2){
		case ZG_COLLIDER2D_TYPE_POINT:
			test=ZG_Collider2d_TestIntersectionPointCircle(
					_t2.translate
					,_t1.translate
					,_t1.scale.x
			);
			break;
		case ZG_COLLIDER2D_TYPE_RECTANGLE:
			test=ZG_Collider2d_TestIntersectionRectangleCircle(
					_t2.translate
					,_t2.scale.x
					,_t2.scale.y
					,_t1.translate
					,_t1.scale.x

			);
			break;
		case ZG_COLLIDER2D_TYPE_CIRCLE:
			test=ZG_Collider2d_TestIntersectionCircleCircle(
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



