#include "zg_math.h"

ZG_Vector2i Vector2i_Zeros(){
	ZG_Vector2i v={.x=0,.y=0};
	return v;
}


ZG_Vector2i Vector2i_New(int x, int y){
	ZG_Vector2i v={.x=x,.y=y};
	return v;
}

ZG_Vector2i Vector2i_Add(ZG_Vector2i p1, ZG_Vector2i p2){
	return Vector2i_New(
		 p1.x+p2.x
		,p1.y+p2.y
	);
}

ZG_Vector2i Vector2i_Sub(ZG_Vector2i p1, ZG_Vector2i p2){
	return Vector2i_New(
		 p1.x-p2.x
		,p1.y-p2.y
	);
}

ZG_Vector2i Vector2i_Mulv2f(ZG_Vector2i p1, Vector2f p2){
	return Vector2i_New(
		p1.x*p2.x
		,p1.y*p2.y
	);
}

bool	 Vector2i_PointRectCollision(ZG_Vector2i point, ZG_Vector2i ini, ZG_Vector2i end){
	return ((ini.x <= point.x) && (point.x <= end.x) && (ini.y <= point.y) && (point.y <= end.y));
}
