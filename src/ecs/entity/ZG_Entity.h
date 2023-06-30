#ifndef __ZG_ENTITY_H__
#define __ZG_ENTITY_H__

// entityid it has lower 16 as the number of entity and upper 16 bits the type of entity
//typedef uint32_t EntityId;

#define ZG_INVALID_ENTITY_ID 0

#define ZG_ENTITY_GET_COMPONENT(_entity, _type_data) \
(_type_data *)ZG_Entity_GetComponent(ZG_Entity_GetEntityManager(_entity, __g_entity_system_component#_type_data);\


#define ZG_ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,_entity_system) assert(ZG_Entity_GetEntityManager(_entity)==_entity_system)
#define ZG_ASSERT_ENTITY_BELONGS_TO_ENTITY_MANAGER(_entity,_archetype) assert(ZG_Entity_GetArchetype(_entity)==_archetype)

typedef unsigned int ZG_Entity;

/*struct ZG_Entity{
//ZG_Archetype 		*archetype;//[ENTITY_COMPONENT_MAX];
//	bool		active;
	void 		*data;

};*/

ZG_Entity ZG_Entity_New(ZG_Archetype *_archetype);
void ZG_Entity_Start(ZG_Entity _entity);
void ZG_Entity_Die(ZG_Entity _entity);
void ZG_Entity_Reset(ZG_Entity _this);
void *ZG_Entity_GetComponent(ZG_Entity *_this, ZG_ComponentId _component_id);

//void Entity_AttachComponent(ZG_Entity *_this, uint16_t idx_component, void *ptr_component);
//void Entity_DeAttachComponent(ZG_Entity *_this, uint16_t idx_component);
// Helper functions
void ZG_Entity_Delete(ZG_Entity *_this);


#endif
