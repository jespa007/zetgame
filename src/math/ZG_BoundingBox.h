#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

typedef struct ZG_BoundingBox ZG_BoundingBox;

struct ZG_BoundingBox{
	float minx,miny,maxx,maxy;
};

ZG_BoundingBox  ZG_BoundingBox_New(void);
ZG_BoundingBox  ZG_BoundingBox_New4f(
	float xmin
	,float _ymin
	,float _xmax
	,float _ymax
);

ZG_BoundingBox  ZG_BoundingBox_MulFactor(ZG_BoundingBox _bb, float _factor);

#endif
