#include "../_zg_ecs_.h"


typedef struct{
	ZG_EntitySystem	*entity_system;
}ZG_ECollisionSystemData;

//------------------------------------------
// PUBLIC FUNCTIONS
ZG_ECollisionSystem *ZG_ECollisionSystem_New(ZG_EntitySystem *_entity_system){
	ZG_ECollisionSystem *system=ZG_NEW(ZG_ECollisionSystem);
	ZG_ECollisionSystemData *data=ZG_NEW(ZG_ECollisionSystemData);

	system->data=data;

	// after first system is created, user cannot register any component anymore
	return system;
}

// Register on begin_collision
// Register on collision
// Register on end_collision


void ZG_ECollisionSystem_Update(ZG_ECollisionSystem * _this){
	// foreach archetypes collisions
	// check collision foreach entity according its current collider
	// check flags begin/end to trigger its callback functions
}

void ZG_ECollisionSystem_Delete(ZG_ECollisionSystem *_this){
	ZG_ECollisionSystemData *data=(ZG_ECollisionSystemData *)_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
