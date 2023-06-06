#ifndef __ENTITY_H__
#define __ENTITY_H__

// entityid it has lower 16 as the number of entity and upper 16 bits the type of entity
//typedef uint32_t EntityId;

#define ZG_INVALID_ENTITY_ID 0

#define ZG_ENTITY_GET_COMPONENT(_entity, _type_data) \
(_type_data *)Entity_GetComponent(Entity_GetEntitySystem(_entity, __g_entity_system_component#_type_data);\


#define ASSERT_ENTITY_BELONGS_TO_SYSTEM(_entity,_entity_system) assert(Entity_GetEntitySystem(_entity)==_entity_system)
#define ASSERT_ENTITY_BELONGS_TO_ENTITY_MANAGER(_entity,_entity_manager) assert(Entity_GetEntityManager(_entity)==_entity_manager)

struct Entity{
//EntityManager 		*entity_manager;//[ENTITY_COMPONENT_MAX];
//	bool		active;
	void 		*data;

};

Entity *Entity_New(EntityManager *_entity_manager);
void Entity_Start(Entity *_this);
void Entity_Die(Entity *_this);
void Entity_Reset(Entity *_this);
EntitySystem *Entity_GetEntitySystem(Entity *_this);
EntityManager *Entity_GetEntityManager(Entity *_this);
void *Entity_GetComponent(Entity *_this, ComponentId _component_id);

//void Entity_AttachComponent(Entity *_this, uint16_t idx_component, void *ptr_component);
//void Entity_DeAttachComponent(Entity *_this, uint16_t idx_component);
// Helper functions

void Entity_Delete(Entity *_this);


#endif
