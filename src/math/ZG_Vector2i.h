#ifndef __VECTOR2I_H__
#define __VECTOR2I_H__


//typedef struct Vector2f Vector2f;
typedef struct ZG_Vector2i ZG_Vector2i;
struct ZG_Vector2i{
	int x,y;
};

ZG_Vector2i Vector2i_Zeros(void);
ZG_Vector2i Vector2i_New(int x, int y);
ZG_Vector2i Vector2i_Add(ZG_Vector2i p1, ZG_Vector2i p2);
ZG_Vector2i Vector2i_Sub(ZG_Vector2i p1, ZG_Vector2i p2);
ZG_Vector2i Vector2i_Mulv2f(ZG_Vector2i p1, Vector2f p2);
bool	 Vector2i_PointRectCollision(ZG_Vector2i point, ZG_Vector2i ini, ZG_Vector2i end);

#endif
