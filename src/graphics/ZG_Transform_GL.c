#include "_zg_graphics_.h"

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

void ZG_Transform_GL_Apply(ZG_Transform *_this){

	glPushMatrix();

	ZG_Matrix4f m=ZG_Transform_GetRotationMatrix(_this);


	glTranslatef(_this->translate.x
			,_this->translate.y
			,_this->translate.z);
	glMultMatrixf((float *)&m);
	glScalef(
			_this->scale.x
			,_this->scale.y
			,_this->scale.z);
}


void ZG_Transform_GL_Restore(ZG_Transform *transform){
	ZG_UNUSUED_PARAM(transform);
	glPopMatrix();
}
