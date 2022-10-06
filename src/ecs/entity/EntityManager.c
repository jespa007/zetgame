#include "ecs/zg_ecs.h"
/*
typedef struct{
	Entity *entities; // max entities it'll have this type
	short max_entities; // max entitites of this type (default 1)
	short active_entities;
	MskEcTypes msk_ec_types; // it says the components it has this entity
}EntityManagerData;


EntityManager *EntityManager_New(int max, MskEcTypes msk_ec_types){
	EntityManager *es_entity_manager=ZG_NEW(EntityManager);
	EntityManagerData *data=es_entity_manager->data=ZG_NEW(EntityManagerData);
	es_entity_manager->data=data;

	data->entities=malloc(sizeof(Entity)*max);
	data->max_entities=max;
	data->active_entities=0;
	data->msk_ec_types=msk_ec_types;

	return es_entity_manager;
}

EntityManager *EntityManager_NewEntity(EntityManager *_this){


	//return;
}*/

typedef struct{

}EntityManagerElement;

typedef struct{
	//Entity *entities; // max entities it'll have this type
	uint16_t max_entities; // max entitites of this type (default 1)
	uint16_t active_entities;
	ESSystem *es_system;
	char *name; // name entity type
	EntityComponent msk_ec_types; // it says the components it has this entity

	short **component_entity;//[ENTITY_COMPONENT_MAX]; // givent entity n it get the component from system

}EntityManagerData;


void  EntityManager_New(ESSystem *es_system, EntityManagerId entity_id, const char *name,int max,EntityComponent msk_ec_types){
	EntityManager * entity_manager=ZG_NEW(EntityManager);
	EntityManagerData *data=ZG_NEW(EntityManagerData);
	entity_manager->data=data;

	data->es_system=es_system;

	//entity_manager->id=(ESSystemData *)_this->data->entity_managers->count;
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
		//entity_manager->entities[i].type_id=entity_manager->id;
	}*/


	// set all entity types as this type
}

Entity  EntityManager_NewEntity(EntityManager *entity_manager, int posx, int posy){
	EntityManagerData *data=entity_manager->data;
	Entity entity=ZG_INVALID_ENTITY_ID;
	if(data->active_entities<data->max_entities){
		uint32_t msk_ec_it=data->msk_ec_types;
		uint32_t component_id=0;
		uint16_t entity_id=data->active_entities;
		Entity entity=entity_manager->id|entity_id;
		while(msk_ec_it>>component_id){
			uint32_t msk_component=(msk_ec_it & (msk_ec_it<<component_id));
			if(msk_component){ // attach component to entity
				uint16_t id_component=ESSystem_NewComponent(data->es_system,msk_component); // request free component
				data->component_entity[component_id][entity_id]=id_component; // attach component to entity
			}
			component_id++;
		}
	}

	return entity;
}
