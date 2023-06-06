#ifndef __VECTOR3F_H__
#define __VECTOR3F_H__


typedef struct ZG_Vector3f ZG_Vector3f;
struct ZG_Vector3f{
	float x,y,z;
};

ZG_Vector3f  Vector3f_Ones(void);
ZG_Vector3f  Vector3f_New3f(float x, float y, float z);
ZG_Vector3f  Vector3f_Zeros(void);
ZG_Vector3f  Vector3f_Instance3f(float x, float y, float z);
ZG_Vector3f  Vector3f_Normalize(ZG_Vector3f  v);
ZG_Vector3f  Vector3f_Add(ZG_Vector3f  v1,ZG_Vector3f  v2);
ZG_Vector3f  Vector3f_Sub(ZG_Vector3f  v1,ZG_Vector3f  v2);
ZG_Vector3f  Vector3f_Mul(ZG_Vector3f  v1,ZG_Vector3f  v2);


#endif

