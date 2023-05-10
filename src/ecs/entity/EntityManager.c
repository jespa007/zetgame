#include "ecs/zg_ecs.h"

typedef struct {
	char 		*name; // name entity type
	EComponent *entity_components;// it says the components it has this entity
	size_t 		n_components; // available components

	uint16_t 	max_entities; // max entitites of this type (default 1)
	uint16_t 	n_entities;
	uint16_t 	active_entities;
	Entity 		**entities;

}EntityManagerData;



Entity  *EntityManager_CreateEntity(EntityManager *_this){
	EntityManagerData *entity_manager_data=_this->data;
	Entity *entity=NULL;
	if(
		entity_manager_data->active_entities>=entity_manager_data->max_entities
	){ // extend entity
		return NULL;
	}

	entity=entity_manager_data->entities[entity_manager_data->active_entities];
	entity_manager_data->active_entities++;

	entity->active=true;

	return entity;
}




