#ifndef __QUATERNION_H__
#define __QUATERNION_H__

typedef struct ZG_Quaternion ZG_Quaternion;

struct ZG_Quaternion{
	float w,x,y,z;
};

ZG_Quaternion 	Quaternion_New(void);
ZG_Quaternion 	Quaternion_Identity(void);
float 		Quaternion_Magnitude(ZG_Quaternion q);
ZG_Quaternion 	Quaternion_Inverse(ZG_Quaternion q);
ZG_Matrix4f  	ZG_Quaternion_ToMatrix4f(ZG_Quaternion q);
Matrix3f  	Quaternion_ToMatrix3f(ZG_Quaternion q);
ZG_Quaternion 	ZG_Quaternion_FromEulerV3f(ZG_Vector3f euler);
ZG_Vector3f  	Quaternion_TransformV3f(ZG_Quaternion q,ZG_Vector3f v);
ZG_Vector3f  	Quaternion_InverseTransformV3f(ZG_Quaternion q,ZG_Vector3f v);
ZG_Quaternion 	Quaternion_Normalize(ZG_Quaternion q);
ZG_Quaternion 	Quaternion_Conjugate(ZG_Quaternion q);
float 		Quaternion_Dot(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion 	Quaternion_Lerp(ZG_Quaternion from, ZG_Quaternion to, float t);
ZG_Quaternion 	Quaternion_Slerp(ZG_Quaternion from, ZG_Quaternion to, float t);
ZG_Quaternion 	Quaternion_Mul(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion	Quaternion_Add(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion 	Quaternion_Scale(ZG_Quaternion q, float s);

#endif
