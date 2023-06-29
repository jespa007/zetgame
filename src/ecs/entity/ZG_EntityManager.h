#ifndef __ZG_ENTITY_MANAGER_H__
#define __ZG_ENTITY_MANAGER_H__


#define ZG_ENTITY_MANAGER_GET_COMPONENT(_entity_manager, _type_data,_component_id) \
ZG_EntityManager_GetComponent(\
	_entity_manager\
	,""#_type_data\
	,_component_id\
)

#define ZG_ENTITY_MANAGER_GET_COMPONENT_IDX(_entity_manager, _type_data,_component_id) \
ZG_EntityManager_GetComponentIdx(\
	_entity_manager\
	,""#_type_data\
	,_component_id\
)

#define ZG_ENTITY_MANAGER_GET_COMPONENT_FROM_IDX(_entity_manager, _type_data_idx, _component_id) \
ZG_EntityManager_GetComponentFromIdx(\
	_entity_manager\
	,_type_data_idx\
	,_component_id\
)



struct ZG_EntityManager{

	void *data;
};

// Static functions
ZG_EntityManager 	*		ZG_EntityManager_New(
		ZG_EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);

ZG_Entity  		*		ZG_EntityManager_NewEntity(ZG_EntityManager *_this);
ZG_EntitySystem	* 		ZG_EntityManager_GetEntitySystem(ZG_EntityManager *_this);
void 			*		ZG_EntityManager_GetComponent(ZG_EntityManager *_this, const char * _component_name, ZG_ComponentId _component_id);

#endif
