#ifndef __ZG_COLLISION_SYSTEM_H__
#define __ZG_COLLISION_SYSTEM_H__


typedef struct ZG_CollisionSystem ZG_CollisionSystem;

struct ZG_CollisionSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				ZG_CollisionSystem_Init(void);
void 				ZG_CollisionSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
ZG_CollisionSystem 	*		ZG_CollisionSystem_New(ZG_EntityManager *_entity_system);

// define callback functions on begin_collision,collision,end_collision
//

void					ZG_CollisionSystem_Update(ZG_CollisionSystem *_this);
void					ZG_CollisionSystem_Delete(ZG_CollisionSystem * _this);


#endif
