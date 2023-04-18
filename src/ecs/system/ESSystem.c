#include "ecs/zg_ecs.h"

List *g_es_system_registered_components=NULL;
bool  g_user_can_register_components=true;

typedef struct{
	EComponent id;
	ESSystemRegisterEComponent data;
}ESSystemRegisteredEComponentData;

typedef struct{
	void 	**ptr_data; // we want to keep pointer in order to support future reallocs
	size_t 	n_elements;
}ESSystemEComponentData;

typedef struct {
	char 		*name; // name entity type
	EComponent *entity_components;// it says the components it has this entity
	size_t 		n_components; // available components

	uint16_t 	max_entities; // max entitites of this type (default 1)
	uint16_t 	n_entities;
	uint16_t 	active_entities;
	Entity 		**entities;

}EntityManagerData;

typedef struct{
	MapString *map_entity_managers;
	List *lst_entity_managers;
	ESSystemEComponentData **components;//[ENTITY_COMPONENT_MAX];
}ESSystemData;

//---------------------------------------------------
// PRIVATE FUNCTIONS
void 	ESSystem_ExtendEntities(ESSystem *_this,EntityManagerData *entity_manager_data, size_t extend_entities);

bool	ESSystem_RegisterComponentBuiltin(EComponent _idx_component,ESSystemRegisterEComponent es_component_register){

	if(g_user_can_register_components==false){
		Log_ErrorF("Components should registered before create any Entity-System");
		return false; //
	}

	if(g_es_system_registered_components == NULL){
		g_es_system_registered_components=List_New();
	}

	EComponent idx_component=_idx_component;//g_es_system_registered_components->count;
	ESSystemRegisteredEComponentData *new_component_register=ZG_NEW(ESSystemRegisteredEComponentData);
	new_component_register->data=es_component_register;
	new_component_register->id=idx_component;
	List_Add(g_es_system_registered_components,new_component_register);

	return true;
}


