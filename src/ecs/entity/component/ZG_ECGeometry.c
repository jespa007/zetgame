#include "ecs/_zg_ecs_.h"


void ZG_ECGeometry_Setup(void *_this, ZG_ComponentId _id){
	ZG_ECGeometry *ec_geometry = _this;
	ec_geometry->header.entity=_entity;
	ec_geometry->header.id=_id;
	ec_geometry->geometry=NULL;//ZG_Geometry_New();
}

void ZG_ECGeometry_Destroy(void *_this){
	ZG_ECGeometry *ec_geometry = _this;
	if(ec_geometry->geometry!=NULL){
		ZG_Geometry_Delete(ec_geometry->geometry);
	}
}
