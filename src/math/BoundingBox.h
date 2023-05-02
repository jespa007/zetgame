#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

typedef struct BoundingBox BoundingBox;

struct BoundingBox{
	float minx,miny,maxx,maxy;
};

BoundingBox  BoundingBox_New();
BoundingBox  BoundingBox_New4f(float _x1,float_ y1,float _x2,float _y2);


#endif
