#include "ecs/zg_ecs.h"


void ECGeometry_Setup(void *_this, Entity *_entity){
	ECGeometry *ec_geometry = _this;
	ec_geometry->entity=_entity;
	ec_geometry->id=EC_GEOMETRY;
	_entity->components[EC_GEOMETRY]=_this;

	ec_geometry->geometry=NULL;//Geometry_New();
}

void ECGeometry_Destroy(void *_this){
	ECGeometry *ec_geometry = _this;
	if(ec_geometry->geometry!=NULL){
		Geometry_Delete(ec_geometry->geometry);
	}
}
