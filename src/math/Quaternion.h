#ifndef __QUATERNION_H__
#define __QUATERNION_H__

// Constraints: It works animation for 180º

typedef struct Quaternion Quaternion;

struct Quaternion{
	float w,x,y,z;
};

Quaternion 	Quaternion_New(void);
Quaternion 	Quaternion_Identity(void);
float 		Quaternion_Magnitude(Quaternion q);
Quaternion 	Quaternion_Inverse(Quaternion q);
Matrix4f  	Quaternion_ToMatrix4f(Quaternion q);
Matrix3f  	Quaternion_ToMatrix3f(Quaternion q);
Quaternion 	Quaternion_FromEulerV3f(Vector3f euler);
Vector3f  	Quaternion_TransformV3f(Quaternion q,Vector3f v);
Vector3f  	Quaternion_InverseTransformV3f(Quaternion q,Vector3f v);
Quaternion 	Quaternion_Normalize(Quaternion q);
Quaternion 	Quaternion_Conjugate(Quaternion q);
float 		Quaternion_Dot(Quaternion q1, Quaternion q2);
Quaternion 	Quaternion_Lerp(Quaternion from, Quaternion to, float t);
Quaternion 	Quaternion_Slerp(Quaternion from, Quaternion to, float t);
Quaternion 	Quaternion_Mul(Quaternion q1, Quaternion q2);
Quaternion	Quaternion_Add(Quaternion q1, Quaternion q2);
Quaternion 	Quaternion_Scale(Quaternion q, float s);

#endif
