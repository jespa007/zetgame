#include "../_zg_ecs_.h"

//------------------------------------------------------------
// entities are simply a pointer that hides its entity manager.
// each entity knows its component offset by doing entity - entities


typedef struct {
	char 			*name; // entity manager name
	void 			**components;// The pointer of component giving its component idx
	ZG_ComponentId	**components_ref; 	// when creates or deletes, entity manager it moves components so each entity should know it new reference
	size_t 			n_entities; // available components per entity

	uint16_t 		max_entities; // max entitites of this type (default 1)


}ZG_EntityManagerData;




ZG_Entity  *ZG_EntityManager_NewEntity(ZG_EntityManager *_this){
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

void 	*ZG_EntityManager_GetComponent(ZG_EntityManager *_this, const char *_component_name, ZG_ComponentId _component_id){
	ZG_EntityManagerData *data=_this->data;

	// search components array


	size_t component_idx=data->components_ref[_component_id];
	void *component_data=data->components[component_idx];

	asset(component_data!=NULL);

	return component_data;

}


