#include "../_zg_ecs_.h"

ZG_List *g_entity_manager_registered_components=NULL;
ZG_MapString *g_map_entity_manager_registered_components=NULL;
bool  g_entity_manager_user_can_register_components=true;

ZG_List *g_entity_manager_registered=NULL;

typedef struct{
	ZG_EComponent id;
	ZG_RegisterComponent data;
}ZG_EntitySystemRegisteredComponentData;

typedef struct{
	uint8_t	*ptr_data; // we want to keep pointer in order to support future reallocs
	size_t  	n_elements;
}ZG_EntitySystemComponentData;


typedef struct{
	ZG_MapString *map_archetypes;
	ZG_List *lst_archetypes;
	ZG_EntitySystemComponentData **components;//[ENTITY_COMPONENT_MAX];
}ZG_EntitySystemData;

//---------------------------------------------------
// PRIVATE FUNCTIONS

bool	ZG_EntitySystem_RegisterComponent(
		const char *_name
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
		,void   (* _on_update)(void *_component_data, ZG_Entity _entity) // function component
		,void   (* _on_create)(void *_component_data, ZG_Entity _entity) // set it up if component need to init or allocate resources on its creation
		,void   (* _on_destroy)(void *_component_data, ZG_Entity _entity)
		,ZG_ComponentList   (*_required_components)(void) // function to destroy
){

	if(g_entity_manager_user_can_register_components==false){
		ZG_LOG_ERRORF("Components should registered before create any ZG_Entity-System");
		return false; //
	}

	if(g_entity_manager_registered_components == NULL){
		g_entity_manager_registered_components=ZG_List_New();
		g_map_entity_manager_registered_components=ZG_Map_New();
	}

	ZG_EComponent idx_component=g_entity_manager_registered_components->count;//g_entity_manager_registered_components->count;
	ZG_EntitySystemRegisteredComponentData *new_component_register=ZG_NEW(ZG_EntitySystemRegisteredComponentData);
	new_component_register->data=(ZG_RegisterComponent){
		.name=_name
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,.size_data=	_size_data // len data component
		//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
		,.on_update=_on_update // function component
		,.on_create=_on_create
		,.on_destroy=_on_create
		,.required_components=_required_components
	}
	new_component_register->id=idx_component;
	ZG_List_Add(g_entity_manager_registered_components,new_component_register);
	ZG_Map_Set(g_map_entity_manager_registered_components,new_component_register,g_entity_manager_registered_components->count);

	return true;
}


//---------------------------------------------------
// STATIC FUNCTIONS
bool ZG_EntitySystem_Init(void){

	unsigned min_iter=0;

	if(ZG_ECS_ADD_COMPONENT(
		ZG_ECTransform
		,ZG_ECTransform_OnUpdate
		,ZG_ECTransform_OnCreate
		,ZG_ECTransform_OnDestroy
		,NULL
	)==false){
		return false;
	}

	// invalid (0)
	/*ZG_ZG_EntitySystem_RegisterComponentBuiltin(ZG_EC_INVALID,(ZG_ESRegisterComponent){
		.size_data				=0
		,.required_components	=(ZG_EComponentList){0,0}
		,.EComponent_Setup		=NULL
		,.ZG_EComponent_Update		=NULL
		,.ZG_EComponent_Destroy	=NULL
	});*/

	//ZG_ECS_REGISTER_COMPONENT(ZG_ECTransform,ZG_ECTransform_Update,ZG_ECTransform_Setup,ZG_ECTransform_Destroy);

	// transform
	/*ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_TRANSFORM,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECTransform)
		,.required_components	=(ZG_EComponentList){0,0}
		,.EComponent_Setup		=ZG_ECTransform_Setup
		,.ZG_EComponent_Update		=ZG_ECTransform_Update
		,.ZG_EComponent_Destroy	=ZG_ECTransform_Destroy
	});*/


	// geometry
	/*ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_GEOMETRY,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECGeometry)
		,.required_components	=(ZG_EComponentList){0,0}
		,.EComponent_Setup		=ZG_ECGeometry_Setup
		,.ZG_EComponent_Update		=NULL
		,.ZG_EComponent_Destroy	=ZG_ECGeometry_Destroy
	});

	// material
	ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_MATERIAL,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECMaterial)
		,.required_components	= (ZG_EComponentList){0,0}
		,.EComponent_Setup		=ZG_ECMaterial_Setup
		,.ZG_EComponent_Update		=NULL
		,.ZG_EComponent_Destroy	=ZG_ECMaterial_Destroy
	});

	// texture
	ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_TEXTURE,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECTexture)
		,.required_components	=(ZG_EComponentList){0,0}
		,.EComponent_Setup		=ZG_ECTexture_Setup
		,.ZG_EComponent_Update		=NULL
		,.ZG_EComponent_Destroy	=NULL
	});

	// sprite renderer (1)
	ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_SPRITE_RENDERER,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECSpriteRenderer)
		,.required_components	=ZG_ECSpriteRenderer_RequiredComponents()
		,.EComponent_Setup		=ZG_ECSpriteRenderer_Setup
		,.ZG_EComponent_Update		=ZG_ECSpriteRenderer_Update
		,.ZG_EComponent_Destroy	=ZG_ECSpriteRenderer_Destroy
	});


	// text box renderer (1)
	ZG_ZG_EntitySystem_RegisterComponentBuiltin(EC_TEXTBOX_RENDERER,(ZG_ESRegisterComponent){
		.size_data				=sizeof(ZG_ECTextBoxRenderer)
		,.required_components	=ZG_ECTextBoxRenderer_RequiredComponents()
		,.EComponent_Setup		=ZG_ECTextBoxRenderer_Setup
		,.ZG_EComponent_Update		=ZG_ECTextBoxRenderer_Update
		,.ZG_EComponent_Destroy	=ZG_ECTextBoxRenderer_Destroy
	});


	// check component consistency
	 min_iter=MIN(g_entity_manager_registered_components->count,EC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		ZG_EntitySystemRegisteredComponentData *component=g_entity_manager_registered_components->items[i];
		if(component->id != i){
			ZG_LOG_ERROR("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	g_entity_manager_user_can_register_components=true;*/
	return true;
}

