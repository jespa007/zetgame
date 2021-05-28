#ifndef __VECTOR3F_H__
#define __VECTOR3F_H__


typedef struct Vector3f Vector3f;
struct Vector3f{
	float x,y,z;
};

Vector3f  Vector3f_Ones(void);
Vector3f  Vector3f_New3f(float x, float y, float z);
Vector3f  Vector3f_Zeros(void);
Vector3f  Vector3f_Instance3f(float x, float y, float z);
Vector3f  Vector3f_Normalize(Vector3f  v);
Vector3f  Vector3f_Add(Vector3f  v1,Vector3f  v2);
Vector3f  Vector3f_Sub(Vector3f  v1,Vector3f  v2);
Vector3f  Vector3f_Mul(Vector3f  v1,Vector3f  v2);


#endif

