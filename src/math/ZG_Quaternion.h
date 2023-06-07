#ifndef __ZG_QUATERNION_H__
#define __ZG_QUATERNION_H__

typedef struct ZG_Quaternion ZG_Quaternion;

struct ZG_Quaternion{
	float w,x,y,z;
};

ZG_Quaternion 	ZG_Quaternion_New(void);
ZG_Quaternion 	ZG_Quaternion_Identity(void);
float 			ZG_Quaternion_Magnitude(ZG_Quaternion q);
ZG_Quaternion 	ZG_Quaternion_Inverse(ZG_Quaternion q);
ZG_Matrix4f  	ZG_Quaternion_ToMatrix4f(ZG_Quaternion q);
ZG_Matrix3f  	ZG_Quaternion_ToMatrix3f(ZG_Quaternion q);
ZG_Quaternion 	ZG_Quaternion_FromEulerV3f(ZG_Vector3f euler);
ZG_Vector3f  	ZG_Quaternion_TransformV3f(ZG_Quaternion q,ZG_Vector3f v);
ZG_Vector3f  	ZG_Quaternion_InverseTransformV3f(ZG_Quaternion q,ZG_Vector3f v);
ZG_Quaternion 	ZG_Quaternion_Normalize(ZG_Quaternion q);
ZG_Quaternion 	ZG_Quaternion_Conjugate(ZG_Quaternion q);
float 			ZG_Quaternion_Dot(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion 	ZG_Quaternion_Lerp(ZG_Quaternion from, ZG_Quaternion to, float t);
ZG_Quaternion 	ZG_Quaternion_Slerp(ZG_Quaternion from, ZG_Quaternion to, float t);
ZG_Quaternion 	ZG_Quaternion_Mul(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion	ZG_Quaternion_Add(ZG_Quaternion q1, ZG_Quaternion q2);
ZG_Quaternion 	ZG_Quaternion_Scale(ZG_Quaternion q, float s);

#endif
