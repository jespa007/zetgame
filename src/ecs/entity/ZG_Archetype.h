#ifndef __ZG_ARCHETYPE_H__
#define __ZG_ARCHETYPE_H__


#define ZG_ARCHETYPE_GET_COMPONENT(_archetype, _type_data,_component_id) \
ZG_Archetype_GetComponent(\
	_archetype\
	,""#_type_data\
	,_component_id\
)

#define ZG_ARCHETYPE_GET_COMPONENT_IDX(_archetype, _type_data,_component_id) \
ZG_Archetype_GetComponentIdx(\
	_archetype\
	,""#_type_data\
	,_component_id\
)

#define ZG_ARCHETYPE_GET_COMPONENT_FROM_IDX(_archetype, _type_data_idx, _component_id) \
ZG_Archetype_GetComponentFromIdx(\
	_archetype\
	,_type_data_idx\
	,_component_id\
)



struct ZG_Archetype{

	void *data;
};

// Static functions
ZG_Archetype 	*		ZG_Archetype_New(
		ZG_EntityManager *_this
		, const char *_id
		,uint16_t max_entities
		, ZG_EComponent * entity_components
		, size_t entity_components_len
);


// The new entity is a handle of flags that locates its archetype and entity offset
ZG_Entity  					ZG_Archetype_NewEntity(ZG_Archetype *_this);
void 				*		ZG_Archetype_GetComponent(ZG_Archetype *_this, ZG_Entity _entity,const char * _component_name);

#endif
