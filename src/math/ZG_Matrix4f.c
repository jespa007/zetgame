#include "_zg_math_.h"



ZG_Matrix4f Matrix4f_New(void){
	return Matrix4f_Identity();
}

ZG_Matrix4f Matrix4f_FromMatrix3f(const ZG_Matrix3f *_m3){
	return (ZG_Matrix4f){
		 _m3->e11,_m3->e12,_m3->e13	,0
		,_m3->e21,_m3->e22,_m3->e23	,0
		,_m3->e31,_m3->e32,_m3->e33	,0
		, 0		 , 0	  , 0		,1
	};
}

ZG_Matrix4f Matrix4f_Identity(void){

	ZG_Matrix4f m;
	float *m_ptr=&m.e11;

	m_ptr[ZG_M4_E11]=1.0f;
	m_ptr[ZG_M4_E12]=0.0f;
	m_ptr[ZG_M4_E13]=0.0f;
	m_ptr[ZG_M4_E14]=0.0f;

	m_ptr[ZG_M4_E21]=0.0f;
	m_ptr[ZG_M4_E22]=1.0f;
	m_ptr[ZG_M4_E23]=0.0f;
	m_ptr[ZG_M4_E24]=0.0f;

	m_ptr[ZG_M4_E31]=0.0f;
	m_ptr[ZG_M4_E32]=0.0f;
	m_ptr[ZG_M4_E33]=1.0f;
	m_ptr[ZG_M4_E34]=0.0f;

	m_ptr[ZG_M4_E41]=0.0f;
	m_ptr[ZG_M4_E42]=0.0f;
	m_ptr[ZG_M4_E43]=0.0f;
	m_ptr[ZG_M4_E44]=1.0f;

	return m;

}

ZG_Matrix4f Matrix4f_Inverse(const ZG_Matrix4f *_this){
	ZG_Matrix4f m=Matrix4f_New();

	float *inverse=&m.e11;
	const float *mvp=&_this->e11;


	//negate_rotate_matrix.setRotateMatrixFrom2Vectors(new_vector, zero_vector);// .load(CTransformation::GetNegateRotateMatrix(new_vector, zero_vector));
	float determinant = +mvp[ZG_M4_E11]*(mvp[ZG_M4_E22]*mvp[ZG_M4_E33]-mvp[ZG_M4_E32]*mvp[ZG_M4_E23])
						-mvp[ZG_M4_E12]*(mvp[ZG_M4_E21]*mvp[ZG_M4_E33]-mvp[ZG_M4_E23]*mvp[ZG_M4_E31])
						+mvp[ZG_M4_E13]*(mvp[ZG_M4_E21]*mvp[ZG_M4_E32]-mvp[ZG_M4_E22]*mvp[ZG_M4_E31]);

	if(determinant != 0){

		float invdet = 1.0f/determinant;
		inverse[ZG_M4_E11] =  (mvp[ZG_M4_E22]*mvp[ZG_M4_E33]-mvp[ZG_M4_E32]*mvp[ZG_M4_E23])*invdet;
		inverse[ZG_M4_E21] = -(mvp[ZG_M4_E12]*mvp[ZG_M4_E33]-mvp[ZG_M4_E13]*mvp[ZG_M4_E32])*invdet;
		inverse[ZG_M4_E31] =  (mvp[ZG_M4_E12]*mvp[ZG_M4_E23]-mvp[ZG_M4_E13]*mvp[ZG_M4_E22])*invdet;
		inverse[ZG_M4_E12] = -(mvp[ZG_M4_E21]*mvp[ZG_M4_E33]-mvp[ZG_M4_E23]*mvp[ZG_M4_E31])*invdet;
		inverse[ZG_M4_E22] =  (mvp[ZG_M4_E11]*mvp[ZG_M4_E33]-mvp[ZG_M4_E13]*mvp[ZG_M4_E31])*invdet;
		inverse[ZG_M4_E32] = -(mvp[ZG_M4_E11]*mvp[ZG_M4_E23]-mvp[ZG_M4_E21]*mvp[ZG_M4_E13])*invdet;
		inverse[ZG_M4_E13] =  (mvp[ZG_M4_E21]*mvp[ZG_M4_E32]-mvp[ZG_M4_E31]*mvp[ZG_M4_E22])*invdet;
		inverse[ZG_M4_E23] = -(mvp[ZG_M4_E11]*mvp[ZG_M4_E32]-mvp[ZG_M4_E31]*mvp[ZG_M4_E12])*invdet;
		inverse[ZG_M4_E33] =  (mvp[ZG_M4_E11]*mvp[ZG_M4_E22]-mvp[ZG_M4_E21]*mvp[ZG_M4_E12])*invdet;

	}else{
		ZG_Log_ErrorF("Determinant zero");
	}

	return m;

}

