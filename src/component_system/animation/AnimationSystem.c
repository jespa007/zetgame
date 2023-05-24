#include "../zg_component_system.h"


List *g_animation_system_registered_components=NULL;
bool  g_animation_system_user_can_register_components=true;

typedef struct{
	AComponent id;
	AnimationSystemRegisterAComponent data;
}AnimationSystemRegisteredAComponentData;

typedef struct{
	void 	**ptr_data; // we want to keep pointer in order to support future reallocs
	int		*free_slots; // it gives information of free slots
	size_t 	n_free_slots;
	size_t 	n_elements;
}AnimationSystemAComponentData;

typedef struct{
	EntitySystem	*animation_system;
	AnimationSystemAComponentData **components;//[ENTITY_COMPONENT_MAX];
}AnimationSystemData;


//---------------------------------------------------
// PRIVATE FUNCTIONS

//---------------------------------------------------
bool	AnimationSystem_RegisterComponentBuiltin(AComponent _idx_component,AnimationSystemRegisterAComponent as_component_register){

	if(g_animation_system_user_can_register_components==false){
		Log_ErrorF("Components should registered before create any Entity-System");
		return false; //
	}

	if(g_animation_system_registered_components == NULL){
		g_animation_system_registered_components=List_New();
	}

	AComponent idx_component=_idx_component;//g_animation_system_registered_components->count;
	AnimationSystemRegisteredAComponentData *new_component_register=ZG_NEW(AnimationSystemRegisteredAComponentData);
	new_component_register->data=as_component_register;
	new_component_register->id=idx_component;
	List_Add(g_animation_system_registered_components,new_component_register);

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
	 min_iter=MIN(g_animation_system_registered_components->count,AC_MAX_COMPONENTS);
	for(unsigned i=0; i < min_iter; i++){
		AnimationSystemRegisteredAComponentData *component=g_animation_system_registered_components->items[i];
		if(component->id != i){
			Log_Error("Inconsistency idx components (enum:%i list:%i)",i,component->id);
			return false;
		}
	}

	g_animation_system_user_can_register_components=true;
	return true;
}

int	AnimationSystem_RegisterComponent(AnimationSystemRegisterAComponent es_component_register){
	int idx_component=0;

	if(g_animation_system_registered_components != NULL){
		idx_component=g_animation_system_registered_components->count;
	}

	if(AnimationSystem_RegisterComponentBuiltin(idx_component,es_component_register)==false){
		return EC_INVALID;
	}
	return idx_component;
}

size_t					AnimationSystem_NumComponents(void){
	if(g_animation_system_registered_components != NULL){
		return g_animation_system_registered_components->count;
	}
	return 0;
}

void AnimationSystem_DeInit(void){
	List_DeleteAndFreeAllItems(g_animation_system_registered_components);
}

//---------------------------------------------------
// PUBLIC FUNCTIONS

void AnimationSystem_ExtendComponent(AnimationSystem *_this,AComponent _idx_component, size_t extend);

AnimationSystem *AnimationSystem_New(EntitySystem *_animation_system){
	AnimationSystem *system=ZG_NEW(AnimationSystem);
	AnimationSystemData *data=ZG_NEW(AnimationSystemData);

	data->components=malloc(sizeof(AnimationSystemAComponentData)*g_animation_system_registered_components->count);
	memset(data->components,0,sizeof(AnimationSystemAComponentData)*g_animation_system_registered_components->count);

	for(unsigned i=0; i < g_animation_system_registered_components->count;i++){
		data->components[i]=ZG_NEW(AnimationSystemAComponentData);
	}

	system->data=data;
	data->animation_system=_animation_system;

	// extend 100 by default
	AnimationSystem_ExtendComponent(system,AC_TRANSFORM_ANIMATION,100);
	AnimationSystem_ExtendComponent(system,AC_MATERIAL_ANIMATION,100);

	// after first system is created, user cannot register any component anymore
	g_animation_system_user_can_register_components=false;

	return system;
}

void AnimationSystem_ExtendComponent(AnimationSystem *_this,AComponent _idx_component, size_t extend){

	if(_idx_component >= g_animation_system_registered_components->count){
		return;
	}

	AnimationSystemData *data=_this->data;
	AnimationSystemAComponentData *component_data=data->components[_idx_component];
	AnimationSystemRegisterAComponent  registered_component_data=((AnimationSystemRegisteredAComponentData *)g_animation_system_registered_components->items[_idx_component])->data;
	int current_component_data_len=component_data->n_elements;
	size_t n_new_elements=current_component_data_len+extend;
	void **old_ptr=component_data->ptr_data;
	int *old_free_slots=component_data->free_slots;
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
		registered_component_data.AComponent_Setup(new_data);
		*ptr_com++=new_data;//=registered_component_data.size_data;
	}

	// set new n elements
	component_data->n_elements=n_new_elements;

	// FREESLOTS
	// update freeslots
	component_data->free_slots=malloc(sizeof(int)*n_new_elements);
	if(old_free_slots != NULL){
		memcpy(component_data->free_slots,old_free_slots,sizeof(int)*component_data->n_elements);
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
	ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,data->animation_system);

	// Check whether entity has transform animation
	ACTransformAnimation *transform_animation=ECTransform_GetTransformAnimation(_entity->components[EC_TRANSFORM]);
	if(transform_animation == NULL){ // assign new transform animation
		// check limit
		AnimationSystemAComponentData *component_data=data->components[AC_TRANSFORM_ANIMATION];
		if(component_data->n_free_slots>0){
			int idx_slot=component_data->free_slots[component_data->n_free_slots--];
			transform_animation=component_data->ptr_data[idx_slot];
			transform_animation->header.entity=_entity;

		}else{
			Log_ErrorF("Reached max animations");
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
	AnimationSystemRegisteredAComponentData  **ptr_registered_component_data=(AnimationSystemRegisteredAComponentData  **)g_animation_system_registered_components->items;
	AnimationSystemAComponentData **component_data=data->components;

	for(unsigned i=0; i < g_animation_system_registered_components->count; i++){
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
	AnimationSystemRegisteredAComponentData  **ptr_registered_component_data=(AnimationSystemRegisteredAComponentData  **)g_animation_system_registered_components->items;
	AnimationSystemAComponentData **component_data=data->components;//[ENTITY_COMPONENT_TRANSFORM];


	// release all entities
	for(unsigned i=0; i < g_animation_system_registered_components->count; i++){
		void **ptr_data=(*component_data)->ptr_data;
		void (*AComponent_Destroy)(void *) =(*ptr_registered_component_data)->data.AComponent_Destroy;
		if( ptr_data != NULL){
			for(unsigned j=0; j < (*component_data)->n_elements; j++){
				if(AComponent_Destroy != NULL){
					AComponent_Destroy(*ptr_data);
				}
				ZG_FREE(*ptr_data++);
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
