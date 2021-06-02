#include "ecs/zg_ecs.h"

typedef struct{
	void 		*components[EC_COMPONENT_MAX];
	bool is_active;
}EntityData;

Entity *Entity_New(void){
	Entity *entity=NEW(Entity);
	EntityData *entity_data=NEW(EntityData);
	memset(entity_data,0,sizeof(EntityData));
	entity->data=entity_data;
	return entity;
}

void Entity_Ini(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=true;
	memset(data->components,0,sizeof(data->components));
}

void Entity_Die(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=false;
}


void Entity_AttachComponent(Entity *_this, int idx_component, void *ptr_component){
	EntityData *data=(EntityData *)_this->data;
	if(data->components[idx_component] == NULL){
		data->components[idx_component]=ptr_component;
	}else{
		Log_Error("Entity component not null");
	}
}

void Entity_DeAttachComponent(Entity *_this, int idx_component){
	EntityData *data=(EntityData *)_this->data;
	if(data->components[idx_component] != NULL){
		data->components[idx_component]=NULL;
	}else{
		Log_Error("Entity component already null");
	}
}

void * Entity_GetComponent(Entity *_this, int idx_component){
	EntityData *data=(EntityData *)_this->data;
	return data->components[idx_component];
}

void Entity_SetTranslate3f(Entity *_this,float x, float y, float z){
	EntityData *data=(EntityData *)_this->data;
	ECTransform *sg_node=data->components[EC_COMPONENT_TRANSFORM];
	if(sg_node != NULL){
		ECTransform_SetTranslate3f(sg_node,x,y,z);
	}
}
