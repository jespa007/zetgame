#ifndef __MATRIX4F_H__
#define __MATRIX4F_H__

#define M4_E11	0
#define M4_E12	1
#define M4_E13	2
#define M4_E14	3
#define M4_E21	4
#define M4_E22	5
#define M4_E23	6
#define M4_E24	7
#define M4_E31	8
#define M4_E32	9
#define M4_E33	10
#define M4_E34	11
#define M4_E41	12
#define M4_E42	13
#define M4_E43	14
#define M4_E44	15

typedef struct ZG_Matrix4f ZG_Matrix4f;

struct ZG_Matrix4f{
	float  e11,e12,e13,e14
	      ,e21,e22,e23,e24
		  ,e31,e32,e33,e34
		  ,e41,e42,e43,e44;
};

ZG_Matrix4f Matrix4f_New(void);
ZG_Matrix4f Matrix4f_FromMatrix3f(const Matrix3f *_m3);
ZG_Matrix4f Matrix4f_Identity(void);
ZG_Vector3f Matrix4f_Transformv3f(const ZG_Matrix4f *_this, ZG_Vector3f v);
ZG_Matrix4f Matrix4f_Inverse(const ZG_Matrix4f *_this);

#endif
