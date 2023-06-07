#ifndef __ZG_MATRIX3F_H__
#define __ZG_MATRIX3F_H__

#define ZG_M3_E11	0
#define ZG_M3_E12	1
#define ZG_M3_E13	2
#define ZG_M3_E21	3
#define ZG_M3_E22	4
#define ZG_M3_E23	5
#define ZG_M3_E31	6
#define ZG_M3_E32	7
#define ZG_M3_E33	8


typedef struct ZG_Matrix3f ZG_Matrix3f;

struct ZG_Matrix3f{
	float  e11,e12,e13
		  ,e21,e22,e23
		  ,e31,e32,e33;
};

ZG_Matrix3f ZG_Matrix3f_New(void);
ZG_Matrix3f ZG_Matrix3f_Identity(void);
ZG_Vector3f ZG_Matrix3f_TransformV3f(const ZG_Matrix3f *_this, ZG_Vector3f v);
ZG_Matrix3f ZG_Matrix3f_Mul(const ZG_Matrix3f * m1, const ZG_Matrix3f * m2);

#endif
