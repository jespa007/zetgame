#ifndef __VECTOR2F_H__
#define __VECTOR2F_H__

typedef struct Vector2f Vector2f;

struct Vector2f{
	float x,y;
};


Vector2f 	Vector2f_New(float x, float y);
Vector2f 	Vector2f_Zeros(void);
float 		Vector2f_Distance(Vector2f *p1, Vector2f *p2);
void  		Vector2f_LeastSquaresFitLinear(ZG_List *lst_points2d,float * gain, float *offset);
#endif
