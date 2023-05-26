#include "../zg_ecs.h"

List *g_entity_system_registered_components=NULL;
bool  g_entity_system_user_can_register_components=true;

typedef struct{
	EComponent id;
	EntitySystemRegisterEComponent data;
}EntitySystemRegisteredEComponentData;

typedef struct{
	uint8_t	*ptr_data; // we want to keep pointer in order to support future reallocs
	size_t  	n_elements;
}EntitySystemEComponentData;


typedef struct{
	MapString *map_entity_managers;
	List *lst_entity_managers;
	EntitySystemEComponentData **components;//[ENTITY_COMPONENT_MAX];
}EntitySystemData;

//---------------------------------------------------
// PRIVATE FUNCTIONS

bool	EntitySystem_RegisterComponentBuiltin(EComponent _idx_component,EntitySystemRegisterEComponent es_component_register){

	if(g_entity_system_user_can_register_components==false){
		Log_ErrorF("Components should registered before create any Entity-System");
		return false; //
	}

	if(g_entity_system_registered_components == NULL){
		g_entity_system_registered_components=List_New();
	}

	EComponent idx_component=_idx_component;//g_entity_system_registered_components->count;
	EntitySystemRegisteredEComponentData *new_component_register=ZG_NEW(EntitySystemRegisteredEComponentData);
	new_component_register->data=es_component_register;
	new_component_register->id=idx_component;
	List_Add(g_entity_system_registered_components,new_component_register);

	return true;
}


