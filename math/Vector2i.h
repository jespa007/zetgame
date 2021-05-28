#ifndef __VECTOR2I_H__
#define __VECTOR2I_H__


typedef struct Vector2f Vector2f;
typedef struct Vector2i Vector2i;
struct Vector2i{
	int x,y;
};

Vector2i Vector2i_Zeros(void);
Vector2i Vector2i_New2i(int x, int y);
Vector2i Vector2i_Add(Vector2i p1, Vector2i p2);
Vector2i Vector2i_Sub(Vector2i p1, Vector2i p2);
Vector2i Vector2i_Mulv2f(Vector2i p1, Vector2f p2);
bool	 Vector2i_PointRectCollision(Vector2i point, Vector2i ini, Vector2i end);

#endif
