#ifndef __ZG_ANIMATION_SYSTEM_H__
#define __ZG_ANIMATION_SYSTEM_H__


#define ZG_ACS_ADD_COMPONENT(_component_type, _on_create,_on_destroy) \
ZG_AnimationSystem_RegisterComponent(\
	""#_component_type,\
	sizeof(_type_data)\
	,_on_create\
	,_on_destroy\
)

#define ZG_ACS_ON_UPDATE(_component_id, _on_update) \
ZG_AnimationSystem_SetComponentOnUpdate(\
	_component_id\
	,_on_update\
)

#define ZG_BITS_ANIMATION_MANAGERS		4
#define ZG_BITS_ANIMATION_ENTITIES		18


typedef unsigned int ZG_AComponentId;
typedef unsigned int ZG_AnimationManager;
/*
struct ZG_AnimationSystem{
	void *data;
};*/
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_AnimationSystem_Init(void);
bool	ZG_AnimationSystem_RegisterComponent(
		const char *_name
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
		,void   (* _on_update)(void *_component_data) // function component
		,void   (* _on_create)(void *_component_data, ZG_Component _id) // set it up if component need to init or allocate resources on its creation
		,void   (* _on_destroy)(void *_component_data)
);
size_t				ZG_AnimationSystem_NumComponents(void);
void 				ZG_AnimationSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
//ZG_AnimationSystem		ZG_AnimationSystem_New();
ZG_AComponentId  	ZG_AnimationSystem_NewTween(
		ZG_Entity _entity
		, ZG_AComponentId _animation_component_id
);
ZG_AComponentId  	ZG_AnimationSystem_NewAnimation(ZG_Entity _entity, const char *_animation_component_name);


//ZG_Entity  		*		AnimationSystem_NewEntity(ZG_AnimationSystem *_this,ZG_EComponent * entity_components, size_t entity_components_len);
void					ZG_AnimationSystem_Update(ZG_AnimationSystem _animation_manager);
//uint8_t *AnimationSystem_NewComponent(ZG_AnimationSystem *_this,int idx_component);
/*void  			AnimationSystem_RemoveEntity(ZG_AnimationSystem * _this, ZG_Entity entity);

// Channels
ZG_Transform 	*		AnimationSystem_GetComponentTransform(ZG_AnimationSystem * _this, ZG_Entity entity);
TransformNode 		*		AnimationSystem_GetComponentTransformNode(ZG_AnimationSystem * _this, ZG_Entity entity);
ZG_Appearance 	*		AnimationSystem_GetComponentAppearance(ZG_AnimationSystem * _this, ZG_Entity entity);
ZG_Geometry 	*		AnimationSystem_GetComponentGeometry(ZG_AnimationSystem * _this, ZG_Entity entity);
Camera 		*		AnimationSystem_GetComponentCamera(ZG_AnimationSystem * _this, ZG_Entity entity);
*/
void				ZG_AnimationSystem_StartTweenTransform(
		ZG_AnimationSystem _animation_manager
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

void				ZG_AnimationSystem_Delete(ZG_AnimationSystem _animation_manager);




#endif
