#ifndef __ZG_EANIMATION_SYSTEM_H__
#define __ZG_EANIMATION_SYSTEM_H__


#define ZG_ACS_ADD_COMPONENT(_type_data, _on_update,_on_create,_on_destroy) \
ZG_EAnimationManager_RegisterComponent(\
	""#_type_data,\
	sizeof(_type_data)\
	,_on_update\
	,_on_create\
	,_on_destroy\
)

#define ZG_BITS_EANIMATION_MANAGERS		4
#define ZG_BITS_EANIMATION_ENTITIES		18


typedef unsigned int ZG_EAComponentId;
typedef unsigned int ZG_EAnimationManager;
typedef unsigned int ZG_EntityAnimation;
/*
struct ZG_EAnimationManager{
	void *data;
};*/
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_EAnimationManager_Init(void);
bool	ZG_EAnimationManager_RegisterComponent(
		const char *_name
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
		,void   (* _on_update)(void *_component_data) // function component
		,void   (* _on_create)(void *_component_data, ZG_Component _id) // set it up if component need to init or allocate resources on its creation
		,void   (* _on_destroy)(void *_component_data)
);
size_t				ZG_EAnimationManager_NumComponents(void);
void 				ZG_EAnimationManager_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_EAnimationManager		ZG_EAnimationManager_New();
ZG_EntityAnimation  	ZG_EAnimationManager_NewEntityAnimation(ZG_EAnimationManager _animation_manager, ZG_Entity _entity, ZG_AComponentId _animation_component_id);
ZG_EntityAnimation  	ZG_EAnimationManager_NewEntityAnimation(ZG_EAnimationManager _animation_manager, ZG_Entity _entity, const char *_animation_component_name);


//ZG_Entity  		*		AnimationSystem_NewEntity(ZG_EAnimationManager *_this,ZG_EComponent * entity_components, size_t entity_components_len);
void					ZG_EAnimationManager_Update(ZG_EAnimationManager _animation_manager);
//uint8_t *AnimationSystem_NewComponent(ZG_EAnimationManager *_this,int idx_component);
/*void  			AnimationSystem_RemoveEntity(ZG_EAnimationManager * _this, ZG_Entity entity);

// Channels
ZG_Transform 	*		AnimationSystem_GetComponentTransform(ZG_EAnimationManager * _this, ZG_Entity entity);
TransformNode 		*		AnimationSystem_GetComponentTransformNode(ZG_EAnimationManager * _this, ZG_Entity entity);
ZG_Appearance 	*		AnimationSystem_GetComponentAppearance(ZG_EAnimationManager * _this, ZG_Entity entity);
ZG_Geometry 	*		AnimationSystem_GetComponentGeometry(ZG_EAnimationManager * _this, ZG_Entity entity);
Camera 		*		AnimationSystem_GetComponentCamera(ZG_EAnimationManager * _this, ZG_Entity entity);
*/
void				ZG_EAnimationManager_StartTweenTransform(
		ZG_EAnimationManager _animation_manager
		,ZG_Entity	_entity
		, ZG_TransformComponent _transform_component
		, ZG_Ease _ease
		, float _from
		, float _to
		, uint32_t _duration
		, int _repeat

);
/*
void				AnimationSystem_StartTweenMaterial(

);

void				AnimationSystem_StartActionTransform(
);


void				AnimationSystem_StartActionMaterial(

);
*/

void				ZG_EAnimationManager_Delete(ZG_EAnimationManager _animation_manager);




#endif
