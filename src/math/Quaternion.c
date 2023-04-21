#include "zg_math.h"

Quaternion Quaternion_New(){
	Quaternion q;

	q.x=0;
	q.y=0;
	q.z=0;
	q.w=0;

	return q;
}
Quaternion Quaternion_Identity(void){
	Quaternion q;

	q.w=1;
	q.x=0;
	q.y=0;
	q.z=0;

	return q;
}

float 	Quaternion_Magnitude(Quaternion q){
	return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

Quaternion Quaternion_Inverse(Quaternion q){
	// q inverse is   		    q*
	//                q^-1 = -------
	//                        |q|^2

	Quaternion qc;
	float m=Quaternion_Magnitude(q);
	float inv_m2=1.0f/(m*m);
	qc=Quaternion_Conjugate(q);
	return (Quaternion){
		 .x=qc.x*inv_m2
		,.y=qc.y*inv_m2
		,.z=qc.z*inv_m2
		,.w=qc.w*inv_m2
	};

}

Matrix3f  Quaternion_ToMatrix3f(Quaternion q){

	// Covert a quaternion into a full three-dimensional rotation matrix.
    //
	// Input
	// :param Q: A 4 element array representing the quaternion (q0,q1,q2,q3)
    //
	// Output
	// :return: A 3x3 element matrix representing the full 3D rotation matrix.
	//		 This rotation matrix converts a point in the local reference
	//		 frame to a point in the global reference frame.


	Matrix3f matrix= Matrix3f_Identity();
	float * mat_array = &matrix.e11;

	// Extract the values from Q
	float q0 = q.w;
	float q1 = q.x;
	float q2 = q.y;
	float q3 = q.z;

	// First row of the rotation matrix
	mat_array[M3_E11] = 2 * (q0 * q0 + q1 * q1) - 1;
	mat_array[M3_E12] = 2 * (q1 * q2 - q0 * q3);
	mat_array[M3_E13] = 2 * (q1 * q3 + q0 * q2);

	// Second row of the rotation matrix
	mat_array[M3_E21] = 2 * (q1 * q2 + q0 * q3);
	mat_array[M3_E22] = 2 * (q0 * q0 + q2 * q2) - 1;
	mat_array[M3_E23] = 2 * (q2 * q3 - q0 * q1);

	// Third row of the rotation matrix
	mat_array[M3_E31] = 2 * (q1 * q3 - q0 * q2);
	mat_array[M3_E32] = 2 * (q2 * q3 + q0 * q1);
	mat_array[M3_E33] = 2 * (q0 * q0 + q3 * q3) - 1;

	return matrix;
}

Matrix4f  Quaternion_ToMatrix4f(Quaternion q){
	Matrix3f m3=Quaternion_ToMatrix3f(q);
	return Matrix4f_FromMatrix3f(&m3);
}

Quaternion Quaternion_FromEulerV3f(Vector3f v){
	// X,Y,Z are angles
	//x *= 0.0174532925f; // x*(180/PI) To radians!
	//y *= 0.0174532925f; // y*(180/PI) To radians!
	//z *= 0.0174532925f; // z*(180/PI) To radians!

	//x *= 0.5f;
	//y *= 0.5f;
	//z *= 0.5f;
	Quaternion q;

	// divide by 4 because luts are calculated for 2PI, and the formula calcules angle in radiants but multiplied by 0.5
	unsigned angle_x=(LUTS_DEGREES_2_FIXED(v.x)>>2)&LUTS_SIZE_MASK; // put mask to avoid overflow on negative numbers
	unsigned angle_y=(LUTS_DEGREES_2_FIXED(v.y)>>2)&LUTS_SIZE_MASK; // put mask to avoid overflow on negative numbers
	unsigned angle_z=(LUTS_DEGREES_2_FIXED(v.z)>>2)&LUTS_SIZE_MASK; // put mask to avoid overflow on negative numbers

	float cosx = Luts_Cos[angle_x];
	float sinx = Luts_Sin[angle_x];

	float cosy = Luts_Cos[angle_y];
	float siny = Luts_Sin[angle_y];

	float cosz = Luts_Cos[angle_z];
	float sinz = Luts_Sin[angle_z];

	q.w = cosx * cosy * cosz + sinx * siny * sinz;
	q.x = sinx * cosy * cosz - cosx * siny * sinz;
	q.y = cosx * siny * cosz + sinx * cosy * sinz;
	q.z = cosx * cosy * sinz - sinx * siny * cosz;

	return q;

}

Quaternion Quaternion_FromAngleAxis(float angle, Vector3f axis) {
	Vector3f vn = Vector3f_Normalize(axis);
	Quaternion q;

	float sinAngle = Luts_Sin[(LUTS_DEGREES_2_FIXED(angle)&LUTS_SIZE_MASK)>>2];
	float cosAngle = Luts_Cos[(LUTS_DEGREES_2_FIXED(angle)&LUTS_SIZE_MASK)>>2];
	
	q.w=cosAngle;
	q.x=vn.x * sinAngle;
	q.y=vn.y * sinAngle;
	q.z=vn.z * sinAngle;
	
	return q;
}

Vector3f  Quaternion_TransformV3f(Quaternion q,Vector3f v){
	Quaternion po;

	// po = q * qv * conj(q)
	Quaternion qn=Quaternion_Normalize(q);
	Quaternion qv=(Quaternion){
		.x=v.x
		,.y=v.y
		,.z=v.z
		,.w=0
	};

	po=Quaternion_Mul(Quaternion_Mul(qn,qv),Quaternion_Conjugate(qn));


	return (Vector3f){
		.x=po.x
		,.y=po.y
		,.z=po.z
	};
}

Vector3f  Quaternion_InverseTransformV3f(Quaternion q,Vector3f v){

	Quaternion po;

	// po = q * qv * conj(q)
	Quaternion qn=Quaternion_Normalize(Quaternion_Inverse(q));
	Quaternion qv=(Quaternion){
		.x=v.x
		,.y=v.y
		,.z=v.z
		,.w=0
	};

	po=Quaternion_Mul(Quaternion_Mul(qn,qv),Quaternion_Conjugate(qn));


	return (Vector3f){
		.x=po.x
		,.y=po.y
		,.z=po.z
	};
}

/*
Vector2f  Quaternion_TransformV2f(Quaternion q,float x, float y){
	Vector2f v;
	return v;
}*/

Quaternion Quaternion_Normalize(Quaternion q) {
	float mag = 1.0f/Quaternion_Magnitude(q);
	q.w*=mag;
	q.x*=mag;
	q.y*=mag;
	q.z*=mag;
		
	return q;
}

Quaternion Quaternion_Conjugate(Quaternion q) {
	// only change sign imaginary components
	q.x*=-1;
	q.y*=-1;
	q.z*=-1;
	return q;
}

Quaternion Quaternion_Mul(Quaternion q1, Quaternion q2){
	Quaternion qr;

	qr.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	qr.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	qr.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	qr.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	return qr;
}

Quaternion Quaternion_Add(Quaternion q1, Quaternion q2){
	q1.w += q2.w;
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;

	return q1;
}

Quaternion Quaternion_Scale(Quaternion q, float s){

	q.w *=s;
	q.x *=s;
	q.y *=s;
	q.z *=s;

	return q;
}

float Quaternion_Dot(Quaternion q1, Quaternion q2){
	return (q1.w * q2.w + q1.x * q2.x * q1.y * q2.y + q1.z * q2.z);
}

Quaternion Quaternion_Lerp(Quaternion from, Quaternion to, float t) {
	Quaternion src = Quaternion_Scale(from, (1.0f - t));
	Quaternion dst = Quaternion_Scale(to, t);

	Quaternion q = Quaternion_Add(src,dst);
	return q;
}

Quaternion Quaternion_Slerp(Quaternion from, Quaternion to, float t) {
	float cosTheta = Quaternion_Dot(from, to);
	Quaternion temp=to;

	if (cosTheta < 0.0f) {
		cosTheta *= -1.0f;
		temp = Quaternion_Scale(temp,-1.0f);
	}

	float theta = acosf(cosTheta);
	float sinTheta = 1.0f / sinf(theta);

	return Quaternion_Scale(
			Quaternion_Add(
						 Quaternion_Scale(from,sinf(theta * (1.0f - t)))
						,Quaternion_Scale(temp,sinf(t * theta))
			)
		,sinTheta);
}

/*
#define m00 right.x
#define m01 up.x
#define m02 forward.x
#define m10 right.y
#define m11 up.y
#define m12 forward.y
#define m20 right.z
#define m21 up.z
#define m22 forward.z

Quaternion Quaternion_LookRotation(Vector3f lookAt, Vector3f upDirection) {
	Vector3f forward = lookAt; Vector3f up = upDirection;
	Vector3f_OrthoNormalize(forward, up);
	Vector3f right = Vector_Cross(up, forward);

	Quaternion ret;
	ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * ret.w);
	ret.x = (m21 - m12) * w4_recip;
	ret.y = (m02 - m20) * w4_recip;
	ret.z = (m10 - m01) * w4_recip;

	return ret;
}

Quaternion Quaternion_LookRotation(Vector3f lookAt) {
	Vector3f up = Vector3f_up;
	Vector3f forward = lookAt;
	Vector3f_OrthoNormalize(forward, up);
	Vector3f right = Vector3f_Cross(up, forward);

	Quaternion ret;
	ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * ret.w);
	ret.x = (m21 - m12) * w4_recip;
	ret.y = (m02 - m20) * w4_recip;
	ret.z = (m10 - m01) * w4_recip;

	return ret;
}

void Quaternion_SetLookRotation(Vector3f lookAt) {
	Vector3f up = (Vector)Vector_up;
	Vector3f forward = lookAt;
	Vector3f_OrthoNormalize(forward, up);
	Vector3f right = Vector3f_Cross(up, forward);

	w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * w);
	x = (m21 - m12) * w4_recip;
	y = (m02 - m20) * w4_recip;
	z = (m10 - m01) * w4_recip;
}

void Quaternion_SetLookRotation(Vector3f lookAt, Vector3f upDirection) {
	Vector3f forward = lookAt; Vector3f up = upDirection;
	Vector3f_OrthoNormalize(&forward, &up);
	Vector3f right = Vector3f_Cross(up, forward);

	w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * w);
	x = (m21 - m12) * w4_recip;
	y = (m02 - m20) * w4_recip;
	z = (m10 - m01) * w4_recip;
}
*/

