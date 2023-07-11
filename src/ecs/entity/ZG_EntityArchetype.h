#ifndef __ZG_ENTITY_TYPE_H__
#define __ZG_ENTITY_TYPE_H__


#define ZG_ENTITY_TYPE_GET_COMPONENT(_archetype, _type_data,_component_id) \
ZG_EntityType_GetComponent(\
	_archetype\
	,""#_type_data\
	,_component_id\
)

#define ZG_ENTITY_TYPE_GET_COMPONENT_IDX(_archetype, _type_data,_component_id) \
ZG_EntityType_GetComponentIdx(\
	_archetype\
	,""#_type_data\
	,_component_id\
)

#define ZG_ENTITY_TYPE_GET_COMPONENT_FROM_IDX(_archetype, _type_data_idx, _component_id) \
ZG_EntityType_GetComponentFromIdx(\
	_archetype\
	,_type_data_idx\
	,_component_id\
)


typedef unsigned int ZG_EntityArcheType;


// Static functions
ZG_EntityArcheType 	ZG_EntityType_New(
		ZG_EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


// The new entity is a handle of flags that locates its archetype and entity offset
ZG_Entity  					ZG_EntityType_NewEntity(ZG_EntityArcheType _entity_archetype);
void 				*		ZG_EntityType_GetComponent(ZG_EntityArcheType _entity_archetype, ZG_Entity _entity,const char * _component_name);

#endif
