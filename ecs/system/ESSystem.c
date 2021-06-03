#include "ecs/zg_ecs.h"

typedef struct{
	size_t 	size_data;
	uint8_t *ptr_data;
	uint8_t *swap_aux;
	size_t 	n_elements;
	size_t 	n_active_elements;
	void (*ini)(void); // function to setup or init component

}ESSystemComponentData;

typedef struct{
	//EntityTypeId id; // the entity type id
	//Entity *entities; // max entities it'll have this type


	//ESSystem *es_system;
	char *name; // name entity type
	bool entity_components[ENTITY_COMPONENT_MAX]; // it says the components it has this entity

	short max_entities; // max entitites of this type (default 1)
	short active_entities;
	Entity **entities;

	//uint8_t **ref_component_entity[ENTITY_COMPONENT_MAX]; // givent entity n it get the component from system
}EntityTypeData;

typedef struct{
	MapString *map_entity_types;
	List *lst_entity_types;
	ESSystemComponentData components[ENTITY_COMPONENT_MAX];
}ESSystemData;

//---------------------------------------------------
// PRIVATE FUNCTIONS
uint8_t *ESSystem_NewComponent(ESSystem *_this,int idx_component);
void ESSystem_DeleteComponent(ESSystem *_this,int idx_component, uint8_t * component_ref);


//---------------------------------------------------
// PUBLIC FUNCTIONS
ESSystem *ESSystem_New(void){
	ESSystem *system=NEW(ESSystem);
	ESSystemData *data=NEW(ESSystemData);
	data->lst_entity_types=List_New();
	data->map_entity_types=MapString_New();

	// initialize all component struct

	system->data=data;

	return system;
}

void ESSystem_ExtendComponent(ESSystem *_this,int idx_component, int extend){
	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=&data->components[idx_component];

	int n_elements=component_data->n_elements+extend;

	if(component_data->ptr_data!=NULL){
		free(component_data->ptr_data);
	}

	component_data->ptr_data=malloc(component_data->size_data*n_elements);
	component_data->n_elements=n_elements;
}

void  ESSystem_NewEntityType(ESSystem *_this
		, const char *_str_entity_type
		,size_t max_entities
		,EntityComponent *entity_components
		, size_t entity_components_len){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=NEW(EntityTypeData);

	//entity_type_data->es_system=_this;
	entity_type_data->max_entities=max_entities;
	entity_type_data->active_entities=0;
	//entity_type_data->entity_components=entity_components;
	//entity_type_data->entity_components_len=entity_components_len;

	entity_type_data->name=malloc(strlen(_str_entity_type)*sizeof(char)+1);
	memset(entity_type_data->name,0,strlen(_str_entity_type)*sizeof(char)+1);
	strcpy(entity_type_data->name,_str_entity_type);

	//uint32_t msk_ec_it=msk_ec_types;
	//uint32_t component_id=0;
	// add components neededs for the type...
	for(unsigned i=0; i < entity_components_len;i++){
		short idx_ec=entity_components[i];
		if(entity_type_data->entity_components[idx_ec]==false){
			entity_type_data->entity_components[idx_ec]=true;
			ESSystem_ExtendComponent(_this,idx_ec,max_entities);
		}
	};

	List_Add(data->lst_entity_types,data);
	MapString_SetValue(data->map_entity_types,_str_entity_type,data);

	// set all entity types as this type
	entity_type_data->entities=malloc(sizeof(Entity)*max_entities);

	for(unsigned i=0; i < max_entities;i++){
		entity_type_data->entities[i]=Entity_New();
	}
}

Entity  *ESSystem_NewEntity(ESSystem *_this,const char *_str_entity_type){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=MapString_GetValue(data->map_entity_types,_str_entity_type);

	if(entity_type_data == NULL){
		Log_Error("Entity type %s not exist",_str_entity_type);
		return NULL;
	}

	Entity *entity=NULL;
	if(entity_type_data->active_entities<entity_type_data->max_entities){
		//uint32_t msk_ec_it=entity_type_data->msk_ec_types;
		//uint32_t idx_component=0;
		//uint16_t entity_id=entity_type_data->active_entities++;
		//Entity entity=entity_type_data->id|entity_id;
		entity=entity_type_data->entities[entity_type_data->active_entities];
		Entity_Reset(entity);

		for(unsigned idx_component=0; idx_component < ENTITY_COMPONENT_MAX; idx_component++){ //(msk_ec_it>>idx_component){ // attach all components
			//uint32_t msk_component=(msk_ec_it & (msk_ec_it<<idx_component));
			if(entity_type_data->entity_components[idx_component]){// msk_component){ // attach component to entity

				uint8_t *ref_component=ESSystem_NewComponent(_this,idx_component); // request free & set default values component
				//entity_type_data->ref_component_entity[idx_component][entity_id]=ref_component; // attach component to entity
				Entity_AttachComponent(entity,idx_component,ref_component);
			}
			//idx_component++;
		}

		entity_type_data->active_entities++;
	}

	return entity;
}

