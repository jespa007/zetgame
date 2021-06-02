#include "ecs/zg_ecs.h"
/*
typedef struct{
	Entity *entities; // max entities it'll have this type
	short max_entities; // max entitites of this type (default 1)
	short active_entities;
	MskEcTypes msk_ec_types; // it says the components it has this entity
}EntityTypeData;


EntityType *EntityType_New(int max, MskEcTypes msk_ec_types){
	EntityType *es_entity_type=NEW(EntityType);
	EntityTypeData *data=es_entity_type->data=NEW(EntityTypeData);
	es_entity_type->data=data;

	data->entities=malloc(sizeof(Entity)*max);
	data->max_entities=max;
	data->active_entities=0;
	data->msk_ec_types=msk_ec_types;

	return es_entity_type;
}

EntityType *EntityType_NewEntity(EntityType *_this){


	//return;
}*/

typedef struct{

}EntityTypeElement;

typedef struct{
	//Entity *entities; // max entities it'll have this type
	short max_entities; // max entitites of this type (default 1)
	short active_entities;
	ESSystem *es_system;
	char *name; // name entity type
	EntityComponent msk_ec_types; // it says the components it has this entity

	short *component_entity[ENTITY_COMPONENT_MAX]; // givent entity n it get the component from system

}EntityTypeData;


void  EntityType_New(ESSystem *es_system, EntityTypeId entity_id, const char *name,int max,EntityComponent msk_ec_types){
	EntityType * entity_type=NEW(EntityType);
	EntityTypeData *data=NEW(EntityTypeData);
	entity_type->data=data;

	data->es_system=es_system;

	//entity_type->id=(ESSystemData *)_this->data->entity_types->count;
	//data->entities=malloc(sizeof(Entity)*max);
	data->max_entities=max;
	data->active_entities=0;
	data->msk_ec_types=msk_ec_types;
	data->name=malloc(strlen(name)*sizeof(char));
	strcpy(data->name,name);

	uint32_t msk_ec_it=msk_ec_types;
	uint32_t component_id=0;
	// add components neededs for the type...
	while(msk_ec_it>>component_id){

		if(msk_ec_it & (msk_ec_it<<component_id)){ // ok get free component
			data->component_entity[component_id]=malloc(sizeof(short)*max); // add current components of this type by amount of max entities needed for this type
		}

		component_id++;
	};

	/*for(int i=0; i < max; i++){
		//entity_type->entities[i].type_id=entity_type->id;
	}*/


	// set all entity types as this type
}

Entity  EntityType_NewEntity(EntityType *entity_type, int posx, int posy){
	EntityTypeData *data=entity_type->data;
	Entity entity=ZG_INVALID_ENTITY_ID;
	if(data->active_entities<data->max_entities){
		uint32_t msk_ec_it=data->msk_ec_types;
		uint32_t component_id=0;
		unsigned int entity_id=data->active_entities;
		Entity entity=entity_type->id|entity_id;
		while(msk_ec_it>>component_id){
			uint32_t msk_component=(msk_ec_it & (msk_ec_it<<component_id));
			if(msk_component){ // attach component to entity
				unsigned id_component=ESSystem_NewComponent(data->es_system,msk_component); // request free component
				data->component_entity[component_id][entity_id]=id_component; // attach component to entity
			}
			component_id++;
		}
	}

	return entity;
}
