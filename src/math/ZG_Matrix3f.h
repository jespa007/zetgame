#ifndef __MATRIX3F_H__
#define __MATRIX3F_H__

#define M3_E11	0
#define M3_E12	1
#define M3_E13	2
#define M3_E21	3
#define M3_E22	4
#define M3_E23	5
#define M3_E31	6
#define M3_E32	7
#define M3_E33	8


typedef struct Matrix3f Matrix3f;

struct Matrix3f{
	float  e11,e12,e13
		  ,e21,e22,e23
		  ,e31,e32,e33;
};

Matrix3f Matrix3f_New(void);
Matrix3f Matrix3f_Identity(void);
ZG_Vector3f Matrix3f_TransformV3f(const Matrix3f *_this, ZG_Vector3f v);
Matrix3f Matrix3f_Mul(const Matrix3f * m1, const Matrix3f * m2);

#endif
