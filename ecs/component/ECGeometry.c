#include "ecs/zg_ecs.h"


void ECGeometry_Setup(void *_this){
	ECGeometry *ec_geometry = _this;
	ec_geometry->geometry=Geometry_New();
}

void ECGeometry_Destroy(void *_this){
	ECGeometry *ec_geometry = _this;
	Geometry_Delete(ec_geometry->geometry);
}