//---------------------------------------------------
// STATIC FUNCTIONS
bool ESSystem_Init(void){

	unsigned min_iter=0;

	// invalid (0)
	ESSystem_RegisterComponentBuiltin(EC_INVALID,(ESSystemRegisterEComponent){
		.size_data				=0
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=NULL
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=NULL
	});

	// transform
	ESSystem_RegisterComponentBuiltin(EC_TRANSFORM,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECTransform)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECTransform_Setup
		,.EComponent_Update		=ECTransform_Update
		,.EComponent_Destroy	=ECTransform_Destroy
	});

	// Animation transform
	ESSystem_RegisterComponentBuiltin(EC_TRANSFORM_ANIMATION,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECTransformAnimation)
		,.required_components	=ECTransformAnimation_RequiredComponents()
		,.EComponent_Setup		=ECTransformAnimation_Setup
		,.EComponent_Update		=ECTransformAnimation_Update
		,.EComponent_Destroy	=ECTransformAnimation_Destroy
	});

	// geometry
	ESSystem_RegisterComponentBuiltin(EC_GEOMETRY,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECGeometry)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECGeometry_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=ECGeometry_Destroy
	});

	// material
	ESSystem_RegisterComponentBuiltin(EC_MATERIAL,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECMaterial)
		,.required_components	= (EComponentList){0,0}
		,.EComponent_Setup		=ECMaterial_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=ECMaterial_Destroy
	});

	// material animation
	ESSystem_RegisterComponentBuiltin(EC_MATERIAL_ANIMATION,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECMaterialAnimation)
		,.required_components	= (EComponentList){0,0}
		,.EComponent_Setup		=ECMaterialAnimation_Setup
		,.EComponent_Update		=ECMaterialAnimation_Update
		,.EComponent_Destroy	=ECMaterialAnimation_Destroy
	});

	// texture
	ESSystem_RegisterComponentBuiltin(EC_TEXTURE,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECTexture)
		,.required_components	=(EComponentList){0,0}
		,.EComponent_Setup		=ECTexture_Setup
		,.EComponent_Update		=NULL
		,.EComponent_Destroy	=NULL
	});

	// sprite renderer (1)
	ESSystem_RegisterComponentBuiltin(EC_SPRITE_RENDERER,(ESSystemRegisterEComponent){
		.size_data				=sizeof(ECSpriteRenderer)
		,.required_components	=ECSpriteRenderer_RequiredComponents()
		,.EComponent_Setup		=ECSpriteRenderer_Setup
		,.EComponent_Update		=ECSpriteRenderer_Update
		,.EComponent_Destroy	=ECSpriteRenderer_Destroy
	});



	// check component consistency
	 min_iter=MIN(g_es_system_registered_components->count,EC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		ESSystemRegisteredEComponentData *component=g_es_system_registered_components->items[i];
		if(component->id != i){
			Log_Error("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	g_user_can_register_components=true;
	return true;
}

EComponent	ESSystem_RegisterComponent(ESSystemRegisterEComponent es_component_register){
	EComponent idx_component=0;

	if(g_es_system_registered_components != NULL){
		idx_component=g_es_system_registered_components->count;
	}

	if(ESSystem_RegisterComponentBuiltin(idx_component,es_component_register)==false){
		return EC_INVALID;
	}
	return idx_component;
}

size_t					ESSystem_NumComponents(void){
	if(g_es_system_registered_components != NULL){
		return g_es_system_registered_components->count;
	}
	return 0;
}

void ESSystem_DeInit(void){
	List_DeleteAndFreeAllItems(g_es_system_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS
ESSystem *ESSystem_New(void){
	ESSystem *system=ZG_NEW(ESSystem);
	ESSystemData *data=ZG_NEW(ESSystemData);
	data->lst_entity_managers=List_New();
	data->map_entity_managers=MapString_New();

	data->components=malloc(sizeof(ESSystemEComponentData)*g_es_system_registered_components->count);
	memset(data->components,0,sizeof(ESSystemEComponentData)*g_es_system_registered_components->count);

	for(unsigned i=0; i < g_es_system_registered_components->count;i++){
		data->components[i]=ZG_NEW(ESSystemEComponentData);
	}

	system->data=data;

	// after first system is created, user cannot register any component anymore
	g_user_can_register_components=false;

	return system;
}

int ESSystem_OrderComponents(const void *_a, const void *_b){
	EComponent *a, *b;
	a = (EComponent *) _a;
	b = (EComponent *) _b;
	return (*a - *b);
}

EComponent *ESSystem_GenerateComponentRequirementList(EComponent *in_data, size_t in_len, size_t *out_len){
	// check number of whether there's no component
	List *list=List_New();

	for(unsigned i=0; i < in_len;i++){
		EComponent entity_component=in_data[i];
		ESSystemRegisteredEComponentData *registered_component=g_es_system_registered_components->items[entity_component];
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

	qsort(new_data,list->count,sizeof(EComponent),ESSystem_OrderComponents);

	List_Delete(list);

	return new_data;
}

Entity *ESSystem_NewEntity(ESSystem *_this,EComponent *_entity_components, size_t _entity_components_len){
	ESSystemData *data=_this->data;
	char _str_entity_manager[1024]="__@_comp";
	size_t entity_components_len;

	// 1. check component requirements
	EComponent *entity_components=ESSystem_GenerateComponentRequirementList(_entity_components,_entity_components_len,&entity_components_len);

	// internal type that is generated according used components...
	for(uint16_t i=0; i < entity_components_len; i++){
		char *num=StrUtils_IntToStr(entity_components[i]);
		strcat(_str_entity_manager,"_");
		strcat(_str_entity_manager,num);
		free(num);
	}

	strcat(_str_entity_manager,"_@__");

	EntityManagerData *entity_manager_data=MapString_GetValue(data->map_entity_managers,_str_entity_manager,NULL);

	if(entity_manager_data == NULL){ // create ...
		entity_manager_data=(EntityManagerData *)ESSystem_NewEntityManager(_this,_str_entity_manager ,UNLIMITIED_ENTITIES, entity_components, entity_components_len);
	}

	// Free because we don't need it anymore
	ZG_FREE(entity_components);
	return ESSystem_NewEntityFromManager(_this,_str_entity_manager);
}

Entity  *ESSystem_NewEntityFromManager(ESSystem *_this,const char *_str_entity_manager){
	ESSystemData *data=_this->data;
	EntityManagerData *entity_manager_data=MapString_GetValue(data->map_entity_managers,_str_entity_manager,NULL);

	if(entity_manager_data == NULL){
		Log_Error("Entity type %s not exist",_str_entity_manager);
		return NULL;
	}

	Entity *entity=NULL;
	if(entity_manager_data->active_entities>=entity_manager_data->n_entities && entity_manager_data->active_entities<entity_manager_data->max_entities){ // extend entity
		ESSystem_ExtendEntities(_this,entity_manager_data,1);
	}else{
		return NULL;
	}

	entity=entity_manager_data->entities[entity_manager_data->active_entities];
	entity_manager_data->active_entities++;

	entity->active=true;

	return entity;
}

void ESSystem_ExtendComponent(ESSystem *_this,EComponent idx_component, Entity ** entities, size_t extend){

	if(idx_component >= g_es_system_registered_components->count){
		return;
	}

	ESSystemData *data=_this->data;
	ESSystemEComponentData *component_data=data->components[idx_component];
	ESSystemRegisterEComponent  registered_component_data=((ESSystemRegisteredEComponentData *)g_es_system_registered_components->items[idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	void **old_ptr=component_data->ptr_data;
	void **ptr_com=NULL;

	// do a realloc...
	component_data->ptr_data=malloc(sizeof(void *)*n_new_elements);

	if(old_ptr != NULL){
		memcpy(component_data->ptr_data,old_ptr,sizeof(void *)*component_data->n_elements);
		ZG_FREE(old_ptr);
	}

	// initialize new components
	ptr_com=component_data->ptr_data + component_data->n_elements;
	for(unsigned i=component_data->n_elements; i < n_new_elements; i++){
		void *new_data=malloc(registered_component_data.size_data);
		registered_component_data.EComponent_Setup(new_data,*entities++);
		*ptr_com++=new_data;//=registered_component_data.size_data;
	}

	// update new elements
	component_data->n_elements=n_new_elements;

}

void ESSystem_ExtendEntities(ESSystem *_this,EntityManagerData *entity_manager_data, size_t extend_entities){
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
		entity_manager_data->entities[i]=Entity_New();
	}

	// extend components
	for(unsigned i=0; i < entity_manager_data->n_components;i++){
		EComponent idx_ec=entity_manager_data->entity_components[i];
		// extend components this type will have
		ESSystem_ExtendComponent(_this,idx_ec,entity_manager_data->entities+entity_manager_data->n_entities,extend_entities);
	};

	// realloc set all entity types as this type
	entity_manager_data->n_entities=total_extend;
}

void * ESSystem_NewEntityManager(ESSystem *_this
		, const char *_str_entity_manager
		, uint16_t max_entities
		, EComponent *entity_components
		, size_t entity_components_len
	){
	ESSystemData *data=_this->data;
	EntityManagerData *entity_manager_data=ZG_NEW(EntityManagerData);

	if(entity_components_len == 0){
		return NULL;
	}

	// check and extend required components if needed ...
	entity_manager_data->entity_components=malloc(sizeof(EComponent)*entity_components_len);
	memset(entity_manager_data->entity_components,0,sizeof(EComponent)*entity_components_len);
	entity_manager_data->n_components=entity_components_len;

	//entity_manager_data->es_system=_this;
	entity_manager_data->n_entities=0;
	entity_manager_data->max_entities=0;
	entity_manager_data->active_entities=0;

	entity_manager_data->name=malloc(strlen(_str_entity_manager)*sizeof(char)+1);
	memset(entity_manager_data->name,0,strlen(_str_entity_manager)*sizeof(char)+1);
	strcpy(entity_manager_data->name,_str_entity_manager);

	EComponent *dst_ptr=entity_manager_data->entity_components;
	EComponent *src_ptr=entity_components;

	// add components neededs for the type...
	for(unsigned i=0; i < entity_components_len;i++){
		// register the components this type will have
		*dst_ptr++=*src_ptr++;
	}

	List_Add(data->lst_entity_managers,entity_manager_data);
	MapString_SetValue(data->map_entity_managers,_str_entity_manager,entity_manager_data);

	// extend entities
	entity_manager_data->max_entities=max_entities;
	if(max_entities != (uint16_t)UNLIMITIED_ENTITIES){
		ESSystem_ExtendEntities(
			_this
			,entity_manager_data
			,max_entities
		);
	}
	return entity_manager_data;
}

void ESSystem_Update(ESSystem * _this){
	ESSystemData *data=(ESSystemData *)_this->data;
	ESSystemRegisteredEComponentData  **ptr_registered_component_data=(ESSystemRegisteredEComponentData  **)g_es_system_registered_components->items;
	ESSystemEComponentData **component_data=data->components;

	for(unsigned i=0; i < g_es_system_registered_components->count; i++){
		void (*EComponent_Update)(void *) =(*ptr_registered_component_data)->data.EComponent_Update;
		if(EComponent_Update != NULL){
			EComponentHeader **ptr_data=(EComponentHeader **)(*component_data)->ptr_data;
			for(unsigned i=0; i < (*component_data)->n_elements; i++){
				EComponentHeader * data=*ptr_data;
				if(data->entity->active){
					EComponent_Update(data);
				}
				ptr_data++;
			}
		}
		ptr_registered_component_data++;
		component_data++;
	}
}

void ESSystem_Delete(ESSystem *_this){
	ESSystemData *data=(ESSystemData *)_this->data;
	ESSystemRegisteredEComponentData  **ptr_registered_component_data=(ESSystemRegisteredEComponentData  **)g_es_system_registered_components->items;
	ESSystemEComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];
	MapStringIterator *iterator=MapStringIterator_New(data->map_entity_managers);

	for(; !MapStringIterator_End(iterator);MapStringIterator_Next(iterator)){
		//const char *key=MapStringIterator_GetKey(iterator);
		EntityManagerData *entity_manager_data=MapStringIterator_GetValue(iterator);

		for(unsigned i=0; i < entity_manager_data->n_entities; i++){
			Entity_Delete(entity_manager_data->entities[i]);
		}

		ZG_FREE(entity_manager_data->entities);
		ZG_FREE(entity_manager_data->entity_components);
		ZG_FREE(entity_manager_data->name);
		ZG_FREE(entity_manager_data);
	}

	MapStringIterator_Delete(iterator);

	// release all entities
	for(unsigned i=0; i < g_es_system_registered_components->count; i++){
		void **ptr_data=(*component_data)->ptr_data;
		void (*EComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.EComponent_Destroy;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(EComponent_Destroy != NULL){
					EComponent_Destroy(*ptr_data);
				}
				ZG_FREE(*ptr_data++);
			}
			ZG_FREE((*component_data)->ptr_data);
		}
		ptr_registered_component_data++;
		component_data++;
	}

	List_Delete(data->lst_entity_managers);
	MapString_Delete(data->map_entity_managers);

	for(unsigned i=0; i < g_es_system_registered_components->count;i++){
		ZG_FREE(data->components[i]);
	}

	ZG_FREE(data->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