/*Entity  *ESSystem_Entity(ESSystem *_this,const char *type){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=MapString_GetValue(data->map_entity_types,type);

	Entity *entity=NULL;
	if(entity_type_data->active_entities<entity_type_data->max_entities){
		uint32_t msk_ec_it=entity_type_data->msk_ec_types;
		uint32_t idx_component=0;
		//uint16_t entity_id=entity_type_data->active_entities++;
		//Entity entity=entity_type_data->id|entity_id;
		entity=entity_type_data->entities[entity_type_data->active_entities];
		Entity_Reset(entity);

		while(msk_ec_it>>idx_component){ // attach all components
			uint32_t msk_component=(msk_ec_it & (msk_ec_it<<idx_component));
			if(msk_component){ // attach component to entity

				uint8_t *ref_component=ESSystem_NewComponent(_this,idx_component); // request free & set default values component
				//entity_type_data->ref_component_entity[idx_component][entity_id]=ref_component; // attach component to entity
				Entity_AttachComponent(entity,idx_component,ref_component);
			}
			idx_component++;
		}

		entity_type_data->active_entities++;
	}

	return entity;
}*/

/*Entity  ESSystem_NewEntity(ESSystem *_this,const char *type){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=MapString_GetValue(data->map_entity_types,type);

	Entity entity=ZG_INVALID_ENTITY_ID;
	if(entity_type_data->active_entities<entity_type_data->max_entities){
		uint32_t msk_ec_it=entity_type_data->msk_ec_types;
		uint32_t idx_component=0;
		//uint16_t entity_id=entity_type_data->active_entities++;
		//Entity entity=entity_type_data->id|entity_id;
		Entity *entity=entity_type_data->entities[entity_type_data->active_entities];
		Entity_Reset(entity);

		while(msk_ec_it>>idx_component){ // attach all components
			uint32_t msk_component=(msk_ec_it & (msk_ec_it<<idx_component));
			if(msk_component){ // attach component to entity

				uint8_t *ref_component=ESSystem_NewComponent(_this,idx_component); // request free & set default values component
				//entity_type_data->ref_component_entity[idx_component][entity_id]=ref_component; // attach component to entity
				Entity_AttachComponent(entity,idx_component,ref_component);
			}
			idx_component++;
		}
	}

	return entity;
}*/

uint8_t *ESSystem_NewComponent(ESSystem *_this,int idx_component){
	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=&data->components[idx_component];
	uint8_t *component_ref=NULL;

	if(component_data->n_active_elements<component_data->n_elements){
		component_ref=component_data->ptr_data+component_data->size_data*component_data->n_active_elements;
		component_data->n_active_elements++;
		return component_ref;
	}

	return NULL;
}

void ESSystem_DeleteComponent(ESSystem *_this,int idx_component, uint8_t * component_ref){
	// PRE: component_ref is the pointer to the list to be inactive
	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=&data->components[idx_component];

	if(component_data->n_active_elements>0){
		// do a swap between last active and the component_ref to be inactive

		// get the last component active...
		uint8_t *last_component_ref_active=component_data->ptr_data+component_data->size_data*(component_data->n_active_elements-1);
		// copy to swap aux...
		memcpy(component_data->swap_aux,last_component_ref_active,component_data->size_data);
		// copy component_ref to last active component (this time will be inactive)
		memcpy(last_component_ref_active,component_ref,component_data->size_data);
		// copy last active component to the pointer where component_ref is
		memcpy(component_ref,component_data->swap_aux,component_data->size_data);

		// decrease n active elements so component_ref data is already inactive
		component_data->n_active_elements--;
	}

}

void ESSystem_Update(ESSystem * _this){
	ESSystemData *data=(ESSystemData *)_this->data;

	//----------------------------------------------------------------------------------
	// UPDATE TRANSFORM COMPONENT
	ESSystemComponentData *component_data=&data->components[ENTITY_COMPONENT_TRANSFORM];
	uint8_t *ptr_data=component_data->ptr_data;
	for(unsigned i=0; i < component_data->n_active_elements; i++){
		ECTransform_Update(((ECTransform *)ptr_data));
		ptr_data+=component_data->size_data;
	}

	//----------------------------------------------------------------------------------
	// UPDATE RENDERABLE COMPONENTS
	component_data=&data->components[ENTITY_COMPONENT_SPRITE_RENDERER];
	ptr_data=component_data->ptr_data;
	for(unsigned i=0; i < component_data->n_active_elements; i++){
		ECSpriteRenderer_Update(((ECSpriteRenderer *)ptr_data));
		ptr_data+=component_data->size_data;
	}

	/*ESSystemComponentData *component_data=&data->components[ENTITY_COMPONENT_VIEWER2D];
	uint8_t *ptr_data=component_data->ptr_data;
	for(unsigned i=0; i < component_data->n_active_elements; i++){
		ECViewer2d_Update(((ECViewer2d *)ptr_data));
		ptr_data+=component_data->size_data;
	}

	ESSystemComponentData *component_data=&data->components[ENTITY_COMPONENT_TEXT2D];
	uint8_t *ptr_data=component_data->ptr_data;
	for(unsigned i=0; i < component_data->n_active_elements; i++){
		ECText2d_Update(((ECText2d *)ptr_data));
		ptr_data+=component_data->size_data;
	}*/
	//
	//----------------------------------------------------------------------------------


	// this function batches all components...
	/*for(unsigned ec=0; ec < ENTITY_COMPONENT_MAX; ec++){
		ESSystemComponentData *component_data=&data->components[ec];
		//unsigned it=component_data->n_active_elements;
		uint8_t *ptr_data=component_data->ptr_data;

		for(unsigned i=0; i < component_data->n_active_elements; i++){
			switch(ec){
				case ENTITY_COMPONENT_TRANSFORM: // it should update all root nodes
					ECTransform_Update(((ECTransform *)ptr_data));
					break;
				default:
					break;
			}
			ptr_data+=component_data->size_data;
		}
	}*/


}