void ZG_EntitySystem_ExtendComponent(ZG_EntitySystem *_this,ZG_EComponent _idx_component, size_t extend){


	// new behaviour

	// TODO
	// 1. Call on_destroy of all required components
	// 2. re/allocate new array * sizeof(component).
	// 3. Call on_create
	// 4. re/allocate new array of indexs for reference components that they changed its position
	// 5. Reset reference indexs

}

bool ZG_EntitySystem_RegisterComponent(
	const char *_component_id,
	size_t __type_data
	,const char * _required_components_ids[]
	,void   (*_on_update)(void *_component_data)
	,void   (*_on_create)(void *_component_data)
	,void   (*_on_destroy)(void *_component_data)
){

	if(g_entity_manager_user_can_register_components==false){
		ZG_LOG_ERRORF("Components should registered before create any ZG_Entity-System");
		return false; //
	}

	if(g_entity_manager_registered_components == NULL){
		g_entity_manager_registered_components=ZG_Lis_New();
		g_map_entity_manager_registered_components=ZG_MapString_New();
	}

	//ZG_EComponent idx_component=_idx_component;//g_entity_manager_registered_components->count;
	ZG_RegisterComponent *new_component_register=ZG_NEW(ZG_RegisterComponent);

	*new_component_register=(ZG_RegisterComponent){
		.component_id=g_entity_manager_num_registered_component++;
		.name=_name;
		.size_data=_size_data;
		.on_update=_on_update;
		.on_create=_on_create;
		.on_destroy=_on_destroy;
	};




	ZG_MapString_Set(g_map_entity_manager_registered_components,_name,new_component_register);
	ZG_Lis_New(g_entity_manager_registered_components,new_component_register);
	return true;

}
/*
int	ZG_EntitySystem_RegisterComponent(ZG_ESRegisterComponent es_component_register){
	int idx_component=0;

	if(g_entity_manager_registered_components != NULL){
		idx_component=g_entity_manager_registered_components->count;
	}

	if(ZG_ZG_EntitySystem_RegisterComponentBuiltin(idx_component,es_component_register)==false){
		return ZG_EC_INVALID;
	}
	return idx_component;
}*/

size_t					ZG_EntitySystem_NumComponents(void){
	if(g_entity_manager_registered_components != NULL){
		return g_entity_manager_registered_components->count;
	}
	return 0;
}

