#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#define UNLIMITIED_ENTITIES	-1

#define ZG_ECS_REGISTER_COMPONENT(_entity_system, _type_data, _required_components,_update_function,_on_create,_on_destroy) \
ComponentId __g_entity_system_component#_type_data=__g_entity_system_component++;\
EntitySystem_RegisterComponentBuiltin(__g_entity_system_component#_type_data,\
		sizeof(_type_data)\
		,_required_components\
		,_update\
		,_on_create\
		,_on_destroy\
);



#define ZG_ECS_COMPONENT_GET_COMPONENT(_entity_system, _type_data) \

/*

typedef struct{
	EComponent id;
	size_t 	size_data; // len data component
	EComponentList required_components;
	//void   (*EComponent_Setup)(void *, ComponentId _id); // function to Setup component
	void   (*EComponent_OnCreate)(void *_component_data); // set it up if component need to init or allocate resources on its creation
	void   (*EComponent_OnUpdate)(void *_component_data); // function component
	void   (*EComponent_OnDestroy)(void *_component_data);// set it up if component need to deinit or deallocate resources on its creation
	//void   (*EComponent_Destroy)(void *); // function to destroy
}EntitySystemRegisterEComponent;*/

struct EntitySystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				EntitySystem_Init(void);
/*void					EntitySystem_RegisterComponent(
		EComponent _idx_component
		//,EntitySystemRegisterEComponent es_component_register
		//EComponent id;
		,size_t 	_size_data // len data component
		,EComponentList required_components
		,void   (*EComponent_OnCreate)(void *_component_data) // set it up if component need to init or allocate resources on its creation
		,void   (*EComponent_OnUpdate)(void *_component_data) // function component
		,void   (*EComponent_OnDestroy)(void *_component_data)
);*/

size_t				EntitySystem_NumComponents(void);
void 				EntitySystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
EntitySystem 	*		EntitySystem_New(void);
EntityManager 	*		EntitySystem_NewEntityManager(
		EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, EComponent * entity_components
		, size_t entity_components_len
);


//Entity  		*		EntitySystem_NewEntity(EntitySystem *_this,EComponent * entity_components, size_t entity_components_len);
void					EntitySystem_Update(EntitySystem *_this);
void 					EntitySystem_GetComponent(EntitySystem *_this, Entity * _entity, ComponentId _component_id);
//uint8_t *EntitySystem_NewComponent(EntitySystem *_this,int idx_component);
/*void  			EntitySystem_RemoveEntity(EntitySystem * _this, Entity entity);

// Channels
ZG_Transform 	*		EntitySystem_GetComponentTransform(EntitySystem * _this, Entity entity);
TransformNode 		*		EntitySystem_GetComponentTransformNode(EntitySystem * _this, Entity entity);
ZG_Appearance 	*		EntitySystem_GetComponentAppearance(EntitySystem * _this, Entity entity);
ZG_Geometry 	*		EntitySystem_GetComponentGeometry(EntitySystem * _this, Entity entity);
Camera 		*		EntitySystem_GetComponentCamera(EntitySystem * _this, Entity entity);

*/
void				EntitySystem_Delete(EntitySystem * _this);




#endif
