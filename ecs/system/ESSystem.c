#include "ecs/zg_ecs.h"

List *g_es_system_registered_components=NULL;
bool  g_user_can_register_components=false;

typedef struct{
	unsigned id;
	ESSystemRegisterComponent data;
}ESSystemRegisteredComponentData;

typedef struct{
	//size_t 	size_data; // len data component
	uint8_t *ptr_data; // ptr data component
	uint8_t *swap_aux;
	size_t 	n_elements;
	size_t 	n_active_elements;
}ESSystemComponentData;

typedef struct {
	//EntityTypeId id; // the entity type id
	//Entity *entities; // max entities it'll have this type
	//ESSystem *es_system;
	char *name; // name entity type
	unsigned *entity_components;//[ENTITY_COMPONENT_MAX]; // it says the components it has this entity
	size_t n_components; // available components

	uint16_t max_entities; // max entitites of this type (default 1)
	uint16_t n_entities;
	uint16_t active_entities;

	Entity **entities;
	//uint8_t **ref_component_entity[ENTITY_COMPONENT_MAX]; // givent entity n it get the component from system
}EntityTypeData;

typedef struct{
	MapString *map_entity_types;
	List *lst_entity_types;
	ESSystemComponentData **components;//[ENTITY_COMPONENT_MAX];
}ESSystemData;

//---------------------------------------------------
// PRIVATE FUNCTIONS
uint8_t *ESSystem_NewComponent(ESSystem *_this,unsigned idx_component);
void ESSystem_DeleteComponent(ESSystem *_this,unsigned idx_component, uint8_t * component_ref);

