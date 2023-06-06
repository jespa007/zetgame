#include "zg_math.h"

ZG_Vector3f  Vector3f_New3f(float x, float y, float z){
	return (ZG_Vector3f){
		.x=x
		,.y=y
		,.z=z
	};
}

ZG_Vector3f  Vector3f_Zeros(){
	return (ZG_Vector3f){
		.x=0
		,.y=0
		,.z=0
	};
}
ZG_Vector3f  Vector3f_Ones(){
	return (ZG_Vector3f){
		.x=1
		,.y=1
		,.z=1
	};
}

ZG_Vector3f  Vector3f_Normalize(ZG_Vector3f  v){
	float length = 1.0f/sqrtf( v.x*v.x + v.y*v.y + v.z*v.z );
	v.x*=length;
	v.y*=length;
	v.z*=length;
	return v;
}

ZG_Vector3f  Vector3f_Add(ZG_Vector3f  v1,ZG_Vector3f  v2){
	return (ZG_Vector3f){
	 .x=v1.x + v2.x
	,.y=v1.y + v2.y
	,.z=v1.z + v2.z
	};
}

ZG_Vector3f  Vector3f_Sub(ZG_Vector3f  v1,ZG_Vector3f  v2){
	return (ZG_Vector3f){
	 .x=v1.x - v2.x
	,.y=v1.y - v2.y
	,.z=v1.z - v2.z
	};
}

ZG_Vector3f  Vector3f_Mul(ZG_Vector3f  v1,ZG_Vector3f  v2){
	return (ZG_Vector3f){
		.x=v1.x * v2.x
		,.y=v1.y * v2.y
		,.z=v1.z * v2.z
	};
}
