#include "system/zg_system.h"
/*
typedef struct{
	bool is_active;
}EntityData;*/

Entity *Entity_New(void){
	Entity *entity=ZG_NEW(Entity);
	//EntityData *data=ZG_NEW(EntityData);
	//memset(data,0,sizeof(EntityData));
	entity->components=malloc(sizeof(void *)*EntitySystem_NumComponents());
	entity->active=false;
	//entity->data=data;
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

/*
void Entity_Reset(Entity *_this){
}*/

void Entity_Delete(Entity *_this){
	//EntityData *data=_this->data;
	ZG_FREE(_this->components);
	//ZG_FREE(data);
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