//---------------------------------------------------
// STATIC FUNCTIONS
bool EntitySystem_Init(void){

	unsigned min_iter=0;

	// invalid (0)
	EntitySystem_RegisterComponentBuiltin(EC_INVALID,(EntitySystemRegisterEComponent){
		.size_data				=0
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=NULL
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=NULL
	});

	// transform
	EntitySystem_RegisterComponentBuiltin(EC_TRANSFORM,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECTransform)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECTransform_Setup
		,.EComponent_Update		=ECTransform_Update
		,.EComponent_Destroy	=ECTransform_Destroy
	});


	// geometry
	EntitySystem_RegisterComponentBuiltin(EC_GEOMETRY,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECGeometry)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECGeometry_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=ECGeometry_Destroy
	});

	// material
	EntitySystem_RegisterComponentBuiltin(EC_MATERIAL,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECMaterial)
		,.required_components	= (EComponentList){0,0}
		,.EComponent_Setup		=ECMaterial_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=ECMaterial_Destroy
	});

	// texture
	EntitySystem_RegisterComponentBuiltin(EC_TEXTURE,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECTexture)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECTexture_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=NULL
	});

	// sprite renderer (1)
	EntitySystem_RegisterComponentBuiltin(EC_SPRITE_RENDERER,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECSpriteRenderer)
		,.required_components	=ECSpriteRenderer_RequiredComponents()
		,.EComponent_Setup		=ECSpriteRenderer_Setup
		,.EComponent_Update		=ECSpriteRenderer_Update
		,.EComponent_Destroy	=ECSpriteRenderer_Destroy
	});


	// text box renderer (1)
	EntitySystem_RegisterComponentBuiltin(EC_TEXTBOX_RENDERER,(EntitySystemRegisterEComponent){
		.size_data				=sizeof(ECTextBoxRenderer)
		,.required_components	=ECTextBoxRenderer_RequiredComponents()
		,.EComponent_Setup		=ECTextBoxRenderer_Setup
		,.EComponent_Update		=ECTextBoxRenderer_Update
		,.EComponent_Destroy	=ECTextBoxRenderer_Destroy
	});


	// check component consistency
	 min_iter=MIN(g_entity_system_registered_components->count,EC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		EntitySystemRegisteredEComponentData *component=g_entity_system_registered_components->items[i];
		if(component->id != i){
			Log_Error("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	g_entity_system_user_can_register_components=true;
	return true;
}

int	EntitySystem_RegisterComponent(EntitySystemRegisterEComponent es_component_register){
	int idx_component=0;

	if(g_entity_system_registered_components != NULL){
		idx_component=g_entity_system_registered_components->count;
	}

	if(EntitySystem_RegisterComponentBuiltin(idx_component,es_component_register)==false){
		return EC_INVALID;
	}
	return idx_component;
}

size_t					EntitySystem_NumComponents(void){
	if(g_entity_system_registered_components != NULL){
		return g_entity_system_registered_components->count;
	}
	return 0;
}

void EntitySystem_DeInit(void){
	List_DeleteAndFreeAllItems(g_entity_system_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS
EntitySystem *EntitySystem_New(void){
	EntitySystem *system=ZG_NEW(EntitySystem);
	EntitySystemData *data=ZG_NEW(EntitySystemData);
	data->lst_entity_managers=List_New();
	data->map_entity_managers=MapString_New();

	data->components=malloc(sizeof(EntitySystemEComponentData)*g_entity_system_registered_components->count);
	memset(data->components,0,sizeof(EntitySystemEComponentData)*g_entity_system_registered_components->count);

	for(unsigned i=0; i < g_entity_system_registered_components->count;i++){
		data->components[i]=ZG_NEW(EntitySystemEComponentData);
	}

	system->data=data;

	// after first system is created, user cannot register any component anymore
	g_entity_system_user_can_register_components=false;

	return system;
}

int EntitySystem_OrderComponents(const void *_a, const void *_b){
	EComponent *a, *b;
	a = (EComponent *) _a;
	b = (EComponent *) _b;
	return (*a - *b);
}

EComponent *EntitySystem_GenerateComponentRequirementList(EComponent *in_data, size_t in_len, size_t *out_len){
	// check number of whether there's no component
	List *list=List_New();

	for(unsigned i=0; i < in_len;i++){
		EComponent entity_component=in_data[i];
		EntitySystemRegisteredEComponentData *registered_component=g_entity_system_registered_components->items[entity_component];
		EComponentList req_com=registered_component->data.required_components;
		bool found=false;
		for(uint16_t j=0; j < req_com.n_components;j++){
			// check if exists...
			EComponent idx_component_req_to_find=req_com.components[j];

			found=false;
			// check whether it's int the list or not
			for(uint16_t k=0; k < list->count && found==false;k++){
				EComponent idx_component_in_list=(intptr_t)list->items[k];
				if(idx_component_req_to_find == idx_component_in_list){
					found=true;
				}
			}

			if(found==false){
				List_Add(list,(void *)((intptr_t)idx_component_req_to_find));
			}
		}

		// add the component if not exist
		if(list != NULL){ // add the component itself
			found=false;
			for(uint16_t k=0; k < list->count && found==false;k++){
				intptr_t idx_component_in_list=(intptr_t)list->items[k];
				if(entity_component == idx_component_in_list){
					found=true;
				}
			}

			if(found==false){
				List_Add(list,(void *)((intptr_t)entity_component));
			}
		}
	}

	EComponent *new_data=(EComponent *)malloc(sizeof(EComponent)*list->count);
	for(uint16_t k=0; k < list->count;k++){
		new_data[k]=(intptr_t)list->items[k];
	}

	*out_len=list->count;

	qsort(new_data,list->count,sizeof(EComponent),EntitySystem_OrderComponents);

	List_Delete(list);

	return new_data;
}
/*
Entity *EntitySystem_NewEntity(EntitySystem *_this,EComponent *_entity_components, size_t _entity_components_len){
	EntitySystemData *data=_this->data;
	char _entity_manager_id[1024]="__@_comp";
	size_t entity_components_len;

	// 1. check component requirements
	EComponent *entity_components=EntitySystem_GenerateComponentRequirementList(_entity_components,_entity_components_len,&entity_components_len);

	// internal type that is generated according used components...
	for(uint16_t i=0; i < entity_components_len; i++){
		char *num=StrUtils_IntToStr(entity_components[i]);
		strcat(_entity_manager_id,"_");
		strcat(_entity_manager_id,num);
		free(num);
	}

	strcat(_entity_manager_id,"_@__");

	EntityManagerData *entity_manager_data=MapString_GetValue(data->map_entity_managers,_entity_manager_id,NULL);

	if(entity_manager_data == NULL){ // create ...
		entity_manager_data=(EntityManagerData *)EntitySystem_NewEntityManager(_this,_entity_manager_id ,UNLIMITIED_ENTITIES, entity_components, entity_components_len);
	}

	// Free because we don't need it anymore
	ZG_FREE(entity_components);
	return EntitySystem_NewEntityFromManager(_this,_entity_manager_id);
}*/
/*
Entity  *EntitySystem_NewEntityFromManager(EntitySystem *_this,const char *_id){
	EntitySystemData *data=_this->data;
	EntityManagerData *entity_manager_data=MapString_GetValue(data->map_entity_managers,_id,NULL);

	if(entity_manager_data == NULL){
		Log_Error("Entity manager '%s' not exist",_id);
		return NULL;
	}

	Entity *entity=NULL;
	if(
			entity_manager_data->active_entities>=entity_manager_data->n_entities
								&&
			entity_manager_data->active_entities<entity_manager_data->max_entities
	){ // extend entity
		EntitySystem_ExtendEntities(_this,entity_manager_data,1);
	}else{
		return NULL;
	}

	entity=entity_manager_data->entities[entity_manager_data->active_entities];
	entity_manager_data->active_entities++;

	entity->active=true;

	return entity;
}*/

void EntitySystem_ExtendComponent(EntitySystem *_this,EComponent idx_component, Entity ** entities, size_t extend){

	if(idx_component >= g_entity_system_registered_components->count){
		return;
	}

	EntitySystemData *data=_this->data;
	EntitySystemEComponentData *component_data=data->components[idx_component];
	EntitySystemRegisterEComponent  registered_component_data=((EntitySystemRegisteredEComponentData *)g_entity_system_registered_components->items[idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	void *old_ptr=component_data->ptr_data;
	uint8_t *ptr_com=NULL;
	Entity **ptr_entities=NULL;
	size_t size_component_data=registered_component_data.size_data;

	// do a realloc...
	component_data->ptr_data=malloc(n_new_elements*registered_component_data.size_data);
	memset(component_data->ptr_data,0,n_new_elements*registered_component_data.size_data);

	if(old_ptr != NULL){
		memcpy(component_data->ptr_data,old_ptr,component_data->n_elements*registered_component_data.size_data);

		// update new pointers
		ptr_entities=entities;
		ptr_com=component_data->ptr_data;
		for(unsigned i=0; i < component_data->n_elements; i++){
			(*ptr_entities)->components[idx_component]=ptr_com;
			ptr_com+=size_component_data;
			ptr_entities++;
		}
		ZG_FREE(old_ptr);
	}

	// initialize new components
	ptr_entities=entities+component_data->n_elements;
	ptr_com=component_data->ptr_data+component_data->n_elements*size_component_data;
	for(unsigned i=component_data->n_elements; i < n_new_elements; i++){
		registered_component_data.EComponent_Setup(ptr_com,i,*ptr_entities);
		ptr_com+=size_component_data;
		ptr_entities++;
	}

	// update new elements
	component_data->n_elements=n_new_elements;

}

void EntitySystem_ExtendEntities(EntitySystem *_this, EntityManagerData *entity_manager_data, size_t extend_entities){

	size_t total_extend=entity_manager_data->n_entities+extend_entities;

	if(total_extend >= entity_manager_data->max_entities){
		Log_Error("cannot extend entity type '%s' up to '%i': Max entities reached (max: %i)",entity_manager_data->name,extend_entities,entity_manager_data->max_entities);
	}

	Entity **old_ptr=entity_manager_data->entities;

	entity_manager_data->entities=malloc(sizeof(Entity *)*total_extend);
	memset(entity_manager_data->entities,0,sizeof(Entity *)*total_extend);

	// copy old ones
	if(old_ptr != NULL){
		memcpy(entity_manager_data->entities,old_ptr,sizeof(Entity *)*entity_manager_data->n_entities);
		ZG_FREE(old_ptr);
	}

	// extend as many entities we need
	for(unsigned i=entity_manager_data->n_entities; i < total_extend;i++){
		entity_manager_data->entities[i]=Entity_New(_this);
	}

	// extend components
	for(unsigned i=0; i < entity_manager_data->n_components;i++){
		EComponent idx_ec=entity_manager_data->entity_components[i];
		// extend components this type will have
		EntitySystem_ExtendComponent(
			_this
			,idx_ec
			,entity_manager_data->entities+entity_manager_data->n_entities
			,extend_entities
		);
	};

	// realloc set all entity types as this type
	entity_manager_data->n_entities=total_extend;
}

EntityManager * EntitySystem_NewEntityManager(
	EntitySystem *_this
	, const char *_str_entity_manager
	, uint16_t _max_entities
	, EComponent *_entity_components
	, size_t _entity_components_len
){
	EntitySystemData *data=_this->data;
	EntityManagerData *entity_manager_data=ZG_NEW(EntityManagerData);
	EntityManager *entity_manager=ZG_NEW(EntityManager);
	entity_manager->data=entity_manager_data;
	size_t	req_entity_components_len=0;

	if(_entity_components_len == 0){
		return NULL;
	}
	// get required components (it has to free after use)
	EComponent *req_entity_components=EntitySystem_GenerateComponentRequirementList(
			_entity_components
			,_entity_components_len
			,&req_entity_components_len);

	// check and extend required components if needed ...
	entity_manager_data->entity_components=malloc(sizeof(EComponent)*req_entity_components_len);
	memset(entity_manager_data->entity_components,0,sizeof(EComponent)*req_entity_components_len);
	entity_manager_data->n_components=req_entity_components_len;

	//entity_manager_data->es_system=_this;
	entity_manager_data->n_entities=0;
	entity_manager_data->max_entities=0;
	entity_manager_data->active_entities=0;

	entity_manager_data->name=malloc(strlen(_str_entity_manager)*sizeof(char)+1);
	memset(entity_manager_data->name,0,strlen(_str_entity_manager)*sizeof(char)+1);
	strcpy(entity_manager_data->name,_str_entity_manager);

	EComponent *dst_ptr=entity_manager_data->entity_components;
	EComponent *src_ptr=req_entity_components;

	// add components neededs for the type...
	for(unsigned i=0; i < req_entity_components_len;i++){
		// register the components this type will have
		*dst_ptr++=*src_ptr++;
	}

	List_Add(data->lst_entity_managers,entity_manager);
	MapString_SetValue(data->map_entity_managers,_str_entity_manager,entity_manager);

	// extend entities
	entity_manager_data->max_entities=_max_entities;
	if(_max_entities != (uint16_t)UNLIMITIED_ENTITIES){
		EntitySystem_ExtendEntities(
			_this
			,entity_manager_data
			,_max_entities
		);
	}

	ZG_FREE(req_entity_components);
	return entity_manager;
}

void EntitySystem_Update(EntitySystem * _this){
	EntitySystemData *data=(EntitySystemData *)_this->data;
	EntitySystemRegisteredEComponentData  **ptr_registered_component_data=(EntitySystemRegisteredEComponentData  **)g_entity_system_registered_components->items;
	EntitySystemEComponentData **components=data->components;
	EntitySystemEComponentData *component=NULL;
	uint8_t *ptr_component_data=NULL;
	size_t size_data=0;
	//EComponentHeader * component_data=NULL;


	for(unsigned i=0; i < g_entity_system_registered_components->count; i++){
		void (*EComponent_Update)(void *) =ptr_registered_component_data[i]->data.EComponent_Update;
		if(EComponent_Update != NULL){
			size_data=ptr_registered_component_data[i]->data.size_data;
			component=*components;
			ptr_component_data=component->ptr_data;
			for(unsigned i=0; i < component->n_elements; i++){
				if(((EComponentHeader * )ptr_component_data)->entity->active){
					EComponent_Update(ptr_component_data);
				}
				ptr_component_data+=size_data;
			}
		}
		ptr_registered_component_data++;
		components++;
	}
}

void EntitySystem_Delete(EntitySystem *_this){
	EntitySystemData *data=(EntitySystemData *)_this->data;
	EntitySystemRegisteredEComponentData  **ptr_registered_component_data=(EntitySystemRegisteredEComponentData  **)g_entity_system_registered_components->items;
	EntitySystemEComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];
	MapStringIterator *iterator=MapStringIterator_New(data->map_entity_managers);

	for(; !MapStringIterator_End(iterator);MapStringIterator_Next(iterator)){
		//const char *key=MapStringIterator_GetKey(iterator);
		EntityManager *entity_manager=MapStringIterator_GetValue(iterator);
		EntityManagerData *entity_manager_data=entity_manager->data;

		for(unsigned i=0; i < entity_manager_data->n_entities; i++){
			Entity_Delete(entity_manager_data->entities[i]);
		}

		ZG_FREE(entity_manager_data->entities);
		ZG_FREE(entity_manager_data->entity_components);
		ZG_FREE(entity_manager_data->name);
		ZG_FREE(entity_manager_data);
		ZG_FREE(entity_manager);
	}

	MapStringIterator_Delete(iterator);

	// release all entities
	for(unsigned i=0; i < g_entity_system_registered_components->count; i++){
		uint8_t *ptr_data=(*component_data)->ptr_data;
		void (*EComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.EComponent_Destroy;
		size_t size_data=(*ptr_registered_component_data)->data.size_data;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(EComponent_Destroy != NULL){
					EComponent_Destroy(ptr_data);
				}
				ptr_data+=size_data;
			}
			ZG_FREE((*component_data)->ptr_data);
		}
		ptr_registered_component_data++;
		component_data++;
	}

	List_Delete(data->lst_entity_managers);
	MapString_Delete(data->map_entity_managers);

	for(unsigned i=0; i < g_entity_system_registered_components->count;i++){
		ZG_FREE(data->components[i]);
	}

	ZG_FREE(data->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