void ZG_EntitySystem_DeInit(void){
	ZG_List_DeleteAndFreeAllItems(g_entity_manager_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_EntitySystem *ZG_EntitySystem_New(void){
	ZG_EntitySystem *system=ZG_NEW(ZG_EntitySystem);
	ZG_EntitySystemData *data=ZG_NEW(ZG_EntitySystemData);
	data->lst_archetypes=ZG_List_New();
	data->map_archetypes=ZG_MapString_New();

	data->components=malloc(sizeof(ZG_EntitySystemComponentData)*g_entity_manager_registered_components->count);
	memset(data->components,0,sizeof(ZG_EntitySystemComponentData)*g_entity_manager_registered_components->count);

	for(unsigned i=0; i < g_entity_manager_registered_components->count;i++){
		data->components[i]=ZG_NEW(ZG_EntitySystemComponentData);
	}

	system->data=data;

	// after first system is created, user cannot register any component anymore
	g_entity_manager_user_can_register_components=false;

	return system;
}

int ZG_EntitySystem_OrderComponents(const void *_a, const void *_b){
	ZG_EComponent *a, *b;
	a = (ZG_EComponent *) _a;
	b = (ZG_EComponent *) _b;
	return (*a - *b);
}

ZG_EComponent *ZG_EntitySystem_GenerateComponentRequirementList(ZG_EComponent *in_data, size_t in_len, size_t *out_len){
	// check number of whether there's no component
	ZG_List *list=ZG_List_New();

	for(unsigned i=0; i < in_len;i++){
		ZG_EComponent entity_component=in_data[i];
		ZG_EntitySystemRegisteredComponentData *registered_component=g_entity_manager_registered_components->items[entity_component];
		ZG_EComponentList req_com=registered_component->data.required_components;
		bool found=false;
		for(uint16_t j=0; j < req_com.n_components;j++){
			// check if exists...
			ZG_EComponent idx_component_req_to_find=req_com.components[j];

			found=false;
			// check whether it's int the list or not
			for(uint16_t k=0; k < list->count && found==false;k++){
				ZG_EComponent idx_component_in_list=(intptr_t)list->items[k];
				if(idx_component_req_to_find == idx_component_in_list){
					found=true;
				}
			}

			if(found==false){
				ZG_List_Add(list,(void *)((intptr_t)idx_component_req_to_find));
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
				ZG_List_Add(list,(void *)((intptr_t)entity_component));
			}
		}
	}

	ZG_EComponent *new_data=(ZG_EComponent *)malloc(sizeof(ZG_EComponent)*list->count);
	for(uint16_t k=0; k < list->count;k++){
		new_data[k]=(intptr_t)list->items[k];
	}

	*out_len=list->count;

	qsort(new_data,list->count,sizeof(ZG_EComponent),ZG_EntitySystem_OrderComponents);

	ZG_List_Delete(list);

	return new_data;
}
/*
ZG_Entity *EntitySystem_NewEntity(ZG_EntitySystem *_this,ZG_EComponent *_entity_components, size_t _entity_components_len){
	ZG_EntitySystemData *data=_this->data;
	char _archetype_id[1024]="__@_comp";
	size_t entity_components_len;

	// 1. check component requirements
	ZG_EComponent *entity_components=ZG_EntitySystem_GenerateComponentRequireZmentList(_entity_components,_entity_components_len,&entity_components_len);

	// internal type that is generated according used components...
	for(uint16_t i=0; i < entity_components_len; i++){
		char *num=ZG_String_IntToString(entity_components[i]);
		strcat(_archetype_id,"_");
		strcat(_archetype_id,num);
		free(num);
	}

	strcat(_archetype_id,"_@__");

	ZG_EntityTypeData *archetype_data=ZG_MapString_Get(data->map_archetypes,_archetype_id,NULL);

	if(archetype_data == NULL){ // create ...
		archetype_data=(ZG_EntityTypeData *)ZG_EntitySystem_NewEntityType(_this,_archetype_id ,ZG_UNLIMITIED_ENTITIES, entity_components, entity_components_len);
	}

	// Free because we don't need it anymore
	ZG_FREE(entity_components);
	return EntitySystem_NewEntityFromManager(_this,_archetype_id);
}*/
/*
ZG_Entity  *EntitySystem_NewEntityFromManager(ZG_EntitySystem *_this,const char *_id){
	ZG_EntitySystemData *data=_this->data;
	ZG_EntityTypeData *archetype_data=ZG_MapString_Get(data->map_archetypes,_id,NULL);

	if(archetype_data == NULL){
		ZG_LOG_ERROR("ZG_Entity manager '%s' not exist",_id);
		return NULL;
	}

	ZG_Entity *entity=NULL;
	if(
			archetype_data->active_entities>=archetype_data->n_entities
								&&
			archetype_data->active_entities<archetype_data->max_entities
	){ // extend entity
		ZG_EntitySystem_ExtendEntities(_this,archetype_data,1);
	}else{
		return NULL;
	}

	entity=archetype_data->entities[archetype_data->active_entities];
	archetype_data->active_entities++;

	entity->active=true;

	return entity;
}*/

void ZG_EntitySystem_ExtendComponent(ZG_EntitySystem *_this,ZG_EComponent idx_component, ZG_Entity ** entities, size_t extend){

	if(idx_component >= g_entity_manager_registered_components->count){
		return;
	}

	ZG_EntitySystemData *data=_this->data;
	ZG_EntitySystemComponentData *component_data=data->components[idx_component];
	ZG_ESRegisterComponent  registered_component_data=((ZG_EntitySystemRegisteredComponentData *)g_entity_manager_registered_components->items[idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	void *old_ptr=component_data->ptr_data;
	uint8_t *ptr_com=NULL;
	ZG_Entity **ptr_entities=NULL;
	size_t size_component_data=registered_component_data.size_data;

	// do a realloc...
	component_data->ptr_data=malloc(n_new_elements*registered_component_data.size_data);
	memset(component_data->ptr_data,0,n_new_elements*registered_component_data.size_data);

	if(old_ptr != NULL){
		memcpy(component_data->ptr_data,old_ptr,component_data->n_elements*registered_component_data.size_data);
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

void ZG_EntitySystem_ExtendEntities(ZG_EntitySystem *_this, ZG_EntityTypeData *archetype_data, size_t extend_entities){

	size_t total_extend=archetype_data->n_entities+extend_entities;

	if(total_extend >= archetype_data->max_entities){
		ZG_LOG_ERROR("cannot extend entity type '%s' up to '%i': Max entities reached (max: %i)",archetype_data->name,extend_entities,archetype_data->max_entities);
	}

	ZG_Entity **old_ptr=archetype_data->entities;

	archetype_data->entities=malloc(sizeof(ZG_Entity *)*total_extend);
	memset(archetype_data->entities,0,sizeof(ZG_Entity *)*total_extend);

	// copy old ones
	if(old_ptr != NULL){
		memcpy(archetype_data->entities,old_ptr,sizeof(ZG_Entity *)*archetype_data->n_entities);
		ZG_FREE(old_ptr);
	}

	// extend as many entities we need
	for(unsigned i=archetype_data->n_entities; i < total_extend;i++){
		archetype_data->entities[i]=ZG_Entity_New(_this);
	}

	// extend components
	for(unsigned i=0; i < archetype_data->n_components;i++){
		ZG_EComponent idx_ec=archetype_data->entity_components[i];
		// extend components this type will have
		ZG_EntitySystem_ExtendComponent(
			_this
			,idx_ec
			//,archetype_data->entities+archetype_data->n_entities
			,extend_entities
		);
	};

	// realloc set all entity types as this type
	archetype_data->n_entities=total_extend;
}

ZG_EntityType * ZG_EntitySystem_NewEntityType(
	ZG_EntitySystem *_this
	, const char *_str_archetype
	, uint16_t _max_entities
	, ZG_EComponent *_entity_components
	, size_t _entity_components_len
){
	ZG_EntitySystemData *data=_this->data;
	ZG_EntityTypeData *archetype_data=ZG_NEW(ZG_EntityTypeData);
	ZG_EntityType *archetype=ZG_NEW(ZG_EntityType);
	archetype->data=archetype_data;
	size_t	req_entity_components_len=0;

	if(_entity_components_len == 0){
		return NULL;
	}
	// get required components (it has to free after use)
	ZG_EComponent *req_entity_components=ZG_EntitySystem_GenerateComponentRequireZmentList(
			_entity_components
			,_entity_components_len
			,&req_entity_components_len);



	// check and extend required components if needed ...
	archetype_data->component_names=malloc(sizeof(char *)*req_entity_components_len);
	archetype_data->components=malloc(sizeof(ZG_EComponent)*req_entity_components_len);
	memset(archetype_data->entity_components,0,sizeof(ZG_EComponent)*req_entity_components_len);
	archetype_data->n_components=req_entity_components_len;

	//archetype_data->es_system=_this;
	archetype_data->n_entities=0;
	archetype_data->max_entities=0;
	archetype_data->active_entities=0;

	archetype_data->name=malloc(strlen(_str_archetype)*sizeof(char)+1);
	memset(archetype_data->name,0,strlen(_str_archetype)*sizeof(char)+1);
	strcpy(archetype_data->name,_str_archetype);

	ZG_EComponent *dst_ptr=archetype_data->component_idxs;
	ZG_EComponent *src_ptr=req_entity_components;

	// add components neededs for the type...
	for(unsigned i=0; i < req_entity_components_len;i++){
		// register the components this type will have
		ZG_EComponent component_id=*src_ptr;
		char *component_name=
		archetype_data->component_names[i]=malloc(sizeof(char)*src_ptr)
		*dst_ptr++=*src_ptr++;
	}


	ZG_List_Add(data->lst_archetypes,archetype);
	ZG_MapString_Set(data->map_archetypes,_str_archetype,archetype);

	// extend entities
	archetype_data->max_entities=_max_entities;
	if(_max_entities != (uint16_t)ZG_UNLIMITIED_ENTITIES){
		ZG_EntitySystem_ExtendEntities(
			_this
			,archetype_data
			,_max_entities
		);
	}

	ZG_FREE(req_entity_components);
	return archetype;
}

void ZG_EntitySystem_Update(ZG_EntitySystem * _this){
	ZG_EntitySystemData *data=(ZG_EntitySystemData *)_this->data;
	ZG_EntitySystemRegisteredComponentData  **ptr_registered_component_data=(ZG_EntitySystemRegisteredComponentData  **)g_entity_manager_registered_components->items;
	ZG_EntitySystemComponentData **components=data->components;
	ZG_EntitySystemComponentData *component=NULL;
	uint8_t *ptr_component_data=NULL;
	size_t size_data=0;
	//ZG_EComponentHeader * component_data=NULL;


	for(unsigned i=0; i < g_entity_manager_registered_components->count; i++){
		void (*ZG_EComponent_Update)(void *) =ptr_registered_component_data[i]->data.ZG_EComponent_Update;
		if(ZG_EComponent_Update != NULL){
			size_data=ptr_registered_component_data[i]->data.size_data;
			component=*components;
			ptr_component_data=component->ptr_data;
			for(unsigned i=0; i < component->n_elements; i++){
				if(((ZG_EComponentHeader * )ptr_component_data)->entity->active){
					ZG_EComponent_Update(ptr_component_data);
				}
				ptr_component_data+=size_data;
			}
		}
		ptr_registered_component_data++;
		components++;
	}
}

void ZG_EntitySystem_Delete(ZG_EntitySystem *_this){
	ZG_EntitySystemData *data=(ZG_EntitySystemData *)_this->data;
	ZG_EntitySystemRegisteredComponentData  **ptr_registered_component_data=(ZG_EntitySystemRegisteredComponentData  **)g_entity_manager_registered_components->items;
	ZG_EntitySystemComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];
	ZG_MapStringIterator *iterator=ZG_MapStringIterator_New(data->map_archetypes);

	for(; !ZG_MapStringIterator_End(iterator);ZG_MapStringIterator_Next(iterator)){
		//const char *key=MapStringIterator_GetKey(iterator);
		ZG_EntityType *archetype=ZG_MapStringIterator_GetValue(iterator);
		ZG_EntityTypeData *archetype_data=archetype->data;

		for(unsigned i=0; i < archetype_data->n_entities; i++){
			ZG_Entity_Delete(archetype_data->entities[i]);
		}

		ZG_FREE(archetype_data->entities);
		ZG_FREE(archetype_data->entity_components);
		ZG_FREE(archetype_data->name);
		ZG_FREE(archetype_data);
		ZG_FREE(archetype);
	}

	ZG_MapStringIterator_Delete(iterator);

	// release all entities
	for(unsigned i=0; i < g_entity_manager_registered_components->count; i++){
		uint8_t *ptr_data=(*component_data)->ptr_data;
		void (*ZG_EComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.ZG_EComponent_Destroy;
		size_t size_data=(*ptr_registered_component_data)->data.size_data;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(ZG_EComponent_Destroy != NULL){
					ZG_EComponent_Destroy(ptr_data);
				}
				ptr_data+=size_data;
			}
			ZG_FREE((*component_data)->ptr_data);
		}
		ptr_registered_component_data++;
		component_data++;
	}

	ZG_List_Delete(data->lst_archetypes);
	ZG_MapString_Delete(data->map_archetypes);

	for(unsigned i=0; i < g_entity_manager_registered_components->count;i++){
		ZG_FREE(data->components[i]);
	}

	ZG_FREE(data->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}


