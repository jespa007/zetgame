#version 130

uniform mat4 MVP; // rotation matrix transformation...
varying float vRotation;

void main(void) {

	vRotation = (gl_Normal.y*3.14)/(180*4);

	//vec4 camera_pos = gl_ModelViewMatrix[3];	
	//float dist = distance(gl_Position.xyz, camera_pos.xyz);
	//float psize = (gl_Normal.x) / dist;
	//gl_PointSize = // assumption: -z points into the screen
	gl_PointSize = (480.0 / -(gl_ModelViewMatrix * gl_Vertex).z)*gl_Normal.x;
	gl_Position = ftransform();
	gl_FrontColor = gl_Color;
	
}