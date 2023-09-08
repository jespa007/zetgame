#include "../@zg_ecs.h"

//------------------------------------------------------------
// entities are simply a pointer that hides its entity manager.
// each entity knows its component offset by doing entity - entities


typedef struct {
	char 			*name; // entity manager name
	void 			**component_idxs;// The pointer of component giving its component idx
	void 			**component_names;// The name of each component
	ZG_ComponentId	**components_ref; 	// when creates or deletes, entity manager it moves components so each entity should know it new reference
	size_t 			n_entities; // available components per entity

	uint16_t 		max_entities; // max entitites of this type (default 1)

}ZG_EntityTypeData;


ZG_Entity  *ZG_EntityType_NewEntity(ZG_EntityType *_this){
	ZG_EntityTypeData *archetype_data=_this->data;
	ZG_Entity *entity=NULL;
	if(
		archetype_data->active_entities>=archetype_data->max_entities
	){ // extend entity
		return NULL;
	}

	entity=archetype_data->entities[archetype_data->active_entities];
	archetype_data->active_entities++;


	return entity;
}

void 	*ZG_EntityType_GetComponent(ZG_EntityType *_this, const char *_component_name, ZG_ComponentId _component_id){
	ZG_EntityTypeData *data=_this->data;

	// search components array


	size_t component_idx=data->components_ref[_component_id];
	void *component_data=data->components[component_idx];

	asset(component_data!=NULL);

	return component_data;

}

void 	*ZG_EntityType_GetComponentIdx(ZG_EntityType *_this, const char *_component_name, ZG_ComponentId _component_position_idx){
	ZG_EntityTypeData *data=_this->data;

	// search components array
	data->


	size_t component_idx=data->components_ref[_component_position_idx];
	void *component_data=data->components[component_idx];

	asset(component_data!=NULL);

	return component_data;

}


