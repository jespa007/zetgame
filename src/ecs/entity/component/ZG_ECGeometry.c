#include "ecs/@zg_ecs.h"


void ZG_ECGeometry_Setup(void *_this){
	ZG_ECGeometry *ec_geometry = _this;
	ec_geometry->geometry=NULL;//ZG_Geometry_New();
}

void ZG_ECGeometry_Destroy(void *_this){
	ZG_ECGeometry *ec_geometry = _this;
	if(ec_geometry->geometry!=NULL){
		ZG_Geometry_Delete(ec_geometry->geometry);
	}
}
