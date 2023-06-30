#ifndef __ZG_ENTITY_SYSTEM_H__
#define __ZG_ENTITY_SYSTEM_H__

#define ZG_UNLIMITIED_ENTITIES	-1

#define ZG_ECS_ADD_COMPONENT(_type_data, _on_create,_on_destroy, _on_update,_required_components) \
ZG_EntityManager_RegisterComponent(\
	""#_type_data,\
	sizeof(_type_data)\
	,_on_update\
	,_on_create\
	,_on_destroy\
	,_required_components\
)



typedef struct{
	ZG_ComponentId component_id;
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

struct ZG_EntityManager{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_EntityManager_Init(void);
/*void					ZG_EntityManager_RegisterComponent(
		ZG_EComponent _idx_component
		//,ZG_ESRegisterComponent es_component_register
		//ZG_EComponent id;
		,size_t 	_size_data // len data component
		,ZG_EComponentList required_components
		,void   (*EComponent_OnCreate)(void *_component_data) // set it up if component need to init or allocate resources on its creation
		,void   (*EComponent_OnUpdate)(void *_component_data) // function component
		,void   (*EComponent_OnDestroy)(void *_component_data)
);*/
bool ZG_EntityManager_RegisterComponent(
	const char *_component_id,
	size_t __type_data
	,const char * _required_components_ids[]
	,void   (*_on_create)(void *_component_data)
	,void   (*_on_update)(void *_component_data)
	,void   (*_on_destroy)(void *_component_data)
	,const char **  (*_required_components)(size_t * _n_components )
);

size_t				ZG_EntityManager_NumComponents(void);
void 				ZG_EntityManager_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_EntityManager 	*		ZG_EntityManager_New(void);
ZG_Archetype 	*		ZG_EntityManager_NewArchetype(
		ZG_EntityManager *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


ZG_Entity  				EntityManager_NewEntity(ZG_EntityManager *_this, const char *_archetype);
void					EntityManager_DestroyEntity(ZG_EntityManager *_this, ZG_Entity _entity);
void					ZG_EntityManager_Update(ZG_EntityManager *_this);
void 					ZG_EntityManager_GetComponent(ZG_EntityManager *_this, ZG_Entity * _entity, ZG_ComponentId _component_id);
//uint8_t *EntityManager_NewComponent(ZG_EntityManager *_this,int idx_component);
/*void  			EntityManager_RemoveEntity(ZG_EntityManager * _this, ZG_Entity entity);

// Channels
ZG_Transform 	*		EntityManager_GetComponentTransform(ZG_EntityManager * _this, ZG_Entity entity);
TransformNode 		*		EntityManager_GetComponentTransformNode(ZG_EntityManager * _this, ZG_Entity entity);
ZG_Appearance 	*		EntityManager_GetComponentAppearance(ZG_EntityManager * _this, ZG_Entity entity);
ZG_Geometry 	*		EntityManager_GetComponentGeometry(ZG_EntityManager * _this, ZG_Entity entity);
Camera 		*		EntityManager_GetComponentCamera(ZG_EntityManager * _this, ZG_Entity entity);

*/
void				ZG_EntityManager_Delete(ZG_EntityManager * _this);




#endif
