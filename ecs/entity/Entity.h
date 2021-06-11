#ifndef __ECS_ENTITY_H__
#define __ECS_ENTITY_H__

typedef struct Entity Entity;

// entityid it has lower 16 as the number of entity and upper 16 bits the type of entity
//typedef uint32_t EntityId;

#define ZG_INVALID_ENTITY_ID 0

struct Entity{
	void 		**components;//[ENTITY_COMPONENT_MAX];
	bool		active;
	void 		*data;

};

Entity *Entity_New(void);
void Entity_Start(Entity *_this);
void Entity_Die(Entity *_this);
void Entity_Reset(Entity *_this);
//void Entity_AttachComponent(Entity *_this, uint16_t idx_component, void *ptr_component);
//void Entity_DeAttachComponent(Entity *_this, uint16_t idx_component);
// Helper functions

void Entity_Delete(Entity *_this);


#endif
