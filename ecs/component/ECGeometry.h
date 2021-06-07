#ifndef __EC_GEOMETRY_H__
#define __EC_GEOMETRY_H__

typedef struct ECGeometry ECGeometry;

struct ECGeometry{
	Geometry *geometry;
};

void ECGeometry_Setup(void *_this);
void ECGeometry_Destroy(void *_this);

#endif
