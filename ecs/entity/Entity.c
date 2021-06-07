#include "ecs/zg_ecs.h"

typedef struct{
	void 		**components;//[ENTITY_COMPONENT_MAX];
	bool is_active;
}EntityData;

Entity *Entity_New(void){
	Entity *entity=NEW(Entity);
	EntityData *data=NEW(EntityData);
	memset(data,0,sizeof(EntityData));
	data->components=malloc(sizeof(void *)*ESSystem_NumComponents());
	entity->data=data;
	return entity;
}

void Entity_Ini(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=true;

	// clear previous used components
	memset(data->components,0,sizeof(void *)*ESSystem_NumComponents());
}

void Entity_Die(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=false;
}


void Entity_Reset(Entity *_this){

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


