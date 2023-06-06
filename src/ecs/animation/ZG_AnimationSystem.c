#include "../zg_ecs.h"


ZG_List *__g_acs_system_registered_components=NULL;
bool  __g_acs_system_user_can_register_components=true;

typedef struct{
	AComponent id;
	AnimationSystemRegisterAComponent data;
}AnimationSystemRegisteredAComponentData;

typedef struct{
	uint8_t				*ptr_data; // we want to keep pointer in order to support future reallocs
	size_t 				n_elements;

	ComponentId			*free_slots; // it gives information of free slots
	size_t 				n_free_slots;
}AnimationSystemAComponentData;

typedef struct{
	EntitySystem	*entity_system;
	AnimationSystemAComponentData **components;//[ENTITY_COMPONENT_MAX];
}AnimationSystemData;


//---------------------------------------------------
// PRIVATE FUNCTIONS

//---------------------------------------------------
bool	AnimationSystem_RegisterComponentBuiltin(AComponent _idx_component,AnimationSystemRegisterAComponent as_component_register){

	if(__g_acs_system_user_can_register_components==false){
		ZG_Log_ErrorF("Components should registered before create any Entity-System");
		return false; //
	}

	if(__g_acs_system_registered_components == NULL){
		__g_acs_system_registered_components=ZG_List_New();
	}

	AComponent idx_component=_idx_component;//__g_acs_system_registered_components->count;
	AnimationSystemRegisteredAComponentData *new_component_register=ZG_NEW(AnimationSystemRegisteredAComponentData);
	new_component_register->data=as_component_register;
	new_component_register->id=idx_component;
	ZG_List_Add(__g_acs_system_registered_components,new_component_register);

	return true;
}

