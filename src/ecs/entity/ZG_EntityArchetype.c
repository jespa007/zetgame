#include "../_zg_ecs_.h"

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

}ZG_ArchetypeData;


ZG_Entity  *ZG_Archetype_NewEntity(ZG_Archetype *_this){
	ZG_ArchetypeData *archetype_data=_this->data;
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

void 	*ZG_Archetype_GetComponent(ZG_Archetype *_this, const char *_component_name, ZG_ComponentId _component_id){
	ZG_ArchetypeData *data=_this->data;

	// search components array


	size_t component_idx=data->components_ref[_component_id];
	void *component_data=data->components[component_idx];

	asset(component_data!=NULL);

	return component_data;

}

void 	*ZG_Archetype_GetComponentIdx(ZG_Archetype *_this, const char *_component_name, ZG_ComponentId _component_position_idx){
	ZG_ArchetypeData *data=_this->data;

	// search components array
	data->


	size_t component_idx=data->components_ref[_component_position_idx];
	void *component_data=data->components[component_idx];

	asset(component_data!=NULL);

	return component_data;

}


