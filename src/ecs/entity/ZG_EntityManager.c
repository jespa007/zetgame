#include "../zg_ecs.h"

//------------------------------------------------------------
// entities are simply a pointer that hides its entity manager.
// each entity knows its component offset by doing entity - entities


typedef struct {
	char 		*name; // entity manager name
	void 		**entity_components;// The pointer of component giving its component idx
	size_t 		n_entity_components; // available components per entity

	uint16_t 	max_entities; // max entitites of this type (default 1)
	uint16_t 	n_entities;
	uint16_t 	active_entities;

	ZG_Entity 		*entities;			// entity reference
	ZG_ComponentId	*entity_component_ref; 	// when creates or deletes, entity manager it moves components so each entity should know it new reference

}ZG_EntityManagerData;




ZG_Entity  *ZG_ZG_EntityManager_NewEntity(ZG_EntityManager *_this){
	ZG_EntityManagerData *entity_manager_data=_this->data;
	ZG_Entity *entity=NULL;
	if(
		entity_manager_data->active_entities>=entity_manager_data->max_entities
	){ // extend entity
		return NULL;
	}

	entity=entity_manager_data->entities[entity_manager_data->active_entities];
	entity_manager_data->active_entities++;


	return entity;
}

void 	*ZG_EntityManager_GetComponent(ZG_EntityManager *_this,ZG_Entity *_entity, ZG_ComponentId _component_id){
	ZG_EntityManagerData *data=_this->data;

	ZG_ASSERT_ENTITY_BELONGS_TO_ENTITY_MANAGER(_this,_entity);

	size_t entity_idx=_entity-data->entities;
	size_t entity_component_idx=data->entity_component_ref[entity_idx];
	void *component_data=data->entity_components[entity_component_idx];

	asset(component_data!=NULL);

	return component_data;

}


