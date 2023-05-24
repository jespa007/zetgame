#ifndef __ANIMATION_SYSTEM_H__
#define __ANIMATION_SYSTEM_H__


typedef struct AnimationSystem AnimationSystem;

typedef struct{
	AComponent id;
	size_t 	size_data; // len data component
	void   (*AComponent_Setup)(void *); // function to Setup component
	void   (*AComponent_Update)(void *); // function to update component
	void   (*AComponent_Destroy)(void *); // function to destroy
}AnimationSystemRegisterAComponent;

struct AnimationSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				AnimationSystem_Init(void);
int					AnimationSystem_RegisterComponent(AnimationSystemRegisterAComponent as_component_register);
size_t				AnimationSystem_NumComponents(void);
void 				AnimationSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
AnimationSystem 	*		AnimationSystem_New(EntitySystem *_entity_system);
EntityManager 	*		AnimationSystem_NewEntityManager(
		AnimationSystem *_this
		, const char *_id
		,uint16_t max_entities
		, EComponent * entity_components
		, size_t entity_components_len
);


//Entity  		*		AnimationSystem_NewEntity(AnimationSystem *_this,EComponent * entity_components, size_t entity_components_len);
void					AnimationSystem_Update(AnimationSystem *_this);
//uint8_t *AnimationSystem_NewComponent(AnimationSystem *_this,int idx_component);
/*void  			AnimationSystem_RemoveEntity(AnimationSystem * _this, Entity entity);

// Channels
Transform 	*		AnimationSystem_GetComponentTransform(AnimationSystem * _this, Entity entity);
TransformNode 		*		AnimationSystem_GetComponentTransformNode(AnimationSystem * _this, Entity entity);
Appearance 	*		AnimationSystem_GetComponentAppearance(AnimationSystem * _this, Entity entity);
Geometry 	*		AnimationSystem_GetComponentGeometry(AnimationSystem * _this, Entity entity);
Camera 		*		AnimationSystem_GetComponentCamera(AnimationSystem * _this, Entity entity);
*/
void				AnimationSystem_StartTweenTransform(
		AnimationSystem *_this
		,Entity	*_entity
		, TransformComponent _transform_component
		, Ease _ease
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

void				AnimationSystem_Delete(AnimationSystem * _this);




#endif
