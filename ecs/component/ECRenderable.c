#include "ecs/zg_ecs.h"

ECRenderable *ECRenderable_New(Entity *_entity
		, uint32_t geometry_properties
		, uint32_t material_properties){

	ECRenderable * ec_renderable = NEW(ECRenderable);
	ec_renderable->entity=_entity;
	ec_renderable->appearance=Appearance_New();
	ec_renderable->geometry=Geometry_NewQuad(); // Quad by default ?

	return ec_renderable;
}

void ECRenderable_Update(ECRenderable *_this){
	ECTransform *ec_transform=Entity_GetComponent(_this->entity,ENTITY_COMPONENT_TRANSFORM);
	Graphics_Draw(&ec_transform->transform,_this->geometry,_this->appearance);
}

void ECRenderable_Delete(ECRenderable *_this){

}
