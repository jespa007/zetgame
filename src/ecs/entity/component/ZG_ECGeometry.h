#ifndef __EC_GEOMETRY_H__
#define __EC_GEOMETRY_H__

typedef struct ECGeometry ECGeometry;

struct ECGeometry{
	EComponentHeader header;
	ZG_Geometry 		*geometry;
};

void ECGeometry_Setup(void *_this, ComponentId _id);
void ECGeometry_Destroy(void *_this);

#endif
