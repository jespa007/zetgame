#include "math.h"

ZG_Matrix3f ZG_Matrix3f_New(void){
	return ZG_Matrix3f_Identity();
}

ZG_Matrix3f ZG_Matrix3f_Identity(void){

	ZG_Matrix3f m;
	float *m_ptr=&m.e11;

	m_ptr[ZG_M3_E11]=1.0f;
	m_ptr[ZG_M3_E12]=0.0f;
	m_ptr[ZG_M3_E13]=0.0f;

	m_ptr[ZG_M3_E21]=0.0f;
	m_ptr[ZG_M3_E22]=1.0f;
	m_ptr[ZG_M3_E23]=0.0f;

	m_ptr[ZG_M3_E31]=0.0f;
	m_ptr[ZG_M3_E32]=0.0f;
	m_ptr[ZG_M3_E33]=1.0f;


	return m;

}

ZG_Matrix3f ZG_Matrix3f_Mul3x3(const ZG_Matrix3f * _m1, const ZG_Matrix3f * _m2){
	ZG_Matrix3f m=ZG_Matrix3f_New();

	const float * m1=&_m1->e11;
	const float * m2=&_m2->e11;

	m.e11 =   m1[ZG_M3_E11] * m2[ZG_M3_E11]
			+ m1[ZG_M3_E12] * m2[ZG_M3_E21]
			+ m1[ZG_M3_E13] * m2[ZG_M3_E31];

	m.e12 =   m1[ZG_M3_E11] * m2[ZG_M3_E12]
			+ m1[ZG_M3_E12] * m2[ZG_M3_E22]
			+ m1[ZG_M3_E13] * m2[ZG_M3_E32];

	m.e13 =   m1[ZG_M3_E11] * m2[ZG_M3_E13]
			+ m1[ZG_M3_E12] * m2[ZG_M3_E23]
			+ m1[ZG_M3_E13] * m2[ZG_M3_E33];

	m.e21 =   m1[ZG_M3_E21] * m2[ZG_M3_E11]
			+ m1[ZG_M3_E22] * m2[ZG_M3_E21]
			+ m1[ZG_M3_E23] * m2[ZG_M3_E31];

	m.e22 =   m1[ZG_M3_E21] * m2[ZG_M3_E12]
			+ m1[ZG_M3_E22] * m2[ZG_M3_E22]
			+ m1[ZG_M3_E23] * m2[ZG_M3_E32];

	m.e23 =   m1[ZG_M3_E21] * m2[ZG_M3_E13]
			+ m1[ZG_M3_E22] * m2[ZG_M3_E23]
			+ m1[ZG_M3_E23] * m2[ZG_M3_E33];

	m.e31 =   m1[ZG_M3_E31] * m2[ZG_M3_E11]
			+ m1[ZG_M3_E32] * m2[ZG_M3_E21]
			+ m1[ZG_M3_E33] * m2[ZG_M3_E31];

	m.e32 =   m1[ZG_M3_E31] * m2[ZG_M3_E12]
			+ m1[ZG_M3_E32] * m2[ZG_M3_E22]
			+ m1[ZG_M3_E33] * m2[ZG_M3_E32];

	m.e33 =   m1[ZG_M3_E31] * m2[ZG_M3_E13]
			+ m1[ZG_M3_E32] * m2[ZG_M3_E23]
			+ m1[ZG_M3_E33] * m2[ZG_M3_E33];

	return m;
}

ZG_Vector3f ZG_Matrix3f_TransformV3f(const ZG_Matrix3f *_this, ZG_Vector3f v){
	ZG_Vector3f transform;
	float *m=(float *)&_this->e11;

	transform.x=v.x*m[ZG_M3_E11]+v.y*m[ZG_M3_E12]+v.z*m[ZG_M3_E13];
	transform.y=v.x*m[ZG_M3_E21]+v.y*m[ZG_M3_E22]+v.z*m[ZG_M3_E23];
	transform.z=v.x*m[ZG_M3_E31]+v.y*m[ZG_M3_E32]+v.z*m[ZG_M3_E33];

	return transform;
}
