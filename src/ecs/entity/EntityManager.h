#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

typedef uint16_t EntityManagerId;

struct EntityManager{

	void *data;
};

// Static functions
EntityManager 	*		EntityManager_New(
		EntitySystem *_this
		, const char *_id
		,uint16_t max_entities
		, EComponent * entity_components
		, size_t entity_components_len
);

Entity  		*		EntityManager_NewEntity(EntityManager *_this);

#endif
