#include "@zg_graphics.h"

//
// Right-handed system
// By convention, OpenGL is a right-handed system. What this basically says is that the positive x-axis is to your right, the positive y-axis is up and the positive z-axis is backwards.
// Think of your screen being the center of the 3 axes and the positive z-axis going through your screen towards you. The axes are drawn as follows
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
// To understand why it's called right-handed do the following:
//
// Stretch your right-arm along the positive y-axis with your hand up top.
// Let your thumb point to the right.
// Let your pointing finger point up.
// Now bend your middle finger downwards 90 degrees.
// If you did things right, your thumb should point towards the positive x-axis, the pointing finger towards the positive y-axis and your middle finger towards the positive z-axis.
// If you were to do this with your left-arm you would see the z-axis is reversed. This is known as a left-handed system and is commonly used by DirectX.
// Note that in normalised device coordinates OpenGL actually uses a left-handed system (the projection matrix switches the handedness).
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

void ZG_Transform_GL_Apply(ZG_Transform *_this, ZG_Matrix4f *_rotation_matrix4x4){

	glPushMatrix();


	glTranslatef(_this->translate.x
			,_this->translate.y
			,_this->translate.z);
	glMultMatrixf((float *)_rotation_matrix4x4);
	glScalef(
			_this->scale.x
			,_this->scale.y
			,_this->scale.z);
}


void ZG_Transform_GL_Restore(ZG_Transform *transform){
	ZG_UNUSUED_PARAM(transform);
	glPopMatrix();
}
