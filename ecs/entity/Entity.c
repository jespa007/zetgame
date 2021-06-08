#include "ecs/zg_ecs.h"

typedef struct{
	bool is_active;
}EntityData;

Entity *Entity_New(void){
	Entity *entity=NEW(Entity);
	EntityData *data=NEW(EntityData);
	memset(data,0,sizeof(EntityData));
	entity->components=malloc(sizeof(void *)*ESSystem_NumComponents());
	entity->data=data;
	return entity;
}

void Entity_Ini(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=true;

	// clear previous used components
	memset(_this->components,0,sizeof(void *)*ESSystem_NumComponents());
}

void Entity_Die(Entity *_this){
	EntityData *data=(EntityData *)_this->data;
	data->is_active=false;
}


void Entity_Reset(Entity *_this){

}

void Entity_AttachComponent(Entity *_this, unsigned idx_component, void *ptr_component){

	if(_this->components[idx_component] == NULL){
		_this->components[idx_component]=ptr_component;
	}else{
		Log_Error("Entity component not null");
	}
}

void Entity_DeAttachComponent(Entity *_this, unsigned idx_component){

	if(_this->components[idx_component] != NULL){
		_this->components[idx_component]=NULL;
	}else{
		Log_Error("Entity component already null");
	}
}


