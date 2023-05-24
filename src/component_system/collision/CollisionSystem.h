#ifndef __COLLISION_SYSTEM_H__
#define __COLLISION_SYSTEM_H__


typedef struct CollisionSystem CollisionSystem;

struct CollisionSystem{
	void *data;
};
//---------------------------------------------------
// STATIC FUNCTIONS
bool				CollisionSystem_Init(void);
void 				CollisionSystem_DeInit(void);

//---------------------------------------------------
// PUBLIC FUNCTIONS
CollisionSystem 	*		CollisionSystem_New(EntitySystem *_entity_system);

// define callback functions on begin_collision,collision,end_collision
//

void					CollisionSystem_Update(CollisionSystem *_this);
void					CollisionSystem_Delete(CollisionSystem * _this);


#endif
