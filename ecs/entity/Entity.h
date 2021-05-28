#ifndef __ENTITY_H__
#define __ENTITY_H__

typedef struct Entity Entity;

// entityid it has lower 16 as the number of entity and upper 16 bits the type of entity
//typedef uint32_t EntityId;

#define ZG_INVALID_ENTITY_ID 0

struct Entity{
	void *data;

};

Entity *Entity_New(void);
void Entity_Start(Entity *_this);
void Entity_Die(Entity *_this);
void Entity_Reset(Entity *_this);
void Entity_AttachComponent(Entity *_this, int idx_component, void *ptr_component);
void Entity_DeAttachComponent(Entity *_this, int idx_component);
void * Entity_GetComponent(Entity *_this, int idx_component);

// Helper functions
void Entity_SetTranslate3f(Entity *_this,float x, float y, float z);
Vector3f Entity_GetTranslate(Entity *_this,TransformNodeType transform_type);

void Entity_Delete(Entity *_this);


#endif