//---------------------------------------------------
// STATIC FUNCTIONS
bool ESSystem_Init(void){

	unsigned min_iter=0;

	// invalid (0)
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=0
		,.required_components	=NULL
		,.Component_Setup		=NULL
		,.Component_Init		=NULL
		,.Component_Update		=NULL
		,.Component_Destroy		=NULL
	});

	// transform
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECTransform)
		,.required_components	=NULL
		,.Component_Setup		=ECTransform_Setup
		,.Component_Init		=ECTransform_Init
		,.Component_Update		=ECTransform_Update
		,.Component_Destroy		=ECTransform_Destroy
	});

	// geometry
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECGeometry)
		,.required_components	=NULL
		,.Component_Setup		=ECGeometry_Setup
		,.Component_Init		=ECGeometry_Init
		,.Component_Update		=ECGeometry_Update
		,.Component_Destroy		=ECGeometry_Destroy
	});

	// material
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECMaterial)
		,.required_components	= (ComponentList){0,0}
		,.Component_Setup		=ECMaterial_Setup
		,.Component_Init		=ECMaterial_Init
		,.Component_Update		=ECMaterial_Update
		,.Component_Destroy		=ECMaterial_Destroy
	});

	// texture
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECTexture)
		,.required_components	=(ComponentList){0,0}
		,.Component_Setup		=ECTexture_Setup
		,.Component_Init		=ECTexture_Init
		,.Component_Update		=ECTexture_Update
		,.Component_Destroy		=ECTexture_Destroy
	});

	// sprite renderer (1)
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECSpriteRenderer)
		,.required_components	= ECSpriteRenderer_RequiredComponents()
		,.Component_Setup		=ECSpriteRenderer_Setup
		,.Component_Init		=ECSpriteRenderer_Init
		,.Component_Update		=ECSpriteRenderer_Update
		,.Component_Destroy		=ECSpriteRenderer_Destroy
	});

	// Animation transform
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECTransformAnimation)
		,.required_components	= ECTransformAnimation_RequiredComponents()
		,.Component_Setup		=ECTransformAnimation_Setup
		,.Component_Init		=ECTransformAnimation_Init
		,.Component_Update		=ECTransformAnimation_Update
		,.Component_Destroy		=ECTransformAnimation_Destroy
	});

	// Animation material
	ESSystem_RegisterComponent((ESSystemRegisterComponent){
		.size_data				=sizeof(ECMaterialAnimation)
		,.required_components	= ECMaterialAnimation_RequiredComponents()
		,.Component_Setup		=ECMaterialAnimation_Setup
		,.Component_Init		=ECMaterialAnimation_Init
		,.Component_Update		=ECMaterialAnimation_Update
		,.Component_Destroy		=ECMaterialAnimation_Destroy
	});



	// check component consistency
	 min_iter=MIN(g_es_system_registered_components->count,ENTITY_COMPONENT_MAX);
	for(unsigned i=0; i < min_iter; i++){
		ESSystemRegisteredComponentData *component=g_es_system_registered_components->items[i];
		if(component->id != i){
			Log_Error("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	g_user_can_register_components=true;

	return true;

}

unsigned	ESSystem_RegisterComponent(ESSystemRegisterComponent es_component_register){

	if(g_user_can_register_components==false){
		Log_Error("Components should registered before create any Entity-System");
		return ENTITY_COMPONENT_INVALID; //
	}

	if(g_es_system_registered_components == NULL){
		g_es_system_registered_components=List_New();
	}

	unsigned idx_component=g_es_system_registered_components->count;
	ESSystemRegisteredComponentData *new_component_register=NEW(ESSystemRegisteredComponentData);
	new_component_register->data=es_component_register;
	List_Add(g_es_system_registered_components,new_component_register);

	return idx_component;
}

size_t					ESSystem_NumComponents(void){
	if(g_es_system_registered_components != NULL){
		return g_es_system_registered_components->count;
	}

	return 0;
}

void ESSystem_DeInit(void){

}

//---------------------------------------------------
// PUBLIC FUNCTIONS
ESSystem *ESSystem_New(void){
	ESSystem *system=NEW(ESSystem);
	ESSystemData *data=NEW(ESSystemData);
	data->lst_entity_types=List_New();
	data->map_entity_types=MapString_New();

	data->components=malloc(sizeof(ESSystemComponentData)*g_es_system_registered_components->count);
	memset(data->components,0,sizeof(ESSystemComponentData)*g_es_system_registered_components->count);

	system->data=data;

	// after first system is created, user cannot register any component anymore
	g_user_can_register_components=false;

	return system;
}

List * ESSystem_CheckComponentRequirements(unsigned *entity_components, size_t entity_components_len){
	// check number of whether there's no component
	List *list=NULL;//List_New();

	for(unsigned i=0; i < entity_components_len;i++){
		unsigned idx=entity_components[i];
		ESSystemRegisteredComponentData *registered_component=g_es_system_registered_components->items[i];
		ComponentList req_com=registered_component->data.required_components;
		for(unsigned j=0; j < req_com.n_components;j++){
			// check if exists...
			unsigned idx_component_req_to_find=req_com.components[j];
			bool found=false;
			for(unsigned k=0; k < entity_components_len && found==false;k++){
				if(idx_component_req_to_find == entity_components[i]){
					found=true;
				}
			}

			if(found==false){
				if(list == NULL){
					list=List_New();
				}

				List_Add(list,idx_component_req_to_find);
			}
		}

		if(list != NULL){ // add this component it self
			List_Add(list,idx);
		}
	}

	return list;
}

Entity *ESSystem_NewEntity(ESSystem *_this,unsigned *entity_components, size_t entity_components_len){
	ESSystemData *data=_this->data;
	char _str_entity_type[1024]="__@_comp";

	unsigned *new_entity_components_by_requirements=NULL;
	unsigned *new_entity_components_by_requirements_len=NULL;

	// 1. check component requirements


	// 2. sort numbers


	// internal type that is generated according used components...
	for(unsigned i=0; i < entity_components_len; i++){
		char *num=StrUtils_IntToStr(entity_components[i]);
		strcat(_str_entity_type,"_");
		strcat(_str_entity_type,num);
		free(num);
	}

	strcat(_str_entity_type,"_@__");

	EntityTypeData *entity_type_data=MapString_GetValue(data->map_entity_types,_str_entity_type);

	if(entity_type_data == NULL){ // create ...
		//Log_Error("Entity type %s not exist",_str_entity_type);
		//return NULL;
		entity_type_data=(EntityTypeData *)ESSystem_NewEntityType(_this,_str_entity_type ,1,entity_components, entity_components_len);

	}

	return ESSystem_NewEntityFromType(_this,_str_entity_type);
}

Entity  *ESSystem_NewEntityFromType(ESSystem *_this,const char *_str_entity_type){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=MapString_GetValue(data->map_entity_types,_str_entity_type);

	if(entity_type_data == NULL){
		Log_Error("Entity type %s not exist",_str_entity_type);
		return NULL;
	}

	Entity *entity=NULL;
	if(entity_type_data->active_entities>=entity_type_data->n_entities && entity_type_data->active_entities<entity_type_data->max_entities){ // extend entity
		ESSystem_ExtendEntities(_this,entity_type_data,1);
	}else{
		return NULL;
	}

	entity=entity_type_data->entities[entity_type_data->active_entities];
	Entity_Reset(entity);

	for(unsigned idx_component=0; idx_component < entity_type_data->n_components; idx_component++){ //(msk_ec_it>>idx_component){ // attach all components
		int idx_ec=entity_type_data->entity_components[idx_component];
		uint8_t *ref_component=ESSystem_NewComponent(_this,idx_ec); // request free & set default values component
		Entity_AttachComponent(entity,idx_component,ref_component);
	}

	entity_type_data->active_entities++;
	return entity;
}

void ESSystem_FreeDataComponents(ESSystem *_this,unsigned idx_component){
	ESSystemData *data=_this->data;

	if(idx_component >= g_es_system_registered_components->count){
		return;
	}

	ESSystemComponentData *component_data=data->components[idx_component];
	ESSystemRegisterComponent  *registered_component=&((ESSystemRegisteredComponentData *)g_es_system_registered_components->items[idx_component])->data;
	if(component_data->ptr_data!=NULL){ // deinit current
		uint8_t *ptr_data=component_data->ptr_data;
		for(unsigned i=0; i < component_data->n_active_elements; i++){
			registered_component->Component_Destroy(ptr_data);
			ptr_data+=registered_component->size_data;
		}
		free(component_data->ptr_data);
		component_data->ptr_data=NULL;
		component_data->n_elements=0;
	}
}

void ESSystem_ExtendComponent(ESSystem *_this,unsigned idx_component, size_t extend){

	if(idx_component >= g_es_system_registered_components->count){
		return;
	}

	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=data->components[idx_component];
	ESSystemRegisterComponent  registered_component_data=((ESSystemRegisteredComponentData *)g_es_system_registered_components->items[idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	int n_new_elements=current_component_data_len+extend;
	uint8_t *old_ptr=component_data->ptr_data;
	uint8_t *ptr_com=NULL;


	// free current...
	//ESSystem_FreeDataComponents(_this,idx_component);

	// do a realloc...
	component_data->ptr_data=malloc(registered_component_data.size_data*n_new_elements);
	memcpy(component_data->ptr_data,old_ptr,registered_component_data.size_data*component_data->n_elements);

	// initialize new components
	ptr_com= component_data->ptr_data + registered_component_data.size_data*component_data->n_elements;
	for(int i=component_data->n_elements; i < n_new_elements; i++){
		registered_component_data.Component_Setup(ptr_com);
		ptr_com+=registered_component_data.size_data;
	}

	// update new elements
	component_data->n_elements=n_new_elements;

	// finally do a
}

void ESSystem_ExtendEntities(ESSystem *_this,EntityTypeData *entity_type_data, size_t extend_entities){
	size_t total_extend=entity_type_data->n_entities;

	if(total_extend >= entity_type_data->max_entities){
		Log_Error("cannot extend entity type '%s' up to '%i': Max entities reached (max: %i)",entity_type_data->name,extend_entities,entity_type_data->max_entities);
	}

	for(unsigned i=0; i < entity_type_data->n_components;i++){
		unsigned idx_ec=entity_type_data->entity_components[i];
		// extend components this type will have
		ESSystem_ExtendComponent(_this,idx_ec,extend_entities);
	};

	// realloc set all entity types as this type
	Entity **old_ptr=entity_type_data->entities;
	entity_type_data->entities=malloc(sizeof(Entity)*total_extend);
	memcpy(entity_type_data->entities,old_ptr,sizeof(Entity)*entity_type_data->n_entities);

	for(unsigned i=entity_type_data->n_entities; i < total_extend;i++){
		entity_type_data->entities[i]=Entity_New();
	}

	entity_type_data->n_entities=total_extend;
}

void * ESSystem_NewEntityType(ESSystem *_this
		, const char *_str_entity_type
		, uint16_t max_entities
		, unsigned *entity_components
		, size_t entity_components_len
	){
	ESSystemData *data=_this->data;
	EntityTypeData *entity_type_data=NEW(EntityTypeData);

	if(entity_components_len == 0){
		return NULL;
	}

	// check and extend required components if needed ...


	entity_type_data->entity_components=malloc(sizeof(bool)*entity_components_len);
	memset(entity_type_data->entity_components,0,sizeof(bool)*entity_components_len);

	//entity_type_data->es_system=_this;
	entity_type_data->n_entities=0;
	entity_type_data->max_entities=0;
	entity_type_data->active_entities=0;

	entity_type_data->name=malloc(strlen(_str_entity_type)*sizeof(char)+1);
	memset(entity_type_data->name,0,strlen(_str_entity_type)*sizeof(char)+1);
	strcpy(entity_type_data->name,_str_entity_type);

	unsigned *dst_ptr=entity_type_data->entity_components;
	unsigned *src_ptr=entity_components;

	// add components neededs for the type...
	for(unsigned i=0; i < entity_components_len;i++){
		// register the components this type will have
		*dst_ptr++=*src_ptr++;
	}

	List_Add(data->lst_entity_types,data);
	MapString_SetValue(data->map_entity_types,_str_entity_type,data);

	// extend entities
	entity_type_data->max_entities=max_entities;
	if(max_entities != (uint16_t)UNLIMITIED_ENTITIES){
		ESSystem_ExtendEntities(
			_this
			,entity_type_data
			,max_entities
		);
	}

	return entity_type_data;
}


/**
 * Reserve component available for use
 */
uint8_t *ESSystem_NewComponent(ESSystem *_this,unsigned idx_component){
	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=data->components[idx_component];
	ESSystemRegisterComponent  registered_component_data=((ESSystemRegisteredComponentData *)g_es_system_registered_components->items[idx_component])->data;
	uint8_t *component_ref=NULL;

	if(component_data->n_active_elements<component_data->n_elements){
		component_ref=component_data->ptr_data+registered_component_data.size_data*component_data->n_active_elements;
		component_data->n_active_elements++;
		return component_ref;
	}

	return NULL;
}

void ESSystem_DeleteComponent(ESSystem *_this,unsigned idx_component, uint8_t * component_ref){
	// PRE: component_ref is the pointer to the list to be inactive
	ESSystemData *data=_this->data;
	ESSystemComponentData *component_data=(ESSystemComponentData *)&data->components[idx_component];
	ESSystemRegisterComponent  registered_component_data=((ESSystemRegisteredComponentData *)g_es_system_registered_components->items[idx_component])->data;

	if(component_data->n_active_elements>0){
		// do a swap between last active and the component_ref to be inactive

		// get the last component active...
		uint8_t *last_component_ref_active=component_data->ptr_data+registered_component_data.size_data*(component_data->n_active_elements-1);
		// copy to swap aux...
		memcpy(component_data->swap_aux,last_component_ref_active,registered_component_data.size_data);
		// copy component_ref to last active component (this time will be inactive)
		memcpy(last_component_ref_active,component_ref,registered_component_data.size_data);
		// copy last active component to the pointer where component_ref is
		memcpy(component_ref,component_data->swap_aux,registered_component_data.size_data);

		// decrease n active elements so component_ref data is already inactive
		component_data->n_active_elements--;
	}

}

void ESSystem_Update(ESSystem * _this){
	ESSystemData *data=(ESSystemData *)_this->data;
	ESSystemRegisteredComponentData  *ptr_registered_component_data=(ESSystemRegisteredComponentData *)g_es_system_registered_components->items;
	ESSystemComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];
	for(int i=0; i < g_es_system_registered_components->count; i++){
		uint8_t *ptr_data=(*component_data)->ptr_data;
		for(unsigned i=0; i < (*component_data)->n_active_elements; i++){
			if(ptr_registered_component_data->data.Component_Update){
				ptr_registered_component_data->data.Component_Update(ptr_data);
			}
			ptr_data+=ptr_registered_component_data->data.size_data;
		}
		ptr_registered_component_data++;
	}

}
