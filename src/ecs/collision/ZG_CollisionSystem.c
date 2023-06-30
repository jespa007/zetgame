#include "../_zg_ecs_.h"


typedef struct{
	ZG_EntityManager	*entity_system;
}ZG_CollisionSystemData;

//------------------------------------------
// PUBLIC FUNCTIONS
ZG_CollisionSystem *ZG_CollisionSystem_New(ZG_EntityManager *_entity_system){
	ZG_CollisionSystem *system=ZG_NEW(ZG_CollisionSystem);
	ZG_CollisionSystemData *data=ZG_NEW(ZG_CollisionSystemData);

	system->data=data;

	// after first system is created, user cannot register any component anymore
	return system;
}

// Register on begin_collision
// Register on collision
// Register on end_collision


void ZG_CollisionSystem_Update(ZG_CollisionSystem * _this){
	// foreach archetypes collisions
	// check collision foreach entity according its current collider
	// check flags begin/end to trigger its callback functions
}

void ZG_CollisionSystem_Delete(ZG_CollisionSystem *_this){
	ZG_CollisionSystemData *data=(ZG_CollisionSystemData *)_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
