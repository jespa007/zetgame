#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

typedef struct BoundingBox BoundingBox;

struct BoundingBox{
	float minx,miny,maxx,maxy;
};

BoundingBox  BoundingBox_New(void);
BoundingBox  BoundingBox_New4f(
	float xmin
	,float _ymin
	,float _xmax
	,float _ymax
);


#endif
