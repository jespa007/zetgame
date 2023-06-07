#ifndef __ZG_VECTOR2F_H__
#define __ZG_VECTOR2F_H__

typedef struct ZG_Vector2f ZG_Vector2f;

struct ZG_Vector2f{
	float x,y;
};


ZG_Vector2f 	ZG_Vector2f_New(float x, float y);
ZG_Vector2f 	ZG_Vector2f_Zeros(void);
float 			ZG_Vector2f_Distance(ZG_Vector2f *p1, ZG_Vector2f *p2);
void  			ZG_Vector2f_LeastSquaresFitLinear(ZG_List *lst_points2d,float * gain, float *offset);
#endif
