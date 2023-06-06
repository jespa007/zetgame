#ifndef __RECTANGLE4F_H__
#define __RECTANGLE4F_H__

typedef struct Rectangle4f Rectangle4f;

struct Rectangle4f{
	float x1,y1,x2,y2;
};

Rectangle4f  Rectangle4f_New4f(float x1,float y1,float x2,float y2);


#endif
