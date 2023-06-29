#include "../_zg_ecs_.h"


ZG_List 		*	g_animation_system_registered_components=NULL;
ZG_MapString 	*	g_map_animation_system_registered_components=NULL;
int 				g_animation_system_num_registered_component=0;

bool  		g_animation_system_user_can_register_components=true;

/*
typedef struct{
	ZG_AComponent id;
	ZG_RegisterComponent data;
}ZG_AnimationSystemRegisteredComponentData;
*/

typedef struct{
	uint8_t				*ptr_data; // we want to keep pointer in order to support future reallocs
	size_t 				n_elements;

	ZG_ComponentId			*free_slots; // it gives information of free slots
	size_t 				n_free_slots;
}ZG_AnimationSystemComponentData;

typedef struct{
	ZG_EntitySystem	*entity_system;
	ZG_AnimationSystemComponentData **components;//[ENTITY_COMPONENT_MAX];
}ZG_AnimationSystemData;


//---------------------------------------------------
// PRIVATE FUNCTIONS

//---------------------------------------------------
/*bool	ZG_AnimationSystem_RegisterComponent(
		ZG_AComponent _idx_component
		,ZG_AnimationSystemRegisterComponent as_component_register
){

	if(g_animation_system_user_can_register_components==false){
		ZG_LOG_ERRORF("Components should registered before create any ZG_Entity-System");
		return false; //
	}

	if(g_animation_system_registered_components == NULL){
		g_animation_system_registered_components=ZG_List_New();
	}

	ZG_AComponent idx_component=_idx_component;//g_animation_system_registered_components->count;
	ZG_AnimationSystemRegisteredComponentData *new_component_register=ZG_NEW(ZG_AnimationSystemRegisteredComponentData);
	new_component_register->data=as_component_register;
	new_component_register->id=idx_component;
	ZG_List_Add(g_animation_system_registered_components,new_component_register);

	return true;
}*/

bool	ZG_AnimationSystem_AddComponent(
		const char *_name
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
		,void   (* _on_update)(void *) // function component
		,void   (* _on_create)(void *) // set it up if component need to init or allocate resources on its creation
		,void   (* _on_destroy)(void *)
){

	if(g_animation_system_user_can_register_components==false){
		ZG_LOG_ERRORF("Components should registered before create any ZG_Entity-System");
		return false; //
	}

	if(g_animation_system_registered_components == NULL){
		g_animation_system_registered_components=ZG_List_New();
		g_map_animation_system_registered_components=ZG_MapString_New();
	}

	//ZG_EComponent idx_component=_idx_component;//g_entity_system_registered_components->count;
	ZG_RegisterComponent *new_component_register=ZG_NEW(ZG_RegisterComponent);

	new_component_register->id=g_animation_system_num_registered_component++;
	new_component_register->name=_name;
	new_component_register->size_data=_size_data;
	new_component_register->on_update=_on_update;
	new_component_register->on_create=_on_create;
	new_component_register->on_destroy=_on_destroy;


	ZG_MapString_Set(g_map_animation_system_registered_components,_name,new_component_register);
	ZG_List_Add(g_animation_system_registered_components,new_component_register);
	return true;
}

