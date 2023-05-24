#include "component_system/zg_component_system.h"


typedef struct{
	EntitySystem	*entity_system;
}CollisionSystemData;

//------------------------------------------
// PUBLIC FUNCTIONS
CollisionSystem *CollisionSystem_New(EntitySystem *_entity_system){
	CollisionSystem *system=ZG_NEW(CollisionSystem);
	CollisionSystemData *data=ZG_NEW(CollisionSystemData);

	system->data=data;

	// after first system is created, user cannot register any component anymore
	g_entity_system_user_can_register_components=false;

	return system;
}

// Register on begin_collision
// Register on collision
// Register on end_collision


void CollisionSystem_Update(CollisionSystem * _this){
	// foreach entity_managers collisions
	// check collision foreach entity according its current collider
	// check flags begin/end to trigger its callback functions
}

void CollisionSystem_Delete(CollisionSystem *_this){
	CollisionSystemData *data=(CollisionSystemData *)_this->data;
	ZG_FREE(data);
	ZG_FREE(_this);
}
