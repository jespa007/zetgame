#ifndef __ZG_ENTITY_SYSTEM_H__
#define __ZG_ENTITY_SYSTEM_H__

#define ZG_UNLIMITIED_ENTITIES	-1

#define ZG_ECS_REGISTER_COMPONENT(_entity_system, _type_data, _required_components,_update_function,_on_create,_on_destroy) \
ZG_ComponentId __g_entity_system_component#_type_data=__g_entity_system_component++;\
ZG_ZG_EntitySystem_RegisterComponentBuiltin(__g_entity_system_component#_type_data,\
		sizeof(_type_data)\
		,_required_components\
		,_update\
		,_on_create\
		,_on_destroy\
);



#define ZG_ECS_COMPONENT_GET_COMPONENT(_entity_system, _type_data) \


typedef struct{
	ZG_ComponentId id;
	const char *name;
	size_t 	size_data; // len data component
	void   (*setup)(void *); // function to Setup component
	void   (*update)(void *); // function to update component
	void   (*destroy)(void *); // function to destroy
}ZG_RegisterComponent;

/*

typedef struct{
	ZG_EComponent id;
	size_t 	size_data; // len data component
	ZG_EComponentList required_components;
	//void   (*EComponent_Setup)(void *, ZG_ComponentId _id); // function to Setup component
	void   (*EComponent_OnCreate)(void *_component_data); // set it up if component need to init or allocate resources on its creation
	void   (*EComponent_OnUpdate)(void *_component_data); // function component
	void   (*EComponent_OnDestroy)(void *_component_data);// set it up if component need to deinit or deallocate resources on its creation
	//void   (*ZG_EComponent_Destroy)(void *); // function to destroy
}ZG_ESRegisterComponent;*/

struct ZG_EntitySystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_EntitySystem_Init(void);
/*void					ZG_EntitySystem_RegisterComponent(
		ZG_EComponent _idx_component
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		,ZG_EComponentList required_components
		,void   (*EComponent_OnCreate)(void *_component_data) // set it up if component need to init or allocate resources on its creation
		,void   (*EComponent_OnUpdate)(void *_component_data) // function component
		,void   (*EComponent_OnDestroy)(void *_component_data)
);*/

size_t				ZG_EntitySystem_NumComponents(void);
void 				ZG_EntitySystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_EntitySystem 	*		ZG_EntitySystem_New(void);
ZG_EntityManager 	*		ZG_EntitySystem_NewEntityManager(
		ZG_EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


//ZG_Entity  		*		EntitySystem_NewEntity(ZG_EntitySystem *_this,ZG_EComponent * entity_components, size_t entity_components_len);
void					ZG_EntitySystem_Update(ZG_EntitySystem *_this);
void 					ZG_EntitySystem_GetComponent(ZG_EntitySystem *_this, ZG_Entity * _entity, ZG_ComponentId _component_id);
//uint8_t *EntitySystem_NewComponent(ZG_EntitySystem *_this,int idx_component);
/*void  			EntitySystem_RemoveEntity(ZG_EntitySystem * _this, ZG_Entity entity);

// Channels
ZG_Transform 	*		EntitySystem_GetComponentTransform(ZG_EntitySystem * _this, ZG_Entity entity);
TransformNode 		*		EntitySystem_GetComponentTransformNode(ZG_EntitySystem * _this, ZG_Entity entity);
ZG_Appearance 	*		EntitySystem_GetComponentAppearance(ZG_EntitySystem * _this, ZG_Entity entity);
ZG_Geometry 	*		EntitySystem_GetComponentGeometry(ZG_EntitySystem * _this, ZG_Entity entity);
Camera 		*		EntitySystem_GetComponentCamera(ZG_EntitySystem * _this, ZG_Entity entity);

*/
void				ZG_EntitySystem_Delete(ZG_EntitySystem * _this);




#endif