//---------------------------------------------------
// STATIC FUNCTIONS
bool ZG_AnimationSystem_Init(void){

	unsigned min_iter=0;

	// invalid (0)
	/*ZG_AnimationSystem_RegisterComponentBuiltin(ZG_AC_INVALID,(ZG_AnimationSystemRegisterComponent){
		.size_data				=0
		,.ZG_AComponent_Setup		=NULL
		,.ZG_AComponent_Update		=NULL
		,.ZG_AComponent_Destroy	=NULL
	});*/

	ZG_ACS_ADD_COMPONENT(ZG_ACTransformAnimation
			,ZG_ACTransformAnimation_Update
			,ZG_ACTransformAnimation_Setup
			,ZG_ACTransformAnimation_Destroy
	);

	// Animation transform
	/*ZG_AnimationSystem_RegisterComponentBuiltin(ZG_AC_TRANSFORM_ANIMATION,(ZG_AnimationSystemRegisterComponent){
		.size_data				=sizeof(ZG_ACTransformAnimation)
		,.ZG_AComponent_Setup		=ACTransformAnimation_Setup
		,.ZG_AComponent_Update		=ACTransformAnimation_Update
		,.ZG_AComponent_Destroy	=ACTransformAnimation_Destroy
	});*/

	// material animation
	/*ZG_AnimationSystem_RegisterComponentBuiltin(ZG_AC_MATERIAL_ANIMATION,(ZG_AnimationSystemRegisterComponent){
		.size_data				=sizeof(ZG_ACMaterialAnimation)
		,.ZG_AComponent_Setup		=ZG_ACMaterialAnimation_Setup
		,.ZG_AComponent_Update		=ZG_ACMaterialAnimation_Update
		,.ZG_AComponent_Destroy	=ZG_ACMaterialAnimation_Destroy
	});


	// check component consistency
	 min_iter=MIN(g_animation_system_registered_components->count,AC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		ZG_AnimationSystemRegisteredComponentData *component=g_animation_system_registered_components->items[i];
		if(component->id != i){
			ZG_LOG_ERROR("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}
*/
	g_animation_system_user_can_register_components=true;
	return true;
}
/*
bool	ZG_AnimationSystem_RegisterComponent(ZG_RegisterComponent _register_component){
	int idx_component=0;

	if(g_animation_system_registered_components != NULL){
		idx_component=g_animation_system_registered_components->count;
	}

	if(ZG_AnimationSystem_RegisterComponentBuiltin(idx_component,_register_component)==false){
		return false;
	}
	return true;
}*/

size_t					ZG_AnimationSystem_NumComponents(void){
	if(g_animation_system_registered_components != NULL){
		return g_animation_system_registered_components->count;
	}
	return 0;
}

