#ifndef __ZG_ANIMATION_SYSTEM_H__
#define __ZG_ANIMATION_SYSTEM_H__


#define ZG_ACS_REGISTER_COMPONENT(_animation_system, _type_data, _update_function,_on_create,_on_destroy) \
ZG_AnimationSystem_RegisterComponent(\
	""#_type_data,\
	sizeof(_type_data)\
	,_update\
	,_on_create\
	,_on_destroy\
);

typedef struct ZG_AnimationSystem ZG_AnimationSystem;

struct ZG_AnimationSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_AnimationSystem_Init(void);
bool				ZG_AnimationSystem_RegisterComponent(ZG_RegisterComponent _register_component);
size_t				ZG_AnimationSystem_NumComponents(void);
void 				ZG_AnimationSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_AnimationSystem 	*		ZG_AnimationSystem_New(ZG_EntitySystem *_entity_system);
ZG_EntityManager 	*		ZG_AnimationSystem_NewEntityManager(
		ZG_AnimationSystem *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


//ZG_Entity  		*		AnimationSystem_NewEntity(ZG_AnimationSystem *_this,ZG_EComponent * entity_components, size_t entity_components_len);
void					ZG_AnimationSystem_Update(ZG_AnimationSystem *_this);
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
		ZG_AnimationSystem *_this
		,ZG_Entity	*_entity
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

void				ZG_AnimationSystem_Delete(ZG_AnimationSystem * _this);




#endif
