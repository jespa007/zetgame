#ifndef __EC_GEOMETRY_H__
#define __EC_GEOMETRY_H__

typedef struct ECGeometry ECGeometry;

struct ECGeometry{
	EComponentHeader header;
	Geometry 		*geometry;
};

void ECGeometry_Setup(void *_this, Entity *_entity);
void ECGeometry_Destroy(void *_this);

#endif
