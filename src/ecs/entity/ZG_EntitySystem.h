#ifndef __ZG_ENTITY_MANAGER_H__
#define __ZG_ENTITY_MANAGER_H__

#define ZG_UNLIMITIED_ENTITIES	-1

#define ZG_BITS_MAX_ENTITY_TYPES			10
#define ZG_BITS_MAX_ENTITIES_PER_TYPE		18


#define ZG_ECS_ADD_COMPONENT(_component_type, _on_update, _on_create,_on_destroy,_required_components) \
ZG_EntitySystem_RegisterComponent(\
	""#_component_type,\
	sizeof(_component_type)\
	,_on_update\
	,_on_create\
	,_on_destroy\
	,_required_components\
)


typedef struct{
	ZG_EComponentId id;
	const char *name;
	size_t 	size_data; // len data component

	void   (*on_update)(void *); // function to update component
	void   (*on_create)(void *); // function to Setup component
	void   (*on_destroy)(void *); // function to destroy
	ZG_ComponentList   (*required_components)(void); // function to destroy
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
/*
struct ZG_EntitySystem{
	void *data;
};*/
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
bool ZG_EntitySystem_RegisterComponent(
	const char *_component_id,
	size_t __type_data
	,const char * _required_components_ids[]
	,void   (*_on_create)(void *_component_data)
	,void   (*_on_update)(void *_component_data)
	,void   (*_on_destroy)(void *_component_data)
	,const char **  (*_required_components)(size_t * _n_components )
);

size_t				ZG_EntitySystem_NumComponents(void);
void 				ZG_EntitySystem_DeInit(void);

// it returns a handle of entity manager (it creates internally its pointers but all are referenced by an integer (like opengl)
int								ZG_EntitySystem_New(void);
void							ZG_EntitySystem_Delete(int _entity_manager_id);
void							ZG_EntitySystem_Destroy(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS


ZG_EntityType 			ZG_EntitySystem_NewEntityType(
		ZG_EntitySystem _entity_manager
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


// The new entity is a handle of flags that locates its archetype and entity offset
ZG_Entity  				EntitySystem_NewEntity(ZG_EntitySystem _entity_manager, ZG_EntityEntityType _entity_archetype);
void					EntitySystem_DestroyEntity(ZG_EntitySystem _entity_manager, ZG_Entity _entity_id);
void					ZG_EntitySystem_Update(ZG_EntitySystem _entity_manager);
void 					ZG_EntitySystem_GetEntityComponent(ZG_EntitySystem _entity_manager,ZG_Entity _entity, ZG_Component _component);
//uint8_t *EntitySystem_NewComponent(ZG_EntitySystem *_this,int idx_component);
/*void  			EntitySystem_RemoveEntity(ZG_EntitySystem * _this, ZG_Entity entity);

// Channels
ZG_Transform 	*		EntitySystem_GetComponentTransform(ZG_EntitySystem * _this, ZG_Entity entity);
TransformNode 		*		EntitySystem_GetComponentTransformNode(ZG_EntitySystem * _this, ZG_Entity entity);
ZG_Appearance 	*		EntitySystem_GetComponentAppearance(ZG_EntitySystem * _this, ZG_Entity entity);
ZG_Geometry 	*		EntitySystem_GetComponentGeometry(ZG_EntitySystem * _this, ZG_Entity entity);
Camera 		*		EntitySystem_GetComponentCamera(ZG_EntitySystem * _this, ZG_Entity entity);

*/
void				ZG_EntitySystem_Delete(ZG_EntitySystem _entity_manager);




#endif
