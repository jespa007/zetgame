#ifndef __RECTANGLE_I_H__
#define __RECTANGLE_I_H__

typedef struct Rectanglei Rectanglei;

struct Rectanglei{
	int x1,y1,x2,y2;
};

Rectanglei  Rectanglei_New4i(int x1,int y1,int x2,int y2);


#endif
