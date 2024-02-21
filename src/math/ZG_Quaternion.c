#include "@zg_math.h"

ZG_Quaternion ZG_Quaternion_New(){
	ZG_Quaternion q;

	q.x=0;
	q.y=0;
	q.z=0;
	q.w=0;

	return q;
}
ZG_Quaternion ZG_Quaternion_Identity(void){
	ZG_Quaternion q;

	q.w=1;
	q.x=0;
	q.y=0;
	q.z=0;

	return q;
}

float 	ZG_Quaternion_Magnitude(ZG_Quaternion q){
	return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

ZG_Quaternion ZG_Quaternion_Inverse(ZG_Quaternion q){
	// q inverse is   		    q*
	//                q^-1 = -------
	//                        |q|^2

	ZG_Quaternion qc;
	float m=ZG_Quaternion_Magnitude(q);
	float inv_m2=1.0f/(m*m);
	qc=ZG_Quaternion_Conjugate(q);
	return (ZG_Quaternion){
		 .x=qc.x*inv_m2
		,.y=qc.y*inv_m2
		,.z=qc.z*inv_m2
		,.w=qc.w*inv_m2
	};

}

ZG_Matrix3f  ZG_Quaternion_ToMatrix3f(ZG_Quaternion q){

	// Covert a quaternion into a full three-dimensional rotation matrix.
    //
	// Input
	// :param Q: A 4 element array representing the quaternion (q0,q1,q2,q3)
    //
	// Output
	// :return: A 3x3 element matrix representing the full 3D rotation matrix.
	//		 This rotation matrix converts a point in the local reference
	//		 frame to a point in the global reference frame.


	ZG_Matrix3f matrix= ZG_Matrix3f_Identity();
	float * mat_array = &matrix.e11;

	// Extract the values from Q
	float q0 = q.w;
	float q1 = q.x;
	float q2 = q.y;
	float q3 = q.z;

	// First row of the rotation matrix
	mat_array[ZG_M3_E11] = 2 * (q0 * q0 + q1 * q1) - 1;
	mat_array[ZG_M3_E12] = 2 * (q1 * q2 - q0 * q3);
	mat_array[ZG_M3_E13] = 2 * (q1 * q3 + q0 * q2);

	// Second row of the rotation matrix
	mat_array[ZG_M3_E21] = 2 * (q1 * q2 + q0 * q3);
	mat_array[ZG_M3_E22] = 2 * (q0 * q0 + q2 * q2) - 1;
	mat_array[ZG_M3_E23] = 2 * (q2 * q3 - q0 * q1);

	// Third row of the rotation matrix
	mat_array[ZG_M3_E31] = 2 * (q1 * q3 - q0 * q2);
	mat_array[ZG_M3_E32] = 2 * (q2 * q3 + q0 * q1);
	mat_array[ZG_M3_E33] = 2 * (q0 * q0 + q3 * q3) - 1;

	return matrix;
}

ZG_Matrix4f  ZG_Quaternion_ToMatrix4f(ZG_Quaternion q){
	ZG_Matrix3f m3=ZG_Quaternion_ToMatrix3f(q);
	return Matrix4f_FromMatrix3f(&m3);
}

ZG_Quaternion ZG_Quaternion_FromEulerV3f(ZG_Vector3f v){
	// X,Y,Z are angles
	//x *= 0.0174532925f; // x*(180/PI) To radians!
	//y *= 0.0174532925f; // y*(180/PI) To radians!
	//z *= 0.0174532925f; // z*(180/PI) To radians!

	//x *= 0.5f;
	//y *= 0.5f;
	//z *= 0.5f;
	ZG_Quaternion q;

	// divide by 1 because quaternions works between -PI -- 0 -- PI or -180 -- 0 -- 180º
	unsigned angle_x=(ZG_LUTS_DEGREES_2_FIXED(v.x)>>1)&(ZG_LUTS_SIZE_MASK>>1); // put mask to avoid overflow on negative numbers
	unsigned angle_y=(ZG_LUTS_DEGREES_2_FIXED(v.y)>>1)&(ZG_LUTS_SIZE_MASK>>1); // put mask to avoid overflow on negative numbers
	unsigned angle_z=(ZG_LUTS_DEGREES_2_FIXED(v.z)>>1)&(ZG_LUTS_SIZE_MASK>>1); // put mask to avoid overflow on negative numbers

	float cosx = ZG_Luts_Cos[angle_x];
	float sinx = ZG_Luts_Sin[angle_x];

	float cosy = ZG_Luts_Cos[angle_y];
	float siny = ZG_Luts_Sin[angle_y];

	float cosz = ZG_Luts_Cos[angle_z];
	float sinz = ZG_Luts_Sin[angle_z];

	/*float angle_xr=(v.x*PI)/180.0f;
	float angle_yr=(v.y*PI)/180.0f;
	float angle_zr=(v.z*PI)/180.0f;

	float cosx = cos(angle_xr*0.5);
	float sinx = sin(angle_xr*0.5);

	float cosy = cos(angle_yr*0.5);
	float siny = sin(angle_yr*0.5);

	float cosz = cos(angle_zr*0.5);
	float sinz = sin(angle_zr*0.5);*/


	q.w = cosx * cosy * cosz + sinx * siny * sinz;
	q.x = sinx * cosy * cosz + cosx * siny * sinz;
	q.y = cosx * siny * cosz - sinx * cosy * sinz;
	q.z = cosx * cosy * sinz - sinx * siny * cosz;

	return q;

}

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
ZG_Vector3f ZG_Quaternion_ToEulerAngles(ZG_Quaternion _quaternion) {
    ZG_Vector3f angles;

    // roll (x-axis rotation)
    /*double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.pitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = std::atan2(siny_cosp, cosy_cosp);*/

    return angles;
}

ZG_Quaternion Quaternion_FromAngleAxis(float angle, ZG_Vector3f axis) {
	ZG_Vector3f vn = ZG_Vector3f_Normalize(axis);
	ZG_Quaternion q;

	float sinAngle = ZG_Luts_Sin[(ZG_LUTS_DEGREES_2_FIXED(angle)&ZG_LUTS_SIZE_MASK)>>1];
	float cosAngle = ZG_Luts_Cos[(ZG_LUTS_DEGREES_2_FIXED(angle)&ZG_LUTS_SIZE_MASK)>>1];
	
	q.w=cosAngle;
	q.x=vn.x * sinAngle;
	q.y=vn.y * sinAngle;
	q.z=vn.z * sinAngle;
	
	return q;
}

ZG_Vector3f  ZG_Quaternion_TransformV3f(ZG_Quaternion q,ZG_Vector3f v){
	ZG_Quaternion po;

	// po = q * qv * conj(q)
	ZG_Quaternion qn=ZG_Quaternion_Normalize(q);
	ZG_Quaternion qv=(ZG_Quaternion){
		.x=v.x
		,.y=v.y
		,.z=v.z
		,.w=0
	};

	po=ZG_Quaternion_Mul(ZG_Quaternion_Mul(qn,qv),ZG_Quaternion_Conjugate(qn));


	return (ZG_Vector3f){
		.x=po.x
		,.y=po.y
		,.z=po.z
	};
}

ZG_Vector3f  ZG_Quaternion_InverseTransformV3f(ZG_Quaternion q,ZG_Vector3f v){

	ZG_Quaternion po;

	// po = q * qv * conj(q)
	ZG_Quaternion qn=ZG_Quaternion_Normalize(ZG_Quaternion_Inverse(q));
	ZG_Quaternion qv=(ZG_Quaternion){
		.x=v.x
		,.y=v.y
		,.z=v.z
		,.w=0
	};

	po=ZG_Quaternion_Mul(ZG_Quaternion_Mul(qn,qv),ZG_Quaternion_Conjugate(qn));


	return (ZG_Vector3f){
		.x=po.x
		,.y=po.y
		,.z=po.z
	};
}

/*
ZG_Vector2f  Quaternion_TransformV2f(ZG_Quaternion q,float x, float y){
	ZG_Vector2f v;
	return v;
}*/

ZG_Quaternion ZG_Quaternion_Normalize(ZG_Quaternion q) {
	float mag = 1.0f/ZG_Quaternion_Magnitude(q);
	q.w*=mag;
	q.x*=mag;
	q.y*=mag;
	q.z*=mag;
		
	return q;
}

ZG_Quaternion ZG_Quaternion_Conjugate(ZG_Quaternion q) {
	// only change sign imaginary components
	q.x*=-1;
	q.y*=-1;
	q.z*=-1;
	return q;
}

ZG_Quaternion ZG_Quaternion_Mul(ZG_Quaternion q1, ZG_Quaternion q2){
	ZG_Quaternion qr;

	qr.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
	qr.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
	qr.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
	qr.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;

	return qr;
}

ZG_Quaternion ZG_Quaternion_Add(ZG_Quaternion q1, ZG_Quaternion q2){
	q1.w += q2.w;
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;

	return q1;
}

ZG_Quaternion ZG_Quaternion_Scale(ZG_Quaternion q, float s){

	q.w *=s;
	q.x *=s;
	q.y *=s;
	q.z *=s;

	return q;
}

float ZG_Quaternion_Dot(ZG_Quaternion q1, ZG_Quaternion q2){
	return (q1.w * q2.w + q1.x * q2.x * q1.y * q2.y + q1.z * q2.z);
}

ZG_Quaternion ZG_Quaternion_Lerp(ZG_Quaternion from, ZG_Quaternion to, float t) {
	ZG_Quaternion src = ZG_Quaternion_Scale(from, (1.0f - t));
	ZG_Quaternion dst = ZG_Quaternion_Scale(to, t);

	ZG_Quaternion q = ZG_Quaternion_Add(src,dst);
	return q;
}

ZG_Quaternion ZG_Quaternion_Slerp(ZG_Quaternion from, ZG_Quaternion to, float t) {
	float cosTheta = ZG_Quaternion_Dot(from, to);
	ZG_Quaternion temp=to;

	if (cosTheta < 0.0f) {
		cosTheta *= -1.0f;
		temp = ZG_Quaternion_Scale(temp,-1.0f);
	}

	float theta = acosf(cosTheta);
	float sinTheta = 1.0f / sinf(theta);

	return ZG_Quaternion_Scale(
			ZG_Quaternion_Add(
						 ZG_Quaternion_Scale(from,sinf(theta * (1.0f - t)))
						,ZG_Quaternion_Scale(temp,sinf(t * theta))
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

ZG_Quaternion Quaternion_LookRotation(ZG_Vector3f lookAt, ZG_Vector3f upDirection) {
	ZG_Vector3f forward = lookAt; ZG_Vector3f up = upDirection;
	ZG_Vector3f_OrthoNormalize(forward, up);
	ZG_Vector3f right = Vector_Cross(up, forward);

	ZG_Quaternion ret;
	ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * ret.w);
	ret.x = (m21 - m12) * w4_recip;
	ret.y = (m02 - m20) * w4_recip;
	ret.z = (m10 - m01) * w4_recip;

	return ret;
}

ZG_Quaternion Quaternion_LookRotation(ZG_Vector3f lookAt) {
	ZG_Vector3f up = ZG_Vector3f_up;
	ZG_Vector3f forward = lookAt;
	ZG_Vector3f_OrthoNormalize(forward, up);
	ZG_Vector3f right = ZG_Vector3f_Cross(up, forward);

	ZG_Quaternion ret;
	ret.w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * ret.w);
	ret.x = (m21 - m12) * w4_recip;
	ret.y = (m02 - m20) * w4_recip;
	ret.z = (m10 - m01) * w4_recip;

	return ret;
}

void Quaternion_SetLookRotation(ZG_Vector3f lookAt) {
	ZG_Vector3f up = (Vector)Vector_up;
	ZG_Vector3f forward = lookAt;
	ZG_Vector3f_OrthoNormalize(forward, up);
	ZG_Vector3f right = ZG_Vector3f_Cross(up, forward);

	w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * w);
	x = (m21 - m12) * w4_recip;
	y = (m02 - m20) * w4_recip;
	z = (m10 - m01) * w4_recip;
}

void Quaternion_SetLookRotation(ZG_Vector3f lookAt, ZG_Vector3f upDirection) {
	ZG_Vector3f forward = lookAt; ZG_Vector3f up = upDirection;
	ZG_Vector3f_OrthoNormalize(&forward, &up);
	ZG_Vector3f right = ZG_Vector3f_Cross(up, forward);

	w = sqrtf(1.0f + m00 + m11 + m22) * 0.5f;
	float w4_recip = 1.0f / (4.0f * w);
	x = (m21 - m12) * w4_recip;
	y = (m02 - m20) * w4_recip;
	z = (m10 - m01) * w4_recip;
}
*/

