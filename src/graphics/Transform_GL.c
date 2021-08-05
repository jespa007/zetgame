#include "zg_graphics.h"

//
//  Directions based in opengl
//
//  	  	y+
//			^
//     		|
//    	 	|
//     		|.0
//     		+-------> x+
//    	   /
//   	  /
//  	 /
// 		z+
//
//
//
// The screen on camera tr(0,0,1)
//
//
//(-ar,+1) --------------------------
//        |                          |
//        |                          |
//        |                          |
//        |                          |
//        |			  +			     |
//        |			(0,0)		     |
//        |						     |
//        |                          |
//        -------------------------- (ar,-1)

void Transform_GL_Apply(Transform *_this){

	glPushMatrix();

	Matrix4f m=Quaternion_ToMatrix4f(_this->quaternion);


	glTranslatef(_this->translate.x
			,_this->translate.y
			,_this->translate.z);
	glMultMatrixf((float *)&m);
	glScalef(
			_this->scale.x
			,_this->scale.y
			,_this->scale.z);
}


void Transform_GL_Restore(Transform *transform){
	glPopMatrix();
}
