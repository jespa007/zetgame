#include "../zg_ecs.h"

typedef struct{
	ZG_Archetype *archetype;
}EntityData;

ZG_Entity *ZG_Entity_New(ZG_EntityManager *_entity_system){
	ZG_Entity *entity=ZG_NEW(ZG_Entity);
	EntityData *data=ZG_NEW(EntityData);
	size_t components_length=sizeof(void *)*ZG_EntityManager_NumComponents();
	entity->components=malloc(components_length);
	memset(entity->components,0,components_length);
	entity->active=false;

	entity->data=data;
	data->entity_system=_entity_system;
	return entity;
}

/*
void Entity_Ini(ZG_Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=true;
}
*/
void ZG_Entity_Die(ZG_Entity *_this){
	/*EntityData *data=(EntityData *)_this->data;
	data->is_active=false;*/
	_this->active=false;
}


ZG_EntityManager *ZG_Entity_GetEntityManager(ZG_Entity *_this){
 EntityData *data=(EntityData *)_this->data;
 return ZG_Archetype_GetEntityManager(data->entity_system);
}

ZG_Archetype *ZG_Entity_GetArchetype(ZG_Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	return data->archetype;
}

void *ZG_Entity_GetComponent(ZG_Entity *_this, ZG_ComponentId _component_id){
	EntityData *data=(EntityData *)_this->data;
	return ZG_Archetype_GetComponent(data->archetype,_this, _component_id);
}
/*
void ZG_Entity_Reset(ZG_Entity *_this){
}*/

void ZG_Entity_Delete(ZG_Entity *_this){
	EntityData *data=_this->data;
	ZG_FREE(_this->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
/*
void Entity_AttachComponent(ZG_Entity *_this, uint16_t idx_component, void *ptr_component){

	if(_this->components[idx_component] == NULL){
		_this->components[idx_component]=ptr_component;
	}else{
		ZG_LOG_ERROR("ZG_Entity component not null");
	}
}

void Entity_DeAttachComponent(ZG_Entity *_this, uint16_t idx_component){

	if(_this->components[idx_component] != NULL){
		_this->components[idx_component]=NULL;
	}else{
		ZG_LOG_ERROR("ZG_Entity component already null");
	}
}
*/