//---------------------------------------------------
// STATIC FUNCTIONS
bool AnimationSystem_Init(void){

	unsigned min_iter=0;

	// invalid (0)
	AnimationSystem_RegisterComponentBuiltin(AC_INVALID,(AnimationSystemRegisterAComponent){
		.size_data				=0
		,.AComponent_Setup		=NULL
		,.AComponent_Update		=NULL
		,.AComponent_Destroy	=NULL
	});

	// Animation transform
	AnimationSystem_RegisterComponentBuiltin(AC_TRANSFORM_ANIMATION,(AnimationSystemRegisterAComponent){
		.size_data				=sizeof(ACTransformAnimation)
		,.AComponent_Setup		=ACTransformAnimation_Setup
		,.AComponent_Update		=ACTransformAnimation_Update
		,.AComponent_Destroy	=ACTransformAnimation_Destroy
	});

	// material animation
	AnimationSystem_RegisterComponentBuiltin(AC_MATERIAL_ANIMATION,(AnimationSystemRegisterAComponent){
		.size_data				=sizeof(ACMaterialAnimation)
		,.AComponent_Setup		=ACMaterialAnimation_Setup
		,.AComponent_Update		=ACMaterialAnimation_Update
		,.AComponent_Destroy	=ACMaterialAnimation_Destroy
	});


	// check component consistency
	 min_iter=MIN(__g_acs_system_registered_components->count,AC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		AnimationSystemRegisteredAComponentData *component=__g_acs_system_registered_components->items[i];
		if(component->id != i){
			ZG_Log_Error("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	__g_acs_system_user_can_register_components=true;
	return true;
}

int	AnimationSystem_RegisterComponent(AnimationSystemRegisterAComponent es_component_register){
	int idx_component=0;

	if(__g_acs_system_registered_components != NULL){
		idx_component=__g_acs_system_registered_components->count;
	}

	if(AnimationSystem_RegisterComponentBuiltin(idx_component,es_component_register)==false){
		return EC_INVALID;
	}
	return idx_component;
}

size_t					AnimationSystem_NumComponents(void){
	if(__g_acs_system_registered_components != NULL){
		return __g_acs_system_registered_components->count;
	}
	return 0;
}

void AnimationSystem_DeInit(void){
	ZG_List_DeleteAndFreeAllItems(__g_acs_system_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS

void AnimationSystem_ExtendComponent(AnimationSystem *_this,AComponent _idx_component, size_t extend);

AnimationSystem *AnimationSystem_New(EntitySystem *_entity_system){
	AnimationSystem *system=ZG_NEW(AnimationSystem);
	AnimationSystemData *data=ZG_NEW(AnimationSystemData);

	data->components=malloc(sizeof(AnimationSystemAComponentData)*__g_acs_system_registered_components->count);
	memset(data->components,0,sizeof(AnimationSystemAComponentData)*__g_acs_system_registered_components->count);

	for(unsigned i=0; i < __g_acs_system_registered_components->count;i++){
		data->components[i]=ZG_NEW(AnimationSystemAComponentData);
	}

	system->data=data;
	data->entity_system=_entity_system;

	// extend 100 by default
	AnimationSystem_ExtendComponent(system,AC_TRANSFORM_ANIMATION,100);
	AnimationSystem_ExtendComponent(system,AC_MATERIAL_ANIMATION,100);

	// after first system is created, user cannot register any component anymore
	__g_acs_system_user_can_register_components=false;

	return system;
}

void AnimationSystem_ExtendComponent(AnimationSystem *_this,AComponent _idx_component, size_t extend){

	if(_idx_component >= __g_acs_system_registered_components->count){
		return;
	}

	AnimationSystemData *data=_this->data;
	AnimationSystemAComponentData *component_data=data->components[_idx_component];
	AnimationSystemRegisterAComponent  registered_component_data=((AnimationSystemRegisteredAComponentData *)__g_acs_system_registered_components->items[_idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	uint8_t *old_ptr=component_data->ptr_data;
	ComponentId *old_free_slots=component_data->free_slots;

	// do a realloc...
	component_data->ptr_data=malloc(n_new_elements*registered_component_data.size_data);

	if(old_ptr != NULL){
		memcpy(component_data->ptr_data,old_ptr,component_data->n_elements*registered_component_data.size_data);
		// update ptr
		ZG_FREE(old_ptr);
	}

	// initialize new components
	for(unsigned i=component_data->n_elements; i < n_new_elements; i++){
		registered_component_data.AComponent_Setup(&component_data[i],i);
	}


	// set new n elements
	component_data->n_elements=n_new_elements;

	// FREESLOTS
	// update freeslots
	component_data->free_slots=malloc(sizeof(int)*n_new_elements);
	if(old_free_slots != NULL){
		memcpy(component_data->free_slots,old_free_slots,sizeof(ComponentId)*component_data->n_elements);
		ZG_FREE(old_free_slots);
	}

	// update free slots
	for(unsigned i=component_data->n_free_slots; i < (component_data->n_free_slots+extend);i++){
		component_data->free_slots[component_data->n_free_slots+i]=current_component_data_len+i;
	}

	// set new n free slots
	component_data->n_free_slots+=extend;
}


void				AnimationSystem_StartTweenTransform(
		AnimationSystem *_this
		,Entity	*_entity
		, TransformComponent _transform_component
		, Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat

){

	AnimationSystemData *data=(AnimationSystemData *)_this->data;
	ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,data->entity_system);

	// Check whether entity has transform animation
	ACTransformAnimation *transform_animation=ECTransform_GetTransformAnimation(
			ZG_ENTITY_GET_COMPONENT(
				_entity,ECTransform
			)
	);//->components[EC_TRANSFORM]);

	if(transform_animation == NULL){ // assign new transform animation
		// check limit
		AnimationSystemAComponentData *component_data=data->components[AC_TRANSFORM_ANIMATION];
		if(component_data->n_free_slots>0){
			AnimationSystemRegisterAComponent * registered_animation=(AnimationSystemRegisterAComponent *)__g_acs_system_registered_components->items[AC_TRANSFORM_ANIMATION];
			int idx_slot=component_data->free_slots[component_data->n_free_slots--];
			transform_animation=(ACTransformAnimation *)component_data->ptr_data+idx_slot*registered_animation->size_data;
			transform_animation->header.entity=_entity;

		}else{
			ZG_Log_ErrorF("Reached max animations");
			return;
		}

	}

	// start tween
	ACTransformAnimation_StartTween(
		  transform_animation
		, _transform_component
		, _ease
		, _from
		, _to
		, _duration
		, _repeat
	);
}

void AnimationSystem_Update(AnimationSystem * _this){
	AnimationSystemData *data=(AnimationSystemData *)_this->data;
	AnimationSystemRegisteredAComponentData  **ptr_registered_component_data=(AnimationSystemRegisteredAComponentData  **)__g_acs_system_registered_components->items;
	AnimationSystemAComponentData **component_data=data->components;

	for(unsigned i=0; i < __g_acs_system_registered_components->count; i++){
		void (*AComponent_Update)(void *) =(*ptr_registered_component_data)->data.AComponent_Update;
		if(AComponent_Update != NULL){
			AComponentHeader **ptr_data=(AComponentHeader **)(*component_data)->ptr_data;
			for(unsigned i=0; i < (*component_data)->n_elements; i++){
				AComponentHeader * data=*ptr_data;
				if(data->entity!=NULL){
					AComponent_Update(data);
				}
				ptr_data++;
			}
		}
		ptr_registered_component_data++;
		component_data++;
	}
}


void AnimationSystem_Delete(AnimationSystem *_this){
	AnimationSystemData *data=(AnimationSystemData *)_this->data;
	AnimationSystemRegisteredAComponentData  **ptr_registered_component_data=(AnimationSystemRegisteredAComponentData  **)__g_acs_system_registered_components->items;
	AnimationSystemAComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];


	// release all entities
	for(unsigned i=0; i < __g_acs_system_registered_components->count; i++){
		uint8_t *ptr_data=(*component_data)->ptr_data;
		void (*AComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.AComponent_Destroy;
		size_t size_data=(*ptr_registered_component_data)->data.size_data;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(AComponent_Destroy != NULL){
					AComponent_Destroy(ptr_data);
				}
				ptr_data+=size_data;
			}
			ZG_FREE((*component_data)->ptr_data);
		}
		ptr_registered_component_data++;
		component_data++;
	}


	for(unsigned i=0; i < __g_acs_system_registered_components->count;i++){
		ZG_FREE(data->components[i]);
	}

	ZG_FREE(data->components);
	ZG_FREE(data);
	ZG_FREE(_this);
}
