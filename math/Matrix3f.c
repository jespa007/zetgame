#include "math.h"

Matrix3f Matrix3f_New(void){
	return Matrix3f_Identity();
}

Matrix3f Matrix3f_Identity(void){

	Matrix3f m;
	float *m_ptr=&m.e11;

	m_ptr[M3_E11]=1.0f;
	m_ptr[M3_E12]=0.0f;
	m_ptr[M3_E13]=0.0f;

	m_ptr[M3_E21]=0.0f;
	m_ptr[M3_E22]=1.0f;
	m_ptr[M3_E23]=0.0f;

	m_ptr[M3_E31]=0.0f;
	m_ptr[M3_E32]=0.0f;
	m_ptr[M3_E33]=1.0f;


	return m;

}

Matrix3f Matrix3f_Mul3x3(const Matrix3f * _m1, const Matrix3f * _m2){
	Matrix3f m=Matrix3f_New();

	const float * m1=&_m1->e11;
	const float * m2=&_m2->e11;

	m.e11 =   m1[M3_E11] * m2[M3_E11]
			+ m1[M3_E12] * m2[M3_E21]
			+ m1[M3_E13] * m2[M3_E31];

	m.e12 =   m1[M3_E11] * m2[M3_E12]
			+ m1[M3_E12] * m2[M3_E22]
			+ m1[M3_E13] * m2[M3_E32];

	m.e13 =   m1[M3_E11] * m2[M3_E13]
			+ m1[M3_E12] * m2[M3_E23]
			+ m1[M3_E13] * m2[M3_E33];

	m.e21 =   m1[M3_E21] * m2[M3_E11]
			+ m1[M3_E22] * m2[M3_E21]
			+ m1[M3_E23] * m2[M3_E31];

	m.e22 =   m1[M3_E21] * m2[M3_E12]
			+ m1[M3_E22] * m2[M3_E22]
			+ m1[M3_E23] * m2[M3_E32];

	m.e23 =   m1[M3_E21] * m2[M3_E13]
			+ m1[M3_E22] * m2[M3_E23]
			+ m1[M3_E23] * m2[M3_E33];

	m.e31 =   m1[M3_E31] * m2[M3_E11]
			+ m1[M3_E32] * m2[M3_E21]
			+ m1[M3_E33] * m2[M3_E31];

	m.e32 =   m1[M3_E31] * m2[M3_E12]
			+ m1[M3_E32] * m2[M3_E22]
			+ m1[M3_E33] * m2[M3_E32];

	m.e33 =   m1[M3_E31] * m2[M3_E13]
			+ m1[M3_E32] * m2[M3_E23]
			+ m1[M3_E33] * m2[M3_E33];

	return m;
}

Vector3f Matrix3f_TransformV3f(const Matrix3f *_this, Vector3f v){
	Vector3f transform;
	float *m=(float *)&_this->e11;

	transform.x=v.x*m[M3_E11]+v.y*m[M3_E12]+v.z*m[M3_E13];
	transform.y=v.x*m[M3_E21]+v.y*m[M3_E22]+v.z*m[M3_E23];
	transform.z=v.x*m[M3_E31]+v.y*m[M3_E32]+v.z*m[M3_E33];

	return transform;
}
