#ifndef __ZG_COLLISION_SYSTEM_H__
#define __ZG_COLLISION_SYSTEM_H__


typedef struct ZG_ECollisionSystem ZG_ECollisionSystem;

struct ZG_ECollisionSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_ECollisionSystem_Init(void);
void 				ZG_ECollisionSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_ECollisionSystem 	*		ZG_ECollisionSystem_New(ZG_EntitySystem *_entity_system);

// define callback functions on begin_collision,collision,end_collision
//

void					ZG_ECollisionSystem_Update(ZG_ECollisionSystem *_this);
void					ZG_ECollisionSystem_Delete(ZG_ECollisionSystem * _this);


#endif
