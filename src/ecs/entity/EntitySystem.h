#ifndef __ENTITY_SYSTEM_H__
#define __ENTITY_SYSTEM_H__

#define UNLIMITIED_ENTITIES	-1

#define ZG_ECS_REGISTER_COMPONENT(_entity_system, _type_data, _extra_options) \
ComponentId __g_entity_system_component#_type_data=__g_entity_system_component++;\
EntitySystem_RegisterComponentBuiltin(__g_entity_system_component#_type_data,(EntitySystemRegisterEComponent){\
		.size_data				=sizeof(_data)\
		,.required_components	=_extra_options!=NULL?_extra_options->_required_components:NULL\
		,.EComponent_OnCreate	=_extra_options!=NULL?_extra_options->on_create:NULL\
		,.EComponent_OnUpdate	=_extra_options!=NULL?_extra_options->on_update:NULL\
		,.EComponent_OnDestroy	=_extra_options!=NULL?_extra_options->on_destroy:NULL\
	}

#define ZG_ECS_COMPONENT_GET_COMPONENT(_entity_system, _type_data) \
ComponentId __g_entity_system_component#_type_data=__g_entity_system_component++;\
EntitySystem_RegisterComponentBuiltin(__g_entity_system_component#_type_data,(EntitySystemRegisterEComponent){\
		.size_data				=sizeof(_data)\
		,.required_components	=_extra_options!=NULL?_extra_options->_required_components:NULL\
		,.EComponent_OnCreate	=_extra_options!=NULL?_extra_options->on_create:NULL\
		,.EComponent_OnUpdate	=_extra_options!=NULL?_extra_options->on_update:NULL\
		,.EComponent_OnDestroy	=_extra_options!=NULL?_extra_options->on_destroy:NULL\
	}



typedef struct{
	EComponent id;
	size_t 	size_data; // len data component
	EComponentList required_components;
	//void   (*EComponent_Setup)(void *, ComponentId _id); // function to Setup component
	void   (*EComponent_OnCreate)(void *); // function to Ini component
	void   (*EComponent_OnUpdate)(void *); // function to update component
	void   (*EComponent_OnDestroy)(void *);
	//void   (*EComponent_Destroy)(void *); // function to destroy
}EntitySystemRegisterEComponent;

struct EntitySystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				EntitySystem_Init(void);
int					EntitySystem_RegisterComponent(EntitySystemRegisterEComponent es_component_register);
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
Transform 	*		EntitySystem_GetComponentTransform(EntitySystem * _this, Entity entity);
TransformNode 		*		EntitySystem_GetComponentTransformNode(EntitySystem * _this, Entity entity);
Appearance 	*		EntitySystem_GetComponentAppearance(EntitySystem * _this, Entity entity);
Geometry 	*		EntitySystem_GetComponentGeometry(EntitySystem * _this, Entity entity);
Camera 		*		EntitySystem_GetComponentCamera(EntitySystem * _this, Entity entity);

*/
void				EntitySystem_Delete(EntitySystem * _this);




#endif
