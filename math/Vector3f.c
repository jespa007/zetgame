#include "zg_math.h"

Vector3f  Vector3f_New3f(float x, float y, float z){
	return (Vector3f){
		.x=x
		,.y=y
		,.z=z
	};
}

Vector3f  Vector3f_Zeros(){
	return (Vector3f){
		.x=0
		,.y=0
		,.z=0
	};
}
Vector3f  Vector3f_Ones(){
	return (Vector3f){
		.x=1
		,.y=1
		,.z=1
	};
}

Vector3f  Vector3f_Normalize(Vector3f  v){
	float length = 1.0f/sqrtf( v.x*v.x + v.y*v.y + v.z*v.z );
	v.x*=length;
	v.y*=length;
	v.z*=length;
	return v;
}

Vector3f  Vector3f_Add(Vector3f  v1,Vector3f  v2){
	return (Vector3f){
	 .x=v1.x + v2.x
	,.y=v1.y + v2.y
	,.z=v1.z + v2.z
	};
}

Vector3f  Vector3f_Sub(Vector3f  v1,Vector3f  v2){
	return (Vector3f){
	 .x=v1.x - v2.x
	,.y=v1.y - v2.y
	,.z=v1.z - v2.z
	};
}

Vector3f  Vector3f_Mul(Vector3f  v1,Vector3f  v2){
	return (Vector3f){
		.x=v1.x * v2.x
		,.y=v1.y * v2.y
		,.z=v1.z * v2.z
	};
}
