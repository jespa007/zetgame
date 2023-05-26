#include "../zg_ecs.h"

typedef struct{
	EntitySystem *entity_system;
}EntityData;

Entity *Entity_New(EntitySystem *_entity_system){
	Entity *entity=ZG_NEW(Entity);
	EntityData *data=ZG_NEW(EntityData);
	size_t components_length=sizeof(void *)*EntitySystem_NumComponents();
	entity->components=malloc(components_length);
	memset(entity->components,0,components_length);
	entity->active=false;

	entity->data=data;
	data->entity_system=_entity_system;
	return entity;
}

/*
void Entity_Ini(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=true;
}
*/
void Entity_Die(Entity *_this){
	/*EntityData *data=(EntityData *)_this->data;
	data->is_active=false;*/
	_this->active=false;
}


EntitySystem *Entity_GetEntitySystem(Entity *_this){
 EntityData *data=(EntityData *)_this->data;
 return data->entity_system;
}
/*
void Entity_Reset(Entity *_this){
}*/

void Entity_Delete(Entity *_this){
	EntityData *data=_this->data;
	ZG_FREE(_this->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
/*
void Entity_AttachComponent(Entity *_this, uint16_t idx_component, void *ptr_component){

	if(_this->components[idx_component] == NULL){
		_this->components[idx_component]=ptr_component;
	}else{
		Log_Error("Entity component not null");
	}
}

void Entity_DeAttachComponent(Entity *_this, uint16_t idx_component){

	if(_this->components[idx_component] != NULL){
		_this->components[idx_component]=NULL;
	}else{
		Log_Error("Entity component already null");
	}
}
*/

