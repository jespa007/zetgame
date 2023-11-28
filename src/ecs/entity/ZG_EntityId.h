#ifndef __ZG_ENTITY_H__
#define __ZG_ENTITY_H__

typedef unsigned int ZG_EntityId;

// entityid it has lower 16 as the number of entity and upper 16 bits the type of entity
//typedef uint32_t EntityId;
/*
#define ZG_ENTITY_GET_COMPONENT(_entity, _type_data) \
(_type_data *)ZG_Entity_GetComponent(ZG_Entity_GetEntitySystem(_entity, __g_entity_system_component#_type_data);\


#define ZG_ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,_entity_system) assert(ZG_Entity_GetEntitySystem(_entity)==_entity_system)
#define ZG_ASSERT_ENTITY_BELONGS_TO_ENTITY_MANAGER(_entity,_archetype) assert(ZG_Entity_GetEntityType(_entity)==_archetype)

// an entity is a handle in the end
typedef unsigned int ZG_Entity;



//ZG_Entity ZG_Entity_New(ZG_EntityType *_archetype);
void ZG_Entity_Start(ZG_Entity _entity);
void ZG_Entity_Die(ZG_Entity _entity);
void ZG_Entity_Reset(ZG_Entity _this);
void *ZG_Entity_GetComponent(ZG_Entity *_this, ZG_ComponentId _component_id);*/

//void Entity_AttachComponent(ZG_Entity *_this, uint16_t idx_component, void *ptr_component);
//void Entity_DeAttachComponent(ZG_Entity *_this, uint16_t idx_component);
// Helper functions
//void ZG_Entity_Delete(ZG_Entity *_this);


#endif
