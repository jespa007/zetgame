#include "ecs/zg_ecs.h"


void ECGeometry_Setup(void *_this){
	ECGeometry *ec_geometry = _this;
	ec_geometry->id=EC_GEOMETRY;

	ec_geometry->geometry=NULL;//Geometry_New();
}

void ECGeometry_Destroy(void *_this){
	ECGeometry *ec_geometry = _this;
	if(ec_geometry->geometry!=NULL){
		Geometry_Delete(ec_geometry->geometry);
	}
}
