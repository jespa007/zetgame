#ifndef __ZG_ECGEOMETRY_H__
#define __ZG_ECGEOMETRY_H__

typedef struct ZG_ECGeometry ZG_ECGeometry;

struct ZG_ECGeometry{
	ZG_EComponentHeader header;
	ZG_Geometry 		*geometry;
};

void ZG_ECGeometry_Setup(void *_this, ZG_ComponentId _id);
void ZG_ECGeometry_Destroy(void *_this);

#endif