void ZG_AnimationSystem_DeInit(void){
	ZG_List_DeleteAndFreeAllItems(g_animation_system_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS

void ZG_AnimationSystem_ExtendComponent(ZG_AnimationSystem *_this,ZG_AComponent _idx_component, size_t extend);

ZG_AnimationSystem *ZG_AnimationSystem_New(ZG_EntitySystem *_entity_system){
	ZG_AnimationSystem *system=ZG_NEW(ZG_AnimationSystem);
	ZG_AnimationSystemData *data=ZG_NEW(ZG_AnimationSystemData);

	data->components=malloc(sizeof(ZG_AnimationSystemComponentData)*g_animation_system_registered_components->count);
	memset(data->components,0,sizeof(ZG_AnimationSystemComponentData)*g_animation_system_registered_components->count);

	for(unsigned i=0; i < g_animation_system_registered_components->count;i++){
		data->components[i]=ZG_NEW(ZG_AnimationSystemComponentData);
	}

	system->data=data;
	data->entity_system=_entity_system;

	// extend 100 by default
	//ZG_AnimationSystem_ExtendComponent(system,ZG_AC_TRANSFORM_ANIMATION,100);
	//ZG_AnimationSystem_ExtendComponent(system,ZG_AC_MATERIAL_ANIMATION,100);

	// after first system is created, user cannot register any component anymore
	g_animation_system_user_can_register_components=false;

	return system;
}

void ZG_AnimationSystem_ExtendComponent(ZG_AnimationSystem *_this,ZG_AComponent _idx_component, size_t extend){


	// new behaviour

	// TODO
	// 1. Call on_destroy of all required components
	// 2. re/allocate new array * sizeof(component).
	// 3. Call on_create
	// 4. re/allocate new array of indexs for reference components that they changed its position
	// 5. Reset reference indexs

	if(_idx_component >= g_animation_system_registered_components->count){
		return;
	}

	ZG_AnimationSystemData *data=_this->data;
	ZG_AnimationSystemComponentData *component_data=data->components[_idx_component];
	ZG_AnimationSystemRegisterComponent  registered_component_data=((ZG_AnimationSystemRegisteredComponentData *)g_animation_system_registered_components->items[_idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	uint8_t *old_ptr=component_data->ptr_data;
	ZG_ComponentId *old_free_slots=component_data->free_slots;

	// do a realloc...
	component_data->ptr_data=malloc(n_new_elements*registered_component_data.size_data);

	if(old_ptr != NULL){
		memcpy(component_data->ptr_data,old_ptr,component_data->n_elements*registered_component_data.size_data);
		// update ptr
		ZG_FREE(old_ptr);
	}

	// initialize new components
	for(unsigned i=component_data->n_elements; i < n_new_elements; i++){
		registered_component_data.ZG_AComponent_Setup(&component_data[i],i);
	}


	// set new n elements
	component_data->n_elements=n_new_elements;

	// FREESLOTS
	// update freeslots
	component_data->free_slots=malloc(sizeof(int)*n_new_elements);
	if(old_free_slots != NULL){
		memcpy(component_data->free_slots,old_free_slots,sizeof(ZG_ComponentId)*component_data->n_elements);
		ZG_FREE(old_free_slots);
	}

	// update free slots
	for(unsigned i=component_data->n_free_slots; i < (component_data->n_free_slots+extend);i++){
		component_data->free_slots[component_data->n_free_slots+i]=current_component_data_len+i;
	}

	// set new n free slots
	component_data->n_free_slots+=extend;
}


void				ZG_AnimationSystem_StartTweenTransform(
		ZG_AnimationSystem *_this
		,ZG_Entity	*_entity
		, ZG_TransformComponent _transform_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat

){

	ZG_AnimationSystemData *data=(ZG_AnimationSystemData *)_this->data;
	ZG_ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,data->entity_system);

	// Check whether entity has transform animation
	ZG_ACTransformAnimation *transform_animation=ZG_ECTransform_GetTransformAnimation(
			ZG_ENTITY_GET_COMPONENT(
				_entity,ZG_ECTransform
			)
	);//->components[EC_TRANSFORM]);

	if(transform_animation == NULL){ // assign new transform animation
		// check limit
		ZG_AnimationSystemComponentData *component_data=data->components[ZG_AC_TRANSFORM_ANIMATION];
		if(component_data->n_free_slots>0){
			ZG_AnimationSystemRegisterComponent * registered_animation=(ZG_AnimationSystemRegisterComponent *)g_animation_system_registered_components->items[ZG_AC_TRANSFORM_ANIMATION];
			int idx_slot=component_data->free_slots[component_data->n_free_slots--];
			transform_animation=(ZG_ACTransformAnimation *)component_data->ptr_data+idx_slot*registered_animation->size_data;
			transform_animation->header.entity=_entity;

		}else{
			ZG_LOG_ERRORF("Reached max animations");
			return;
		}

	}

	// start tween
	ZG_ACTransformAnimation_StartTween(
		  transform_animation
		, _transform_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void ZG_AnimationSystem_Update(ZG_AnimationSystem * _this){
	ZG_AnimationSystemData *data=(ZG_AnimationSystemData *)_this->data;
	ZG_AnimationSystemRegisteredComponentData  **ptr_registered_component_data=(ZG_AnimationSystemRegisteredComponentData  **)g_animation_system_registered_components->items;
	ZG_AnimationSystemComponentData **component_data=data->components;

	for(unsigned i=0; i < g_animation_system_registered_components->count; i++){
		void (*ZG_AComponent_Update)(void *) =(*ptr_registered_component_data)->data.ZG_AComponent_Update;
		if(ZG_AComponent_Update != NULL){
			ZG_AComponentHeader **ptr_data=(ZG_AComponentHeader **)(*component_data)->ptr_data;
			for(unsigned i=0; i < (*component_data)->n_elements; i++){
				ZG_AComponentHeader * data=*ptr_data;
				if(data->entity!=NULL){
					ZG_AComponent_Update(data);
				}
				ptr_data++;
			}
		}
		ptr_registered_component_data++;
		component_data++;
	}
}


void ZG_AnimationSystem_Delete(ZG_AnimationSystem *_this){
	ZG_AnimationSystemData *data=(ZG_AnimationSystemData *)_this->data;
	ZG_AnimationSystemRegisteredComponentData  **ptr_registered_component_data=(ZG_AnimationSystemRegisteredComponentData  **)g_animation_system_registered_components->items;
	ZG_AnimationSystemComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];


	// release all entities
	for(unsigned i=0; i < g_animation_system_registered_components->count; i++){
		uint8_t *ptr_data=(*component_data)->ptr_data;
		void (*ZG_AComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.ZG_AComponent_Destroy;
		size_t size_data=(*ptr_registered_component_data)->data.size_data;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(ZG_AComponent_Destroy != NULL){
					ZG_AComponent_Destroy(ptr_data);
				}
				ptr_data+=size_data;
			}
			ZG_FREE((*component_data)->ptr_data);
		}
		ptr_registered_component_data++;
		component_data++;
	}


	for(unsigned i=0; i < g_animation_system_registered_components->count;i++){
		ZG_FREE(data->components[i]);
	}

	ZG_FREE(data->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
