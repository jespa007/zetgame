#include "ecs/zg_ecs.h"


void EntityTransform_SetTranslate3f(Entity *_this,float x, float y, float z){
	ECTransform_SetTranslate3f(Entity_GetComponent(_this,ENTITY_COMPONENT_TRANSFORM),x,y,z);
}

EntityTransform_SetRotate3f(Entity *_this,float x, float y, float z){
	ECTransform_SetRotate3f(Entity_GetComponent(_this,ENTITY_COMPONENT_TRANSFORM),x,y,z);
}

void 	 EntityTransform_SetPosition2i(Entity *_this,int x, int y){
	ECTransform_SetPosition2i(Entity_GetComponent(_this,ENTITY_COMPONENT_TRANSFORM),x,y);
}

void 	 EntityTransform_Attach(Entity *_this, Entity *_entity){
	ECTransform_Attach(
			Entity_GetComponent(_this,ENTITY_COMPONENT_TRANSFORM)
			,Entity_GetComponent(ENTITY_COMPONENT_TRANSFORM,_entity)
	);
}
