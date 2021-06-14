#include "zg_math.h"

Vector2i Vector2i_Zeros(){
	Vector2i v={.x=0,.y=0};
	return v;
}


Vector2i Vector2i_New(int x, int y){
	Vector2i v={.x=x,.y=y};
	return v;
}

Vector2i Vector2i_Add(Vector2i p1, Vector2i p2){
	return Vector2i_New(
		 p1.x+p2.x
		,p1.y+p2.y
	);
}

Vector2i Vector2i_Sub(Vector2i p1, Vector2i p2){
	return Vector2i_New(
		 p1.x-p2.x
		,p1.y-p2.y
	);
}

Vector2i Vector2i_Mulv2f(Vector2i p1, Vector2f p2){
	return Vector2i_New(
		p1.x*p2.x
		,p1.y*p2.y
	);
}

bool	 Vector2i_PointRectCollision(Vector2i point, Vector2i ini, Vector2i end){
	return ((ini.x <= point.x) && (point.x <= end.x) && (ini.y <= point.y) && (point.y <= end.y));
}
