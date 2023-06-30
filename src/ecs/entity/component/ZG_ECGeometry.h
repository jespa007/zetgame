#ifndef __ZG_ECGEOMETRY_H__
#define __ZG_ECGEOMETRY_H__

typedef struct ZG_ECGeometry ZG_ECGeometry;

struct ZG_ECGeometry{
	ZG_EntityHeader 	header;
	ZG_Geometry 		*geometry;
};

//---
void ZG_ECGeometry_OnCreate(void *_this);
void ZG_ECGeometry_OnDestroy(void *_this);
//---

#endif
