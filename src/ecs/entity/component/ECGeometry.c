#include "ecs/zg_ecs.h"


void ECGeometry_Setup(void *_this, ComponentId _id, Entity *_entity){
	ECGeometry *ec_geometry = _this;
	ec_geometry->header.entity=_entity;
	ec_geometry->header.id=_id;
	_entity->components[EC_GEOMETRY]=_this;

	ec_geometry->geometry=NULL;//Geometry_New();
}

void ECGeometry_Destroy(void *_this){
	ECGeometry *ec_geometry = _this;
	if(ec_geometry->geometry!=NULL){
		Geometry_Delete(ec_geometry->geometry);
	}
}
