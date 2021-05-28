#ifndef __RECTANGLE_F_H__
#define __RECTANGLE_F_H__

typedef struct Rectanglef Rectanglef;

struct Rectanglef{
	float x1,y1,x2,y2;
};

Rectanglef  Rectanglef_New4f(float x1,float y1,float x2,float y2);


#endif
